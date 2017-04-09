/*In our 'dataGen.cpp' file we create a database file with 9 columns.The schema of table is  <member_id, sex,ethnicity,education,industry,familySize, salary,age,netWorth ,location>.  In this file the function createAndSaveIndexes creates indexes on the file columns for the file path passed in the command line argument while execution. 
As per the cardinality of each column files will be created. For example cardinality of 'Sex' filed is 2 (male and female) so the following file'll create 1.bin and 2.bin files which are the bit vectors for male and female respectively.A map file is also saved so you could check which .bin file represents which value.
*/

#include <stdio.h>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "../EWAHBoolArray/headers/ewah.h"
#include "../EWAHBoolArray/headers/boolarray.h"
#include "generateIndexes.h"

using namespace std;

void createAndSaveIndexes(string dataPath,int* columnList,int numCols) {
	for(int i=0;i<numCols;i++) {
		int indexNum=columnList[i];
		//Now generate Index;
		map<string,int> enumMap;
		map<int,int> cardinalityMap;
	        map<int,EWAHBoolArray<int> > indexMap=generateIndex(dataPath,indexNum,enumMap,cardinalityMap);
		//Now save the index on the filesystem;
		char indexName[100];
		sprintf(indexName,"%s%i","indexColumn",indexNum);
		string indexFolderPath= dataPath+"/indexes/"+indexName;
		system(("mkdir -p "+indexFolderPath).c_str());
		string indexMapPath=indexFolderPath+"/map";
		//Save the index
		for( map<int,EWAHBoolArray<int> >::iterator ii=indexMap.begin(); ii!=indexMap.end(); ++ii)
		{
			char binFileName[100];
			sprintf(binFileName,"%i.bin",(*ii).first);
			
			string indexFilePath=indexFolderPath+"/"+ binFileName;
			EWAHBoolArray<int> index=(*ii).second; 
//			cout<<" index "<<(*ii).first <<endl; 
//			cout<<" index "<<*(ii).second<<endl; 
			::remove(indexFilePath.c_str());
			ofstream out(indexFilePath.c_str(), ios::out | ios::binary);
			index.write(out);
			out.close();
			//Check 
			EWAHBoolArray<int> yawn;
        		ifstream in1(indexFilePath.c_str(), ios::binary);
        		yawn.read(in1);
        		in1.close();
		}

		//Save the Map
		ofstream mapFile;
		mapFile.open(indexMapPath.c_str());
		for( map<string, int>::iterator ii=enumMap.begin(); ii!=enumMap.end(); ++ii)
		{
			mapFile << (*ii).second << "," << (*ii).first << endl;
		}
		mapFile.close();
	
	}	
}

int main(int argc, char* argv[]){
	string dataPath = string(argv[1]);
	int columnList[]={1,2,3,4,5,6,7,8,9};
	//int columnList[]={3};
	//int columnList[]={2};
	int numCols = (sizeof(columnList)/sizeof(int));
	createAndSaveIndexes(dataPath,columnList,numCols);	
	return 0;
}
