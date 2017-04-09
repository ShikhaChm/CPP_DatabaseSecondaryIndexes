#include<iostream>
#include <fstream>
#include<vector>
#include<map>
#include<algorithm>
#include<math.h>
#include "../EWAHBoolArray/headers/ewah.h"
#include "../EWAHBoolArray/headers/boolarray.h"
#include "utils.h"
using namespace std;

struct node2
{
	int min_key_value,max_key_value; //This is the keyValue
	int cardinality;
	EWAHBoolArray<int> bitmap; 
	string bitmapFilename;
	vector<node2 *> children;
};

void partitionBitmap(EWAHBoolArray<int> inputBitmap,int firstPartitionSize,EWAHBoolArray<int>& firstPartition,EWAHBoolArray<int>& secondPartition) {
	vector<int> recordsInBitVector;
	firstPartition.reset();
	secondPartition.reset();
	inputBitmap.appendSetBits(recordsInBitVector);
	if(firstPartitionSize > recordsInBitVector.size()){
		cout << "Bad Input! inputBitmap cardinality=" << recordsInBitVector.size() << "  firstPartitionSize=" << firstPartitionSize << endl;
		return;
	}
	for(int i=0;i<recordsInBitVector.size();i++) {
		if(i<firstPartitionSize) firstPartition.set(recordsInBitVector[i]);
		else secondPartition.set(recordsInBitVector[i]);
	}
	return;
}

