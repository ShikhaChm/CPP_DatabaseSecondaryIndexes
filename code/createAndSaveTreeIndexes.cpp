#include<iostream>
#include<time.h>
#include<stdlib.h>
#include<math.h>
#include<vector>
#include "binaryTrees.h"
#include "generateIndexes.h"
using namespace std;
BTree createType2Index(string dataPath,int column,int branchingFactor,int tType) {
	map<string,int> enumMap;
	map<int,int> cardinalityMap;
	map<int,EWAHBoolArray<int> > indexMap=generateIndex(dataPath,column,enumMap,cardinalityMap);
	map<int,string> reverseEnumMap;
	for (map<string, int>::iterator i = enumMap.begin(); i != enumMap.end(); ++i) reverseEnumMap[i->second] = i->first;
	int cnt=0;
	int treeType=tType;
	BTree treeIndex(indexMap,enumMap,cardinalityMap,branchingFactor,treeType);
	return treeIndex;
}

void createAndSaveType2Indexes(string dataPath,int* columnList,int numCols,int branchingFactor,int tType) {
	for(int i=0;i<numCols;i++) {
		int indexNum = columnList[i];
		int treeType=tType;
		string indexFolderPath;
		BTree index = createType2Index(dataPath,indexNum,branchingFactor,treeType);
		char buff[100];
		sprintf(buff,"index%i",indexNum);
		if(treeType==1){
			indexFolderPath=dataPath+"/indexType1/"+buff;
			system(("rm -rf "+indexFolderPath).c_str());
			system(("mkdir -p "+indexFolderPath).c_str());
			index.writeToXML(indexFolderPath);}
		else if(treeType==2){
			indexFolderPath=dataPath+"/indexType2/"+buff;
			system(("rm -rf "+indexFolderPath).c_str());
			system(("mkdir -p "+indexFolderPath).c_str());
			index.writeToXML(indexFolderPath);}
		else{
			indexFolderPath=dataPath+"/indexType3/"+buff;
			system(("rm -rf "+indexFolderPath).c_str());
			system(("mkdir -p "+indexFolderPath).c_str());
			index.writeToXML(indexFolderPath);}

	}
}

int main(int argc,char* argv[]) {
	string dataPath = string(argv[1]);
	int treeType= atoi(argv[2]);
	int columnList[]={5,6,7,8}; // creates indexes on number 5,6,7,8 which are numeric columns.We can give more columns but the coulmns must contain only numeric values
	int numCols = (sizeof(columnList)/sizeof(int));
	createAndSaveType2Indexes(dataPath,columnList,numCols,128,treeType); // 128 here is the branching factor , we can change it to other value. Preferably take the number in the power of 2.
	return 0;
}
