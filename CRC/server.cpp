#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include<ctype.h>
#include <fstream>
#include<iomanip>
using namespace std;
int client;
int main(int argc,char *argv[]) 
{
	int server; 
	//cout<<argc;
	if(argc<3)
		return 0;
	int portNum = atoi(argv[2]) ;
	int bufSize = 1024; 
	
	char result[bufSize],buffer[1024],message[1024],c[30]; 
	struct sockaddr_in server_addr;
	socklen_t size;
	cout << "\n- Starting server..." << endl;
	server = socket(AF_INET, SOCK_STREAM, 0);
	if (server < 0) {
		cout << "Error establishing socket ..." << endl;
		exit(-1);
	}
	cout << "- Socket server has been created..." << endl;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_family = AF_INET;
	//server_addr.sin_addr.s_addr = htons(INADDR_ANY);
	server_addr.sin_port = htons(portNum);
	if ((bind(server, (struct sockaddr*) &server_addr, sizeof(server_addr)))< 0) 
	{
		cout << "- Error binding connection, the socket has already been established..." << endl;
		exit(-1);
	}
		size = sizeof(server_addr);
	cout << "- Looking for clients..." << endl;
	listen(server, 10);
	client = accept(server, (struct sockaddr *) &server_addr, &size);
	if (client < 0)
		cout << "- Error on accepting..." << endl;
	recv(client,c,30,0);
	int m=strlen(c);
	cout<<"\n crc function : "<<c;
	int i,flag=0;
	while(true)
	{
		recv(client,buffer,1024,0);
		cout<<"\n received from client : "<<buffer;
			for(;;)
			{
				
				for(i=0;i<strlen(buffer)&&buffer[i]!='1';i++);
				if(strlen(buffer)-i<m)
					break;
				for(int j=0;j<m;j++,i++)
				{
					if(c[j]!=buffer[i])
					{
							buffer[i]='1';
					}
					else
					{
						buffer[i]='0';
					}
				}
				cout<<"\n"<<buffer;
				for(i=0;i<strlen(buffer)&&buffer[i]!='1';i++);
				if(strlen(buffer)-i<m)
					break;
			}
			flag=0;
			for(int i=0;i<strlen(buffer);i++)
			{
				if(buffer[i]=='1')
				{
					flag=1;
				}	
			}
			if(flag==1)
			{
				cout<<"\n received bits contains error";
				cout<<"\n remainder is : "<<buffer;	
			}
			else
			{
				cout<<"\n received bits doesn't contain error";
			}
	}

	return 0;
}