class BTree
{
	public:
		BTree();
		BTree(map<int, EWAHBoolArray<int> > indexMap,map<string,int> enumMap,map<int,int> cardinalityMap,int branchingFactor,int treeType);
		~BTree();
		EWAHBoolArray<int> rangeQuery(int min,int max);
		void destroy_tree();
		void printTree();
		EWAHBoolArray<int> rangeQueryExternalMemory(int min,int max,string indexFolderPath,int& numDiskReads);
		void writeToXML(string indexFolderPath);
		void readFromXML(string indexFolderPath);
	private:
		void destroy_tree(node2 *leaf);
		void insert(node2* leaf,vector<node2 *> sortedNodes,int branchingFactor);
		void insertOptimal(node2* leaf,vector<node2 *> sortedNodes,int branchingFactor);
		void insertOptimal1(node2* leaf,vector<node2 *> sortedNodes,int branchingFactor);
		void rangeQuery(int min,int max,node2* leaf,EWAHBoolArray<int>& result);
		void rangeQueryExternalMemory(int min,int max,node2* leaf,EWAHBoolArray<int>& result,string indexFolderPath,int& numDiskReads);
		void writeToXML(node2 *someNode, ofstream& xmlFile,string indexFolderPath, int& nodeCount, string emptySpace);	
		int branchingFactor;
		int treeType;
		node2 *root;
};
void printNode(node2* n ) {
	cout << "{Min="<<n->min_key_value << "; Max=" << n->max_key_value << "; Card=" << n->cardinality <<"}  ";
}
void BTree::printTree(){
	vector<node2*> currentLevel,nextLevel;
	vector<int> currentLevelCount,nextLevelCount;
	currentLevel.push_back(root);
	currentLevelCount.push_back(-1);
	int level= 0;
	while(currentLevel.size()>0) {
		for(int i=0;i< currentLevel.size();i++) {
			cout<< endl;
			node2* thisNode= currentLevel[i];
			cout << "<LEVEL-"<< level<<"><CurrentLevel "<<i <<"><ParentLevel " << currentLevelCount[i] <<"> <" ;
			for(int j=0;j<level;j++) cout << "===" ;
			cout << ">" ;
			printNode(thisNode);
			//vector<int> rc;
			//(thisNode->bitmap;
			//cout << "   actual Cardinality " << rc.size() ;
			vector<node2*> children = thisNode->children;
			for(int j=0; j< children.size();j++) {
				nextLevel.push_back(children[j]);
				nextLevelCount.push_back(i);
			}
		}
		currentLevel=nextLevel;
		currentLevelCount=nextLevelCount;
		nextLevel.clear();
		nextLevelCount.clear();
		level= level+1;
		cout << endl;
	}
}
BTree::BTree()
{
	root=NULL;
}
BTree::~BTree()
{
	destroy_tree();
}
void BTree::destroy_tree(node2 *leaf)
{
	if(leaf!=NULL)
	{
		if((leaf->children).size()>0) {
			for(int i=0;i<(leaf->children).size();i++ ) destroy_tree(leaf->children[i]);
		}
		delete leaf;
	}
}
BTree::BTree(map<int, EWAHBoolArray<int> > indexMap,map<string,int> enumMap,map<int,int> cardinalityMap, int branchingFactor,int treeType) {
	vector<node2*> currentLevel;
	map<int,string> reverseEnumMap;
	for (map<string, int>::iterator i = enumMap.begin(); i != enumMap.end(); ++i) reverseEnumMap[i->second] = i->first;
	for( map<int,EWAHBoolArray<int> >::iterator ii=indexMap.begin(); ii!=indexMap.end(); ++ii)
	{
		int key=(*ii).first;
		node2* tmpNode= new node2;

		tmpNode->min_key_value=atoi(reverseEnumMap[key].c_str());
		tmpNode->max_key_value=atoi(reverseEnumMap[key].c_str());
		tmpNode->cardinality=cardinalityMap[key];
		tmpNode->bitmap=(*ii).second;
		if(currentLevel.empty()) currentLevel.push_back(tmpNode);
		else if(tmpNode->min_key_value <= (currentLevel[0])->min_key_value) currentLevel.insert(currentLevel.begin(),tmpNode);
		else if(tmpNode->min_key_value >= (currentLevel[currentLevel.size()-1])->min_key_value ) currentLevel.push_back(tmpNode);
		else {
			vector<node2*>::iterator it = currentLevel.begin();
			while(true) {
				it=it+1;
				if(tmpNode->min_key_value < (*it)->min_key_value) {
					currentLevel.insert(it,tmpNode);
					break;
				}
			}
		}
	}
	root= new node2;

	if( treeType==1) 	insertOptimal(root,currentLevel,branchingFactor); //Optimal structure that we didn't want
	else if( treeType==2)   	insertOptimal1(root,currentLevel,branchingFactor); //optimal structure that we wanted

	else   	insert(root,currentLevel,branchingFactor); //sub- optimal structure

//Print all distinct nodes & their cardinality
//for ( int j=0;j<currentLevel.size();j++) cout<<"min= "<<currentLevel[j]->min_key_value<<" max= " << currentLevel[j]->max_key_value << " Card= " << currentLevel[j]->cardinality<<endl;
}
//Optimal Binary Tree Structure type1

