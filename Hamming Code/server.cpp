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
#include<math.h>
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
	
	char result[1500],buffer[1024]; 
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
	int i,t;
	for(int i=0;i<1500;i++)
		result[i]='\0';
	while(true)
	{
		recv(client,result,1500,0);
		if(strcmp(result,"stop")==0)
			break;
		cout<<"\n Received stream of bits from client : ";	
		for(int i=1;i<strlen(result);i++)
			cout<<result[i];
		for(int i=1;i<strlen(result);i++)
		{
			if(log2(i)!=floor(log2(i)))
			{
				int g=i,l;
				//cout<<"\n"<<i;
				for(l=0;g>0;g=g/2,l++)
				{
					if(g%2!=0)
					{
						int a=floor(pow(2,l));
						//cout<<"\n d["<<a<<"]="<<d[a];
						if(result[i]=='1')
						{
							if(result[a]=='0')
								result[a]='1';
							else
								result[a]='0';
						}
						//cout<<"\n d["<<a<<"]="<<d[a];
						
					}
				}
			}
		}
		bool f=true;
		int k=0;
		for(int i=1;i<strlen(result);i*=2)
		{
			if(result[i]=='1')
			{
				f=false;
				k=k+i;
			}
		}
		if(f)
			cout<<"\n NO error in received bits from client";
		else
			cout<<"\n Error encountered in received bits from client at index "<<k;
		for(int i=0;i<1500;i++)
		result[i]='\0';
		cout<<"\n";
		
				
	}
	close(client);
	exit(1);
	close(server);
	return 0;
}
/*
1 0 1 1 0 1 0 1 1  1 1
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
*/
