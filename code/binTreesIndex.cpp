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


int main(int argc,char* argv[]) {
	string dataPath=argv[1];
	int num=0;
	string dataGenPath= "./dataGen";
	int treeType= atoi(argv[2]);
	BTree type2Index = createType2Index(dataPath,6,4,treeType);
	type2Index.printTree();
	//system("sync ; echo 3 | sudo tee /proc/sys/vm/drop_caches >/dev/null");
	clock_t start;
	double totalTime=0;
	//        start = clock();
	int i=0;
	EWAHBoolArray<int> result=type2Index.rangeQuery(200000,200000);
	cout<<"\n The current time is: "<<currentTime()<<endl;
/*	while((clock()-start) < 1000000) {
		i++;
		EWAHBoolArray<int> result=type2Index.rangeQuery(150000,200000);
	}
	totalTime=(double(clock() - start)*1000.0/(double)CLOCKS_PER_SEC);
	cout<<"Process id = "<<getpid()<<endl;
	//cout << " \nAvg time Over " << i << " samples = " << (totalTime/i) <<"MS"<< endl;

 	vector<int> resultVector;
	EWAHBoolArray<int> result=type2Index.rangeQuery(100000,150000);
	result.appendSetBits(resultVector);*/
	return 0;
}