void BTree::insertOptimal1(node2* leaf,vector<node2 *> sortedNodes,int branchingFactor) 
{		int numNodes= sortedNodes.size();
	if(numNodes==0) return;
	else if(numNodes==1) {
		node2* tmpNode=sortedNodes[0];
		leaf->min_key_value=tmpNode->min_key_value;
		leaf->max_key_value=tmpNode->max_key_value;
		leaf->cardinality= tmpNode->cardinality;
		leaf->bitmap = tmpNode->bitmap;
		leaf->children = tmpNode->children;
		return;
	}
	leaf->min_key_value = sortedNodes[0]->min_key_value;
	leaf->max_key_value = (sortedNodes[sortedNodes.size()-1])->max_key_value;
	EWAHBoolArray<int> tmpBitmap;
	vector<int> cumulativeCardinality;
	for(int i=0;i<numNodes;i++) {
		tmpBitmap=leaf->bitmap;
		tmpBitmap.logicalor((sortedNodes[i])->bitmap,leaf->bitmap);
		if(i==0) cumulativeCardinality.push_back(sortedNodes[i]->cardinality);
		else cumulativeCardinality.push_back(cumulativeCardinality[i-1]+sortedNodes[i]->cardinality);
	}
	leaf->cardinality= cumulativeCardinality[numNodes-1];
	if(numNodes<=branchingFactor) {
		leaf->children=sortedNodes;
	} else {
		int counter=0, childNodeCount=0;
		node2* tmpNode;
		vector<node2*> tmpSortedNodes;
		int cardinalitySum=0;

		int i=0;
		while(i<sortedNodes.size()) {
			if(counter==0) {
				tmpNode= new node2;
				leaf->children.push_back(tmpNode);
				childNodeCount=childNodeCount+1;
			}
			tmpSortedNodes.push_back(sortedNodes[i]);
			cardinalitySum=cardinalitySum+(sortedNodes[i])->cardinality;
			counter=counter+1;
			if(cardinalitySum==leaf->cardinality*((double)childNodeCount/(double)branchingFactor)) {
				counter=0;
				insertOptimal(tmpNode,tmpSortedNodes,branchingFactor);
				tmpSortedNodes.clear();
			}else if(cardinalitySum>leaf->cardinality*((double)childNodeCount/(double)branchingFactor)){
				counter=0;
				int lastCardinalitySum=cardinalitySum-(sortedNodes[i])->cardinality;
				int firstPartitionSize=ceil(leaf->cardinality*((double)childNodeCount/(double)branchingFactor))-lastCardinalitySum;
				int secondPartitionSize=cardinalitySum-ceil(leaf->cardinality*((double)childNodeCount/(double)branchingFactor));
				if(secondPartitionSize==0) {
					insertOptimal1(tmpNode,tmpSortedNodes,branchingFactor);
				} else {
					//we need to split the bit vector for this node
					node2 *original=tmpSortedNodes.back();
					tmpSortedNodes.pop_back();
					cardinalitySum=lastCardinalitySum;
					node2 *part1=new node2;
					node2 *part2=new node2;
					part1->min_key_value = original->min_key_value;
					part1->max_key_value = original->max_key_value;
					part1->cardinality=firstPartitionSize;
					part2->min_key_value = original->min_key_value;
					part2->max_key_value = original->max_key_value;
					part2->cardinality=secondPartitionSize;
					EWAHBoolArray<int> firstPartition,secondPartition;
					partitionBitmap(original->bitmap,part1->cardinality,firstPartition,secondPartition);
					part1->bitmap=firstPartition;
					part2->bitmap=secondPartition;
					tmpSortedNodes.push_back(part1);
					cardinalitySum=cardinalitySum+firstPartitionSize;
					int tmpCard=0;
					for(int j=0;j<tmpSortedNodes.size();j++) {
						tmpCard=tmpCard+tmpSortedNodes[j]->cardinality;
						//			cout << "     Tmp Sorted Node " << j << " min=" << tmpSortedNodes[j]->min_key_value << " max=" << tmpSortedNodes[j]->max_key_value << " Card=" << tmpSortedNodes[j]->cardinality<< endl;
					}
					insertOptimal1(tmpNode,tmpSortedNodes,branchingFactor);
					sortedNodes.insert((sortedNodes.begin()+1+i),part2);
				}
				tmpSortedNodes.clear();
			}
			i=i+1;
		}
	}
}


//Optimal Binary Tree Structure type2 (What we wanted)

