#include<iostream>
#include<time.h>
#include<stdlib.h>
#include<math.h>
#include<vector>
#include "binaryTree.h"
#include "generateIndexes.h"
using namespace std;
btree createType1Index(string dataPath,int column) {
	btree treeIndex;
	map<string,int> enumMap;
        map<int,int> cardinalityMap;
        map<int,EWAHBoolArray<int> > indexMap=generateIndex(dataPath,column,enumMap,cardinalityMap);
	map<int,string> reverseEnumMap;
	for (map<string, int>::iterator i = enumMap.begin(); i != enumMap.end(); ++i) reverseEnumMap[i->second] = i->first;
	int cnt=0;
	for( map<int,EWAHBoolArray<int> >::iterator ii=indexMap.begin(); ii!=indexMap.end(); ++ii)
        {
		int key=(*ii).first;
		int key_value=atoi(reverseEnumMap[key].c_str());
		int cardinality=cardinalityMap[key];
		EWAHBoolArray<int> bitmap=(*ii).second;
		treeIndex.insert(key_value,cardinality,bitmap);
		cnt+=1; 
	}
	return treeIndex;
}
//vector<int> simpleRangeQuery(int min, int max, btree treeIndex) {
//	
//}
int main(int argc,char* argv[]) {
	string dataPath=argv[1];
	btree type1Index = createType1Index(dataPath,6); // here 6 is the column value , you may pass any column number which has numeric values ex: salary,age etc
	type1Index.inOrder();
	EWAHBoolArray<int> result=type1Index.rangeQuery(100000,150000); // This is the range, please change it according to the values in the column
	vector<int> resultVector;
	result.appendSetBits(resultVector);
	for(int i=0;i < resultVector.size();i++) {
		cout << resultVector[i] << endl;
	}
	return 0;
}
