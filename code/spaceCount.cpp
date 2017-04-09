#include<iostream>
#include<stdlib.h>
using namespace std;
int main(){
	cout<<"\n (Optimal Tree) space occupied  "<<endl;
	system("wc -c ../data/dataset500k/indexType2/index7/*.bin |  awk '{s+=$1} END {print s/1024}' ");
	cout<<"\n (Sub-Optimal Tree) space occupied  "<<endl;
	system("wc -c ../data/dataset500k/indexType3/index7/*.bin |  awk '{s+=$1} END {print s/1024}' ");
	cout<<"\n (BPlus Tree) space occupied  "<<endl;
	system("wc -l ../data/dataset500k/bptIndexes/*.xml |  awk '{s+=$1-3} END {print s}' ");
	return 0;
}