void BTree::insertOptimal(node2* leaf,vector<node2 *> sortedNodes,int branchingFactor) {
	int numNodes= sortedNodes.size();
	if(numNodes==0) return;
	else if(numNodes==1) {
		node2* tmpNode=sortedNodes[0];
		leaf->min_key_value=tmpNode->min_key_value;
		leaf->max_key_value=tmpNode->max_key_value;
		leaf->cardinality= tmpNode->cardinality;
		leaf->bitmap = tmpNode->bitmap;
		leaf->children = tmpNode->children;
		return;
	}
	leaf->min_key_value = sortedNodes[0]->min_key_value;
	leaf->max_key_value = (sortedNodes[sortedNodes.size()-1])->max_key_value;
	EWAHBoolArray<int> tmpBitmap;
	vector<int> cumulativeCardinality;
	for(int i=0;i<numNodes;i++) {
		tmpBitmap=leaf->bitmap;
		tmpBitmap.logicalor((sortedNodes[i])->bitmap,leaf->bitmap);
		if(i==0) cumulativeCardinality.push_back(sortedNodes[i]->cardinality);
		else cumulativeCardinality.push_back(cumulativeCardinality[i-1]+sortedNodes[i]->cardinality);
	}
	leaf->cardinality= cumulativeCardinality[numNodes-1];
	if(leaf->cardinality<branchingFactor) {
		leaf->children=sortedNodes;
	} else {
		int counter=0, childNodeCount=0;
		node2* tmpNode;
		vector<node2*> tmpSortedNodes;
		int cardinalitySum=0;

		int i=0;
		while(i<sortedNodes.size()) {
			if(counter==0) {
				tmpNode= new node2;
				leaf->children.push_back(tmpNode);
				childNodeCount=childNodeCount+1;
			}
			tmpSortedNodes.push_back(sortedNodes[i]);
			cardinalitySum=cardinalitySum+(sortedNodes[i])->cardinality;
			counter=counter+1;
			if(cardinalitySum==leaf->cardinality*((double)childNodeCount/(double)branchingFactor)) {
				counter=0;
				insertOptimal(tmpNode,tmpSortedNodes,branchingFactor);
				tmpSortedNodes.clear();
			}else if(cardinalitySum>leaf->cardinality*((double)childNodeCount/(double)branchingFactor)){
				counter=0;
				int lastCardinalitySum=cardinalitySum-(sortedNodes[i])->cardinality;
				int firstPartitionSize=ceil(leaf->cardinality*((double)childNodeCount/(double)branchingFactor))-lastCardinalitySum;
				int secondPartitionSize=cardinalitySum-ceil(leaf->cardinality*((double)childNodeCount/(double)branchingFactor));
				if(secondPartitionSize==0) {
					insertOptimal(tmpNode,tmpSortedNodes,branchingFactor);
				} else {
					//we need to split the bit vector for this node
					node2 *original=tmpSortedNodes.back();
					tmpSortedNodes.pop_back();
					cardinalitySum=lastCardinalitySum;
					node2 *part1=new node2;
					node2 *part2=new node2;
					part1->min_key_value = original->min_key_value;
					part1->max_key_value = original->max_key_value;
					part1->cardinality=firstPartitionSize;
					part2->min_key_value = original->min_key_value;
					part2->max_key_value = original->max_key_value;
					part2->cardinality=secondPartitionSize;
					EWAHBoolArray<int> firstPartition,secondPartition;
					partitionBitmap(original->bitmap,part1->cardinality,firstPartition,secondPartition);
					part1->bitmap=firstPartition;
					part2->bitmap=secondPartition;
					tmpSortedNodes.push_back(part1);
					cardinalitySum=cardinalitySum+firstPartitionSize;
					int tmpCard=0;
					for(int j=0;j<tmpSortedNodes.size();j++) {
						tmpCard=tmpCard+tmpSortedNodes[j]->cardinality;
						//			cout << "     Tmp Sorted Node " << j << " min=" << tmpSortedNodes[j]->min_key_value << " max=" << tmpSortedNodes[j]->max_key_value << " Card=" << tmpSortedNodes[j]->cardinality<< endl;
					}
					insertOptimal(tmpNode,tmpSortedNodes,branchingFactor);
					sortedNodes.insert((sortedNodes.begin()+1+i),part2);
				}
				tmpSortedNodes.clear();
			}
			i=i+1;
		}
	}

}

