#include "net.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
void transform(const char *origin,int add[4]){
	int newadd[5];
	long lastlength=0;
	long length;char newint[4];
	for (int i = 0; i < 4; ++i)
	{
		length=strchr(origin,'.')-origin-lastlength;
		if (length>3&&length==0){
			throw std::out_of_range("out of range");
		}
		strncpy(newint, origin+lastlength, length);
		printf("length is %ld,last length is %ld\n", length,lastlength);
		std::cout<<newint<<std::endl;
		newadd[i]=std::atoi(newint);
		lastlength=length;
		memset(newint, 0, length);
	}
		
}
void make_socket(const char *add){
	struct sockaddr_in *sck;
	char abuf[INET_ADDRSTRLEN];
	
}

int main(int argc, char const *argv[])
{
	int arr[4];
	transform("127.0.0.1", arr);
	for(size_t i=0;i<4;i++)std::cout<<arr[i]<<".";
	std::cout<<std::endl;
	return 0;
}