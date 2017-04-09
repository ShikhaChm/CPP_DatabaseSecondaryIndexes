#include<iostream>
#include<time.h>
#include<stdlib.h>
#include<math.h>
#include<vector>
#include "binaryTrees.h"
#include "generateIndexes.h"
using namespace std;

int main(int argc,char* argv[]) {
	string dataPath=argv[1];
	int treeType=atoi(argv[2]);
	int num,column=5 ,numDiskReads=0;
	int i=0, rangeVal1=0,rangeVal2=0;	string indexFolderPath;
	BTree type2Index;
	char buff[100];
	sprintf(buff,"index%i",column);
	if (treeType==1){
		indexFolderPath=dataPath+"/indexType1/"+buff;
		type2Index.readFromXML(indexFolderPath);}
	else if (treeType==2){
		indexFolderPath=dataPath+"/indexType2/"+buff;
		type2Index.readFromXML(indexFolderPath);}
	else {
		indexFolderPath=dataPath+"/indexType3/"+buff;
		type2Index.readFromXML(indexFolderPath);}
	//type2Index.printTree(); 

	if(treeType==1)		cout<<"Tree Type 1"<<endl;
	else if (treeType==2)   cout<<"Optimal Tree Structure"<<endl;
	else   			cout<<"Sub-Optimal Tree Structure"<<endl;
	//To empty the cache
	//system("sync ; echo 3 | sudo tee /proc/sys/vm/drop_caches >/dev/null");
	EWAHBoolArray<int> result;
	double totalTime=0;clock_t start;

	cout<<"Enter the range values for rangeQuery :"<<endl;  	
	cin>>rangeVal1>>rangeVal2;

	while((clock()-start)<1000000){
		i++;
		type2Index.rangeQueryExternalMemory(rangeVal1,rangeVal2,indexFolderPath,numDiskReads);
	}
	totalTime=(double(clock()-start)*1000.0/double(CLOCKS_PER_SEC));
	result=type2Index.rangeQueryExternalMemory(rangeVal1,rangeVal2,indexFolderPath,numDiskReads);
	vector<int> resultVector;

	result.appendSetBits(resultVector);
	cout<<"Number of Nodes Accessed = "<<numDiskReads/i<<endl; 
	cout<<"Number of values retreived in the query= "<<resultVector.size()<<endl;	

	cout<<"Result of range query (Row ids) : \n";
	//	for(int i=0;i < resultVector.size();i++)  cout << resultVector[i]<<" ";	cout<<endl;
	cout<<"Avg. time taken for query response "<<"in "<<i<<" samples= "<<totalTime/i<<endl; 
	return 0;
}