//Sub-Optimal tree structure
void BTree::insert(node2* leaf, vector<node2*> sortedNodes,int branchingFactor ) {
	int numNodes= sortedNodes.size();
	if(numNodes==0) return;
	else if(numNodes==1) {
		node2* tmpNode=sortedNodes[0];
		leaf->min_key_value=tmpNode->min_key_value;
		leaf->max_key_value=tmpNode->max_key_value;
		leaf->cardinality= tmpNode->cardinality;
		leaf->bitmap = tmpNode->bitmap;
		leaf->children = tmpNode->children;
		return;
	}
	leaf->min_key_value = sortedNodes[0]->min_key_value;
	leaf->max_key_value = (sortedNodes[sortedNodes.size()-1])->max_key_value;
	EWAHBoolArray<int> tmpBitmap;
	vector<int> cumulativeCardinality;
	for(int i=0;i<numNodes;i++) {
		tmpBitmap=leaf->bitmap;
		tmpBitmap.logicalor((sortedNodes[i])->bitmap,leaf->bitmap);
		if(i==0) cumulativeCardinality.push_back(1);//sortedNodes[i]+ ->cardinality);
		else cumulativeCardinality.push_back(cumulativeCardinality[i-1]+1);//sortedNodes[i]->cardinality);
	}
	leaf->cardinality= cumulativeCardinality[numNodes-1];
	if(numNodes <= branchingFactor) {
		leaf->children=sortedNodes;
	} else {
		int counter=0, childNodeCount=0;
		node2* tmpNode;
		vector<node2*> tmpSortedNodes;
		for(int i=0;i<numNodes;i++){
			if(counter==0) {
				tmpNode= new node2;
				leaf->children.push_back(tmpNode);
				childNodeCount=childNodeCount+1;
			}
			tmpSortedNodes.push_back(sortedNodes[i]);
			counter=counter+1;
			if((1+i)>=numNodes*((double)childNodeCount/(double)branchingFactor)) {
				counter=0;
				insert(tmpNode,tmpSortedNodes,branchingFactor);
				tmpSortedNodes.clear();
			}
		}
	}
}


void BTree::writeToXML(string indexFolderPath) {
	string xmlFilePath= indexFolderPath+"/treeIndex.xml";
	::remove(xmlFilePath.c_str());
	ofstream xmlFile(xmlFilePath.c_str(), ios::out | ios::binary);
	int nodeCount=0;
	string emptySpace="";
	xmlFile << "<?xml version=\"1.0\"?>" << endl;
	writeToXML(root,xmlFile,indexFolderPath,nodeCount,emptySpace);
	xmlFile.close();
	cout << "Total Number of nodes Written " << nodeCount << endl;
}

void BTree::writeToXML(node2 *someNode, ofstream& xmlFile,string indexFolderPath,int& nodeCount,string emptySpace) {
	if(someNode != NULL) {
		nodeCount=nodeCount+1;
		// Write the opening tag entry for the given node in the xml file
		xmlFile << emptySpace.c_str() << "<node min_key_value=" << someNode->min_key_value << " max_key_value=" << someNode->max_key_value << " cardinality="<<someNode->cardinality << " bitmapFilename=" << nodeCount <<".bin >" << endl;
		// Write the bin file to disk
		char binFileName[100];
		sprintf(binFileName,"%i.bin",nodeCount);
//		cout<<"/ binfilename "<<binFileName<<endl;
		string indexFilePath=indexFolderPath+"/"+ binFileName;
		EWAHBoolArray<int> index=someNode->bitmap;
		::remove(indexFilePath.c_str());
		ofstream out(indexFilePath.c_str(), ios::out | ios::binary);
		index.write(out);
		out.close();
		// write down all the child nodes of the given node.
		vector<node2*> childNodes = someNode->children;
		if(childNodes.size() > 0) {
			for(int i=0; i < childNodes.size(); i++ ) {
				writeToXML(childNodes[i],xmlFile,indexFolderPath,nodeCount, emptySpace+"    ");
			}
		}
		// Write the closing tag entry for the given node in the xml file
		xmlFile << emptySpace.c_str() << "</node>" << endl;
	}
}
void BTree::readFromXML(string indexFolderPath) {
	string indexFilePath= indexFolderPath+"/treeIndex.xml";
	vector<node2*> nodeStack;
	ifstream inf;
	inf.open(indexFilePath.c_str());
	// While there's still stuff left to read
	string line;
	getline(inf,line); //This gets the  xml header line of the file. Subsequent calls will fetch the node data
	string openNodePattern="<node";
	string closeNodePattern="</node";
	while (getline(inf,line))
	{
		size_t foundOpen= line.find(openNodePattern);
		size_t foundClose= line.find(closeNodePattern);
		if(foundOpen != string::npos) {
			//new node found
			node2* someNode= new node2;
			string cleanOfWhiteSpace= trim(line);		
			vector<string> attributeValuePairs = split(cleanOfWhiteSpace,' ');
			someNode->min_key_value=atoi(((split(attributeValuePairs[1],'='))[1]).c_str());
			someNode->max_key_value=atoi(((split(attributeValuePairs[2],'='))[1]).c_str());
			someNode->cardinality  =atoi(((split(attributeValuePairs[3],'='))[1]).c_str());
			someNode->bitmapFilename=string((split(attributeValuePairs[4],'='))[1]);
			if(nodeStack.size()==0) {
				root=someNode;
				nodeStack.push_back(someNode);
			} else {
				node2* parentNode= nodeStack.back();
				(parentNode->children).push_back(someNode);
				nodeStack.push_back(someNode);
			}
		} else if(foundClose != string::npos) {
			//close node found
			nodeStack.pop_back();
		}
	}


}

