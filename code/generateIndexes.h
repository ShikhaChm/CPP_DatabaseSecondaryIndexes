#include <stdio.h>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include<fstream>
#include "../EWAHBoolArray/headers/ewah.h"
#include "../EWAHBoolArray/headers/boolarray.h"
#include "utils.h"

using namespace std;

map<int,EWAHBoolArray<int> > generateIndex(string dataPath,int column,map<string,int>& enumMap,map<int,int>& cardinalityMap)  {
        map<int,EWAHBoolArray<int> > indexMap;
	enumMap.clear();
	cardinalityMap.clear();
        int cardinality=0;
//        string val = recordElems[column];
        string dataFilePath=dataPath+"/dataFile.csv";
        ifstream inf;
        inf.open(dataFilePath.c_str());

        // While there's still stuff left to read
        string line;
        getline(inf,line); //This gets the header line of the file. Subsequent calls will fetch the records

        while (getline(inf,line))
        {
                string recordData=trim(line);
                vector<string> recordElems=split(recordData,',');
                string val = recordElems[column];
                string primary = recordElems[0];
                int primaryIndex = atoi(primary.c_str());
                if ( enumMap.find( val) == enumMap.end() ) {
                        // not found
                        cardinality=cardinality+1;
                        enumMap[val]=cardinality;
                        EWAHBoolArray<int> newBoolArray;
                        indexMap[cardinality]= newBoolArray;
			cardinalityMap[cardinality]=0;
                }
                indexMap[enumMap[val]].set(primaryIndex);
//		cout<<"indexMap[enumMap[Val] "<<primaryIndex<<endl;
		cardinalityMap[enumMap[val]]=cardinalityMap[enumMap[val]]+1;
        }
        return indexMap;

}

