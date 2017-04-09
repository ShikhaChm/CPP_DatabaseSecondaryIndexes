#include <iostream> 
#include <stdio.h>
#include <stdlib.h> 
#include <memory> 
#include <limits>
#include "utils.h"
using namespace std;

const int MAX = 256 ;
const int MIN = 2 ;
const int INTMIN=std::numeric_limits<int>::min();
const int INTMAX=std::numeric_limits<int>::max();

struct bplusNode { 
	int count ;
	int value[MAX + 1] ;
	bplusNode *child[MAX + 1] ;
	int recordId[MAX+1];
	bool isLeaf;
} ;
vector<string> stringsplit(const string &s, char delim) {
    vector<string> elems;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

class bplusTree { 
	private : 
		bplusNode *root ;
		void rangeQuery(int minValue,int maxValue, vector<int> &result,bplusNode* node);
		void rangeQueryExternalMemory(int minValue,int maxValue, vector<int> &result,string nodeFileName,string indexFolderPath,int& nodeCount);
 		int writeToXML(bplusNode *someNode,string indexFolderPath, int& nodeCount);
		int setval ( int val,int recId, bplusNode *n, int *p, int *r,bplusNode **c ) ;
		static bplusNode * search ( int val, bplusNode *root, int *pos ) ;
		static int searchnode ( int val, bplusNode *n, int *pos,int firstOrLast ) ;
		static int binarySearchFirst(int val,bplusNode *someNode, int *pos,int begin,int end);
		static int binarySearchLast(int val,bplusNode *someNode, int *pos,int begin,int end);
		static int searchVector ( int val, vector<int> allValues, int *pos ,int firstOrLast); 
		static int binarySearchVectorFirst(int val,vector<int> allValues, int *pos,int begin,int end);
		static int binarySearchVectorLast(int val,vector<int> allValues, int *pos,int begin,int end);
		void fillnode ( int val, int recId,bplusNode *c, bplusNode *n, int k ) ;
		void split ( int val,int recId, bplusNode *c, bplusNode *n, int k, int *y, int *r,bplusNode **newnode ) ;
		int getLargestVal(bplusNode *n);

	public : 
		bplusTree( ) ;
		void insert ( int val, int recId );
		void show( ) ;
		static void display ( bplusNode *root ,int level) ;
		void writeToXML(string indexFolderPath);
		vector<int> rangeQuery(int minValue,int maxValue);
		vector<int> rangeQueryExternalMemory(int minValue,int maxValue,string indexFolderPath,int& nodeCount);
		static void deltree ( bplusNode *root ) ;
		~bplusTree( ) ;
};
bplusTree :: bplusTree( ) { 
	root = NULL ;
}
void bplusTree :: insert ( int val ,int recId) { 
	int i,r ;
	bplusNode *c, *n ;
	int flag ;
	flag = setval ( val,recId, root, &i,&r, &c ) ;
	if ( flag ) { 
		//cout << "setval returned non zero" << endl;
		n = new bplusNode ;
		n -> isLeaf=true;
		n -> count = 1 ;
		n -> value[1] = i ;
		n -> recordId[1]=r;
		n -> child[0] = root ;
		n -> child[1] = c ;
		root = n ;
	} 
	if(flag==2) {
		//cout << "setval returned 2 " << endl;
		root -> isLeaf=false;
	}
}
int bplusTree :: setval ( int val,int recId, bplusNode *n, int *p, int *r,bplusNode **c ) { 
	int k ,r1;
	if ( n == NULL ) { 
		//cout << "setval base case hit " << endl;
		*p = val ;
		*r = recId;
		*c = NULL ;
		return 1 ;
	}
	else { 
		//if ( searchnode ( val, n, &k ) ) cout << endl << "Key value already exists." << endl ;
		searchnode ( val, n, &k ,0);	
		//cout << "setval going for recursive call for position " << k << endl;
		int flag=setval ( val, recId,n -> child[k], p, &r1,c ); 
		if (flag) { 
			//cout << "setval recursive call returned "<< flag << "  n->count=" << n->count<< endl;
			//show();
			if(flag==2) n->isLeaf=false; 
			if ( n -> count < MAX ) { 
				//cout << "Simple Fill" << endl; 
				fillnode ( *p,r1, *c, n, k ) ;
				(*r)=r1;
				return 0 ;
			}
			else { 
				//cout << "Going to split node" << endl;
				split ( *p, r1,*c, n, k, p,r, c ) ;
				//cout << "After split this is what the child has " << endl;
				//display(*c,0);
				return 2 ;
			}
			//show();
		}
		return 0 ;
	}
}
void bplusTree :: fillnode ( int val,int recId, bplusNode *c, bplusNode *n, int k ) { 
	//cout << "Fillnode called val=" << val << " recId=" << recId << "  k=" << k << " leaf=" << n->isLeaf<<endl;
	int i ;
	for ( i = n -> count ;i > k ;i-- ) { 
		n -> value[i + 1] = n -> value[i] ;
		n -> recordId[i + 1] = n -> recordId[i] ;
		n -> child[i + 1] = n -> child[i] ;
	}
	if(n->isLeaf) {
		//cout << "fillnode 1" <<endl;
		n -> value[k + 1] = val ;
		n -> child[k + 1] = c ;
		n -> recordId[k + 1]=recId;
		n -> count++ ;
	} else if(recId != -1){
		//cout << "fillnode 2" <<endl;
		int k1;
		searchnode(val,c,&k1,0);
		fillnode(val,recId,NULL,c,k1);
	}else {
		//cout << "fillnode 3" <<endl;
		n -> value[k + 1] =val; 
		n -> child[k + 1] = c ;
		n -> recordId[k + 1]=recId;
		n -> count++ ;
		if(n->child[0]==NULL) {
			for(int i=0; i < n->count; i++) n->child[i]= n->child[i+1];
			for(int i=1; i < n->count; i++) n->value[i]= n->value[i+1];
			n->count--;
		}
	}
}
void bplusTree :: split ( int val, int recId, bplusNode *c, bplusNode *n, int k, int *y, int *r,bplusNode **newnode ) { 
	//cout << " Split called val=" << val << " recId=" << recId << " k=" << k << endl;
	//cout << "Child node send to split is " << endl;
	//display(c,0);
	//cout << "Child node send to split : display complete " << endl;
	int i, mid ;
	//if ( k <= MIN ) mid = MIN ;
	//else mid = (MIN +MAX)/2 ;
	mid = (MIN +MAX)/2 ;
	//cout << "Mid = " << mid << endl;
	*newnode = new bplusNode ;
	if(recId!=-1) {
		//cout << "Splitting Leaf node" << endl;
		(*newnode)->isLeaf=true;
		for ( i = mid + 1 ;i <= MAX ;i++ ) { 
			( *newnode ) -> value[i - mid] = n -> value[i] ;
			( *newnode ) -> recordId[i - mid] = n -> recordId[i] ;
			( *newnode ) -> child[i - mid] = n -> child[i] ;
			//cout << "copying i=" << i << endl;
		}
		( *newnode ) -> child[0] = NULL ;
		( *newnode ) -> count = MAX - mid ;
		n -> count = mid ;
		//cout << "In Split before fillnode, the value of new node is " << endl;
		//display((*newnode),0);
		if ( k <= mid ) {
		//	cout << "k is less MIN" << endl;
			fillnode ( val,recId, c, n, k ) ;
		}
		else fillnode ( val, recId, c, *newnode, k - mid ) ;
	} else {
		//cout << "splitting non leaf node" << endl;
		(*newnode)->isLeaf=false;
		for ( i = mid + 1 ;i <= MAX ;i++ ) { 
			( *newnode ) -> value[i - mid] = n -> value[i] ;
			( *newnode ) -> recordId[i - mid] = n -> recordId[i] ;
			( *newnode ) -> child[i - mid] = n -> child[i] ;
			//cout << "copying i=" << i << endl;
		}
		( *newnode ) -> child[0] = NULL ;
		( *newnode ) -> count = MAX - mid ;
		n -> count = mid ;
		//cout << "In Split before fillnode, the value of new node is " << endl;
		//display((*newnode),0);
		if ( k <= mid ) {
			fillnode ( val,recId, c, n, k ) ;
			if((*newnode)->child[0]==NULL) {
				for(int i=0; i < (*newnode)->count; i++) (*newnode)->child[i]= (*newnode)->child[i+1];
				for(int i=1; i < (*newnode)->count; i++) (*newnode)->value[i]= (*newnode)->value[i+1];
				(*newnode)->count--;
			}
		}
		else fillnode ( val, recId, c, *newnode, k - mid ) ;
	}
	if(n->isLeaf) *y = n -> value[n -> count] ;
	else *y = getLargestVal(n);
	*r=-1;
}
int bplusTree::getLargestVal(bplusNode *n) {
	//cout << "getLargestVal 0 " << endl;
	if(n==NULL) {
		//cout << "getLargestVal 1 " << endl;
		return -2;
	} 
	else {
		//cout << "getLargestVal 1.5" << endl;
		if(n->isLeaf) {
			//cout << "getLargestVal 2 " << endl;
			return n->value[n->count];
		}
		else {
			//cout << "getLargestVal 3 " << endl;
			return getLargestVal(n->child[n->count]);
		}
	}
}
bplusNode * bplusTree :: search ( int val, bplusNode *root, int *pos ) { 
	if ( root == NULL ) return NULL ;
	else { if ( searchnode ( val, root, pos,0 ) ) return root ;
		else return search ( val, root->child[*pos], pos ) ;
	}
}
int bplusTree :: searchnode ( int val, bplusNode *n, int *pos,int firstOrLast ) { 
	if ( val < n -> value[1] ) { 
		*pos = 0 ;
		return 0 ;
	} else if( val > n-> value[n->count]) {
		*pos = n->count;
		return 0;
	}
	else { 
		if(firstOrLast==0) {
			return binarySearchFirst(val,n,pos,1,n->count);
		} else {
			return binarySearchLast(val,n,pos,1,n->count);
		}
	}
}
int bplusTree::binarySearchFirst(int val,bplusNode *someNode, int *pos,int begin,int end) {
	int retVal = 0;
	if(val == someNode->value[begin] ) {
		//Check for boundary points
		*pos=begin;
		retVal=1;
	} else 	if(end - begin==1) {
		//base case for recursion to terminate
		if(val == someNode->value[end]) {
		//Check for boundary points
			*pos=end;
			retVal=1;
		} else {
			*pos=begin;
			retVal=0;
		}
	} else {
		//recursive case
		int mid= (begin+end)/2;
		if(val <= someNode->value[mid]) retVal=binarySearchFirst(val,someNode,pos,begin,mid) ;
		else retVal=binarySearchFirst(val,someNode,pos,mid,end);
	}
	return retVal;
}
int bplusTree::binarySearchLast(int val,bplusNode *someNode, int *pos,int begin,int end) {
	int retVal = 0;
	if(val == someNode->value[end] ) {
		//Check for boundary points
		*pos=end;
		retVal=1;
	} else 	if(end - begin==1) {
		//base case for recursion to terminate
		if(val == someNode->value[begin]) {
		//Check for boundary points
			*pos=begin;
			retVal=1;
		} else {
			*pos=begin;
			retVal=0;
		}
	} else {
		//recursive case
		int mid= (begin+end)/2;
		if(val < someNode->value[mid]) retVal=binarySearchLast(val,someNode,pos,begin,mid) ;
		else retVal=binarySearchLast(val,someNode,pos,mid,end);
	}
	return retVal;
}

int bplusTree :: searchVector ( int val, vector<int> allValues, int *pos,int firstOrLast ) { 
	if ( val < allValues[1] ) { 
		*pos = 0 ;
		return 0 ;
	} else if( val > allValues[allValues.size()-1]) {
		*pos = allValues.size()-1;
		return 0;
	}
	else { 
		if(firstOrLast==0) {
			return binarySearchVectorFirst(val,allValues,pos,1,allValues.size()-1);
		} else {
			return binarySearchVectorLast(val,allValues,pos,1,allValues.size()-1);
		}
	}
}
int bplusTree::binarySearchVectorFirst(int val,vector<int> allValues, int *pos,int begin,int end) {
	int retVal = 0;
	if(val == allValues[begin] ) {
		//Check for boundary points
		*pos=begin;
		retVal=1;
	} else 	if(end - begin==1) {
		if(val == allValues[end]) {
			//Check for boundary points
			*pos=end;
			retVal=1;
		} else { 
		//base case for recursion to terminate
			*pos=begin;
			retVal=0;
		}
	} else {
		//recursive case
		int mid= (begin+end)/2;
		if(val <= allValues[mid]) retVal=binarySearchVectorFirst(val,allValues,pos,begin,mid) ;
		else retVal=binarySearchVectorFirst(val,allValues,pos,mid,end);
	}
	return retVal;
}
int bplusTree::binarySearchVectorLast(int val,vector<int> allValues, int *pos,int begin,int end) {
	int retVal = 0;
	if(val == allValues[end] ) {
		//Check for boundary points
		*pos=end;
		retVal=1;
	} else 	if(end - begin==1) {
		if(val == allValues[begin]) {
			//Check for boundary points
			*pos=begin;
			retVal=1;
		} else { 
		//base case for recursion to terminate
			*pos=begin;
			retVal=0;
		}
	} else {
		//recursive case
		int mid= (begin+end)/2;
		if(val < allValues[mid]) retVal=binarySearchVectorLast(val,allValues,pos,begin,mid) ;
		else retVal=binarySearchVectorLast(val,allValues,pos,mid,end);
	}
	return retVal;
}
/*
int bplusTree :: searchnode ( int val, bplusNode *n, int *pos ) { 
	if ( val < n -> value[1] ) { 
		*pos = 0 ;
		return 0 ;
	}
	else { 
		*pos = n -> count ;
		while ( ( val < n -> value[*pos] ) && *pos > 1 ) ( *pos )-- ;
		if ( val == n -> value[*pos] ) return 1 ;
		else return 0 ;
	}
}
*/
void bplusTree :: show( ) { 
	display ( root,0 ) ;
	cout << endl;
}
void bplusTree :: display ( bplusNode *root , int level ) { 
	if ( root != NULL ) { 
		for(int j=0;j < level; j++) cout << "\t" ;
		cout << "Level "<< level ;
		if(root->isLeaf) cout << " Is a leaf. num keys=" << root-> count << endl;
		else cout << " Is Not a Leaf. num keys= " << root->count << endl;
		int i;
		for ( i = 0 ;i < root->count ;i++ ) { 
			display ( root->child[i],level+1 ) ;
			for(int j=0;j < level; j++) cout << "\t" ;
			cout << "Level "<< level << " : " ;
			cout << "Key Number " << i+1 <<": key=" << root->value[i+1] <<" recordId=" << root->recordId[i+1] <<  "\t" << endl;
		}
		display ( root->child[i] ,level+1) ;
	}
}
void bplusTree :: deltree ( bplusNode *root ) { 
	if ( root != NULL ) { 
		int i;
		for ( i = 0 ;i < root -> count ;i++ ) { 
			deltree ( root -> child[i] ) ;
			delete ( root -> child[i] ) ;
		}
		deltree ( root -> child[i] ) ;
		delete ( root -> child[i] ) ;
	}
}
bplusTree :: ~bplusTree( ) { 
	deltree ( root ) ;
}

void bplusTree::writeToXML(string indexFolderPath) {
	int nodeCount=-1;
        string emptySpace="";
	indexFolderPath= indexFolderPath+"/bptIndexes";
	system(("rm -rf "+indexFolderPath).c_str());
        system(("mkdir "+indexFolderPath).c_str());
        writeToXML(root,indexFolderPath,nodeCount);
        cout << "Total Number of nodes Written " << 1+nodeCount << endl;
}

int bplusTree::writeToXML(bplusNode *someNode, string indexFolderPath,int& nodeCount) {
	int nodeId=-1;
        if(someNode != NULL) {
                nodeCount=nodeCount+1;
		nodeId = nodeCount;
                // Write the opening tag entry for the given node in the xml file
		string xmlFileName= "root.xml";
		if(nodeCount!=0) 
		{
			char buffer[100];
			sprintf(buffer,"%i.xml",nodeCount);
			xmlFileName= string(buffer);
		}
		string xmlFilePath= indexFolderPath+"/"+xmlFileName;
		::remove(xmlFilePath.c_str());
		ofstream xmlFile(xmlFilePath.c_str(), ios::out | ios::binary);
		xmlFile << "<?xml version=\"1.0\"?>" << endl;
                xmlFile << "<node numKeys=" << someNode->count << " isLeaf=" << someNode->isLeaf << " >" << endl;
                // write down all the child nodes of the given node.
		for(int i=0; i <= someNode->count; i++ ) {
			string childNodeFileName="NULL";
			if(!someNode->isLeaf) {
				int childNodeId=writeToXML(someNode->child[i],indexFolderPath,nodeCount);
				char buff[100];
				sprintf(buff,"%i.xml",childNodeId);
				childNodeFileName=string(buff);
			}
			if(i==0) xmlFile << "  <childnode childNodeFileName=" << childNodeFileName << " keyvalue=NULL recordId=NULL />" << endl;
		 	else xmlFile << "  <childnode childNodeFileName=" << childNodeFileName << " keyvalue="<<someNode->value[i] << " recordId=" << someNode->recordId[i] << " />" << endl;
		}
                // Write the closing tag entry for the given node in the xml file
                xmlFile << "</node>" << endl;
		xmlFile.close();
        }
	return nodeId;
}
void bplusTree::rangeQuery(int minValue,int maxValue,vector<int> & result, bplusNode * node){
	if(node != NULL) {
		if (node->isLeaf) {
			for( int i=1;i <= node->count ; i++ ) {
				if( minValue <= node->value[i] && maxValue >= node->value[i]) 
					result.push_back(node->recordId[i]);
			}
		}else {
			int begin=0,end=0;
			if( minValue <= node->value[1]) begin=0; 
			else {
				int ret=searchnode(minValue,node,&begin,0);
				if(ret==1) begin=begin-1; // the case where min value is not one of keys
			}
			if( maxValue >= node->value[node->count]) end=node->count;
			else {
				searchnode(maxValue,node,&end,1);
				end=end+1;
			}
			for( int i=begin;i <= end ; i++ ) {
				rangeQuery(minValue,maxValue,result,node->child[i]);
			}
		}
	}
}
/*
void bplusTree::rangeQuery(int minValue,int maxValue,vector<int> & result, bplusNode * node){
	if(node != NULL) {
		if (node->isLeaf) {
			for( int i=1;i <= node->count ; i++ ) {
				if( minValue <= node->value[i] && maxValue >= node->value[i]) 
					result.push_back(node->recordId[i]);
			}
		}else {
			for( int i=0;i <= node->count ; i++ ) {
				bool recurse=false;
				if(i==0) {if( minValue <= node->value[i+1]) recurse=true;}
				else if(i==node->count) {if( maxValue >= node->value[i]) recurse=true;}
				else if(minValue <= node->value[i+1] && maxValue>=node->value[i]) recurse=true;
				if(recurse) rangeQuery(minValue,maxValue,result,node->child[i]);
			}
		}
	}
}
*/
vector<int> bplusTree::rangeQuery(int minValue,int maxValue){
	vector<int> result;
	rangeQuery(minValue,maxValue,result,root);
	return result;
}
void bplusTree::rangeQueryExternalMemory(int minValue,int maxValue,vector<int> & result, string nodeFileName,string indexFolderPath,int& nodeCount){
	nodeCount=nodeCount+1;
	string nodeFilePath= indexFolderPath+"/bptIndexes/"+nodeFileName;
        vector<int> value;
        vector<int> recordId;
	vector<string> childNodeFileNames;
	bool isLeaf=false;
	int numKeys=0;
        ifstream inf;
        inf.open(nodeFilePath.c_str());
        // While there's still stuff left to read
        string line;
        getline(inf,line); //This gets the  xml header line of the file. Subsequent calls will fetch the node data
        string openNodePattern="<node";
        string closeNodePattern="</node";
	string childNodePattern="<childnode";
        while (getline(inf,line))
        {
                size_t foundOpen= line.find(openNodePattern);
                size_t foundClose= line.find(closeNodePattern);
                size_t foundChild= line.find(childNodePattern);
		if(foundOpen != string::npos) {
			string cleanOfWhiteSpace= trim(line);
                	vector<string> attributeValuePairs = stringsplit(cleanOfWhiteSpace,' ');
                	numKeys=atoi(((stringsplit(attributeValuePairs[1],'='))[1]).c_str());
                	isLeaf=(1==atoi(((stringsplit(attributeValuePairs[2],'='))[1]).c_str()));
		} else if(foundChild != string::npos){
			string cleanOfWhiteSpace= trim(line);
                	vector<string> attributeValuePairs = stringsplit(cleanOfWhiteSpace,' ');
			childNodeFileNames.push_back((stringsplit(attributeValuePairs[1],'='))[1]);
			string val=(stringsplit(attributeValuePairs[2],'='))[1];
			if(val=="NULL") {
				value.push_back(INTMIN);
				recordId.push_back(INTMIN);
			}
			else {
				value.push_back(atoi(val.c_str()));
				recordId.push_back(atoi(((stringsplit(attributeValuePairs[3],'='))[1]).c_str()));
			}
		} 
		else if(foundClose != string::npos) break; 
	}
	if (isLeaf) {
		for( int i=1;i <= numKeys ; i++ ) {
			if( minValue <= value[i] && maxValue >= value[i])
				result.push_back(recordId[i]);
		}
	}else {
		int begin=0,end=0;
		if( minValue <= value[1]) begin=0; 
		else {
			int ret=searchVector(minValue,value,&begin,0);
			if(ret==1) begin=begin-1; // the case where min value is not one of keys
		}
		if( maxValue >= value[numKeys]) end=numKeys;
		else {
			searchVector(maxValue,value,&end,1);
			end=end+1;
		}
		for( int i=begin;i <= end ; i++ ) {
			rangeQueryExternalMemory(minValue,maxValue,result,childNodeFileNames[i],indexFolderPath,nodeCount);
		}
	}

}
/*
void bplusTree::rangeQueryExternalMemory(int minValue,int maxValue,vector<int> & result, string nodeFileName,string indexFolderPath,int& nodeCount){
	nodeCount=nodeCount+1;
	string nodeFilePath= indexFolderPath+"/"+nodeFileName;
        vector<string> value;
:0
        vector<string> recordId;
	vector<string> childNodeFileNames;
	bool isLeaf=false;
	int numKeys=0;
        ifstream inf;
        inf.open(nodeFilePath.c_str());
        // While there's still stuff left to read
        string line;
        getline(inf,line); //This gets the  xml header line of the file. Subsequent calls will fetch the node data
        string openNodePattern="<node";
        string closeNodePattern="</node";
	string childNodePattern="<childnode";
        while (getline(inf,line))
        {
                size_t foundOpen= line.find(openNodePattern);
                size_t foundClose= line.find(closeNodePattern);
                size_t foundChild= line.find(childNodePattern);
		if(foundOpen != string::npos) {
			string cleanOfWhiteSpace= trim(line);
                	vector<string> attributeValuePairs = stringsplit(cleanOfWhiteSpace,' ');
                	numKeys=atoi(((stringsplit(attributeValuePairs[1],'='))[1]).c_str());
                	isLeaf=(1==atoi(((stringsplit(attributeValuePairs[2],'='))[1]).c_str()));
		} else if(foundChild != string::npos){
			string cleanOfWhiteSpace= trim(line);
                	vector<string> attributeValuePairs = stringsplit(cleanOfWhiteSpace,' ');
			childNodeFileNames.push_back((stringsplit(attributeValuePairs[1],'='))[1]);
			value.push_back((stringsplit(attributeValuePairs[2],'='))[1]);
			recordId.push_back((stringsplit(attributeValuePairs[3],'='))[1]);
		} 
		else if(foundClose != string::npos) break; 
	}
	if (isLeaf) {
		for( int i=1;i <= numKeys ; i++ ) {
			if( minValue <= atoi(value[i].c_str()) && maxValue >= atoi(value[i].c_str()))
				result.push_back(atoi(recordId[i].c_str()));
		}
	}else {
		for( int i=0;i <= numKeys ; i++ ) {
			bool recurse=false;
			if(i==0) {if( minValue <= atoi(value[i+1].c_str())) recurse=true;}
			else if(i==numKeys) {if( maxValue >= atoi(value[i].c_str())) recurse=true;}
			else if(minValue <= atoi(value[i+1].c_str()) && maxValue>=atoi(value[i].c_str())) recurse=true;
			if(recurse) rangeQueryExternalMemory(minValue,maxValue,result,childNodeFileNames[i],indexFolderPath,nodeCount);
		}
	}

}
*/
vector<int> bplusTree::rangeQueryExternalMemory(int minValue,int maxValue,string indexFolderPath, int& nodeCount){
	vector<int> result;
	rangeQueryExternalMemory(minValue,maxValue,result,"root.xml",indexFolderPath,nodeCount);
	return result;
}



int main(int argc,char* argv[]) {
        bplusTree bTreeIndex ;
        vector<int> arr;
        int i=0, rangeStart,rangeEnd;
        string dataPath= string(argv[1]);
        string dataFilePath=dataPath+"/dataFile.csv";
        int columnNum=atoi(argv[2]);
        string indexFolderPath= string(argv[3]);
        ifstream inf;
        inf.open(dataFilePath.c_str());
        // While there's still stuff left to read
        string line;
        getline(inf,line); //This gets the header line of the file. Subsequent calls will fetch the records
        while (getline(inf,line))
        {
                string recordData=trim(line);
                vector<string> recordElems=split(recordData,',');
                string primary = recordElems[0];
                int primaryIndex = atoi(primary.c_str());
                string col = recordElems[columnNum];
                int key = atoi(col.c_str());
                arr.push_back(key);
                bTreeIndex.insert (key ,primaryIndex) ;
                i++;
        }
//      cout << "B-tree of order 5:" << endl;           b.show( );
        bTreeIndex.writeToXML(indexFolderPath);
        cout<<"Enter the rangeStart & rangeEnd numbers :"<<endl;
	// cin>>rangeStart>>rangeEnd;
        cout << "Range Query (" << rangeStart<<" - "<<rangeEnd<<")"<< endl;
        int nodeCount=0;
        vector<int> resultExternal;
        resultExternal = bTreeIndex.rangeQueryExternalMemory(rangeStart,rangeEnd,indexFolderPath,nodeCount);
//      for(int i=0; i< resultExternal.size();i++)  cout<<resultExternal[i]<< "  ";
//        cout<<endl;        cout<<"Number of values retreived in the query= "<<resultExternal.size()<<endl;        cout << "Number of Nodes Accessed = " << nodeCount << endl;

        double totalTime=0;
        int j=0,samples;
//        cout<<"Enter the number of times query must be run :"<<endl;
       // cin>>samples;   cout << "Range Query (" << rangeStart<<" - "<<rangeEnd<<")"<< endl;
         clock_t start;

//      while((clock()-start)<1000000)
      // for(j=0;j<samples;j++){
//        j++;
        //bTreeIndex.rangeQueryExternalMemory(rangeStart,rangeEnd,indexFolderPath,nodeCount);
      //  bTreeIndex.rangeQuery(rangeStart,rangeEnd);         }

//        totalTime=(double(clock()-start)*1000.0/double(CLOCKS_PER_SEC));
  //      cout<<"Avg. time taken for query response in "<<j<<" samples "<<totalTime/samples<<endl;
}




