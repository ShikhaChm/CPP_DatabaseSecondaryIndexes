#include<iostream>
#include<stdlib.h>
using namespace std;
int main(){
	//	system("grep -A1 'bin|xml' out1.txt  | grep read | awk -F'=' 'BEGIN{sum=0}{sum=sum+$NF}END{print sum}'");
	//	For summing only xml read
	//cout<<"Number of bytes read ('xml' & 'bit vectors')by TreeType First = "<<endl;
	//system("grep -A1 'xml' out1.txt  | grep read | awk -F'=' 'BEGIN{sum=0}{sum=sum+$NF}END{print sum}'");
	//	For summing only bin reads
	//system("grep -A1 'bin' out1.txt  | grep read | awk -F'=' 'BEGIN{sum=0}{sum=sum+$NF}END{print sum}'");

	//	system("grep -A1 'xml' out2.txt  | grep read | awk -F'=' 'BEGIN{sum=0}{sum=sum+$NF}END{print sum}'");
	//	system("grep -A1 'bin' out2.txt  | grep read | awk -F'=' 'BEGIN{sum=0}{sum=sum+1+($NF/4096)}END{print sum}'");
	cout<<"\n (Optimal Tree) Bytes read & #Blocks read       "<<endl;
	system("grep read strace-OptTree.txt  | awk -F'=' 'BEGIN{sum=0}{sum=sum+$NF}END{print sum}'");
	system("grep read strace-OptTree.txt  | awk -F'=' 'BEGIN{sum=0}{sum=sum+1+($NF/4096)}END{print sum}'");
	
	cout<<"\n (Sub-Optimal Tree) Bytes read  & #Blocks read     "<<endl;
	system("grep read strace-SubOptTree.txt  | awk -F'=' 'BEGIN{sum=0}{sum=sum+$NF}END{print sum}'");
	system("grep read strace-SubOptTree.txt  | awk -F'=' 'BEGIN{sum=0}{sum=sum+1+($NF/4096)}END{print sum}'");

	cout<<"\n (bPlusTree)  Bytes read   & #Blocks read   "<<endl;
	system("grep read strace-bpt.txt  | awk -F'=' 'BEGIN{sum=0}{sum=sum+$NF}END{print sum}'");
	system("grep read strace-bpt.txt  | awk -F'=' 'BEGIN{sum=0}{sum=sum+1+($NF/4096)}END{print  sum}'");

//	cout<<"Number of disk reads ( bPlusTree ) = "<<endl; 	system("grep -c read strace-bpt.txt ");
	//	For printing the size of each read
	//system("grep -A1 'bin\'|'xml' out.txt  | grep read | awk -F'=' '{print $NF}'");
	return 0;
}