void BTree::rangeQuery(int min, int max, node2* leaf,EWAHBoolArray<int>& result) {
	if(leaf != NULL) {
		if(min >leaf->max_key_value || max < leaf->min_key_value) {
			//ZERO OVERLAP CASE
			//Do nothing don't look in any children;
		}
		else if(min <= leaf->min_key_value && max >=leaf->max_key_value) {
			// Full overlap
			//add it and donot look Forward
			EWAHBoolArray<int > resultOld =result;
			resultOld.logicalor(leaf->bitmap,result);
		} else {
			//some sort of partial overlap of range
			//donot add, look in all children
			vector<node2*> children=leaf->children;
			for(int i=0;i<children.size();i++) rangeQuery(min,max,children[i],result);
		} 
	}
}

EWAHBoolArray<int> BTree::rangeQuery(int min,int max) {
	EWAHBoolArray<int> result;
	rangeQuery(min,max,root,result);
	return result;
}

void BTree::rangeQueryExternalMemory(int min, int max, node2* leaf,EWAHBoolArray<int>& result, string indexFolderPath,int& numDiskReads) {
	if(leaf != NULL) {
		if(min >leaf->max_key_value || max < leaf->min_key_value) {
			//ZERO OVERLAP CASE
			//Do nothing don't look in any children;
		}
		else if(min <= leaf->min_key_value && max >=leaf->max_key_value) {
			// Full overlap
			//add it and donot look Forward
			EWAHBoolArray<int > resultOld =result;
			// read bitmap from disk
			EWAHBoolArray<int > leafBitmap;
			string bitmapFilePath = indexFolderPath+"/"+leaf->bitmapFilename;
			ifstream in(bitmapFilePath.c_str(), ios::binary);
			leafBitmap.read(in);
			in.close();
			numDiskReads=numDiskReads+1;
		//	system("ls -l ../data/ds21/indexType2/index6/10.bin | awk '{print $5}'");	
			// proceed as in case of in memory  query
			resultOld.logicalor(leafBitmap,result);
		} else {
			//some sort of partial overlap of range
			//donot add, look in all children
			vector<node2*> children=leaf->children;
			for(int i=0;i<children.size();i++) rangeQueryExternalMemory(min,max,children[i],result,indexFolderPath,numDiskReads);
		} 
	}
}


EWAHBoolArray<int> BTree::rangeQueryExternalMemory(int min,int max, string indexFolderPath,int& numDiskReads)  {
	EWAHBoolArray<int> result;
	rangeQueryExternalMemory(min,max,root,result,indexFolderPath,numDiskReads);
	return result;
}

void BTree::destroy_tree()
{
	destroy_tree(root);
}

//Function to return current system time
const std::string currentTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%X", &tstruct);

	return buf;
}


