#include<iostream>
#include<stdlib.h>
using namespace std;

int main(){
	cout<<"Executing ./createAndSaveTreeIndexes for Tree 2 & 3, ./readTreeIndexes, ./bplusTree"<<endl;
	cout<<" Optimal Tree: "<<endl;
	//create Tree Indexes by running the following commands
	system("./createAndSaveTreeIndexes ../data/dataset500k/ 2");
	//Read from the saved Tree Indexes by running the following commands
//	system("strace -e trace=read -o strace-OptTree.txt ./readTreeIndexes ../data/dataset500k/ 2");

	cout<<"\n Sub-Optimal Tree: "<<endl;
	//create Tree Indexes by running the following commands
	system("./createAndSaveTreeIndexes ../data/dataset500k/ 3");
	//Read from the saved Tree Indexes by running the following commands
//	system("strace -e trace=read -o strace-SubOptTree.txt ./readTreeIndexes ../data/dataset500k/ 3");
	cout<<"\n BPlusTree: "<<endl;
	
	system("strace -e trace=read -o strace-bpt.txt ./bplusTree ../data/dataset500k/ 7 ../data/dataset500k ");

}
