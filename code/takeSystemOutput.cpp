#include <iostream>
#include <stdio.h>
#include<stdlib.h>
using namespace std;

extern string execute(char* cmd) {
	FILE* pipe = popen(cmd, "r");
	if (!pipe) return "ERROR";
	char buffer[256];
	std::string result = " ";
	while(!feof(pipe)) {
		if(fgets(buffer, 128, pipe) != NULL)
			result += buffer;
	}
	pclose(pipe);
	return result;
}

int main(){
char cmd[]="grep -A1 'xml' out1.txt  | grep read | awk -F'=' 'BEGIN{sum=0}{sum=sum+$NF}END{print sum}'";
string result=execute(cmd);
int numOfBytes= atoi(result.c_str());	
cout<<"Number of bytes = "<<numOfBytes<<endl;
	return 0;
}
