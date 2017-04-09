#ifndef UTILS
#define UTILS
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include<time.h>
#include<math.h>
# include <map>
#include <vector>
#include "../EWAHBoolArray/headers/ewah.h"
#include "../EWAHBoolArray/headers/boolarray.h"

using namespace std;

string trim(const string &str0)
{
    string str = str0;
    size_t at2 = str.find_last_not_of(" \t\r\n\0\a\b\f\v");
    size_t at1 = str.find_first_not_of(" \t\r\n\0\a\b\f\v");
    if (at2 != string::npos) str.erase(at2+1);
    if (at1 != string::npos) str.erase(0,at1);
    return str;
}
vector<string> split(const string &s, char delim) {
    vector<string> elems;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
int getRand(int min,int max) {
        int randomNumber = rand();
        double scaled = (double)randomNumber/(double)RAND_MAX ;
        double rescaled = scaled*(max - min);
        int random = min +round(rescaled);
        //cout << "RandMax=" << RAND_MAX << "  Random Number=" << randomNumber<< " scaled=" << scaled<< " rescaled=" << rescaled << " random=" << random<<endl;
        return random;
        //return (min + round((((double)rand())/(double)RAND_MAX)*(max-min)));
}

map<string,int> loadMapFile(string mapFilePath) {
        map<string,int> mapFileData;
        ifstream inf;
        inf.open(mapFilePath.c_str());
        // While there's still stuff left to read
        string line;
        while (getline(inf,line))
        {
                // read stuff from the file into a string and print it
                vector<string> elems= split(line,',');
                string id = elems[0];
                string value=elems[1];
                int valueId = atoi(id.c_str());
                mapFileData[value]=valueId;
        }
        return mapFileData;
}
#endif /*!FILE_FOO_SEEN */
