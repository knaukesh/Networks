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
#include"hello.h"
#include "colors.h"
using namespace std;
//char buff[100];
int client;
int main(int argc,char *argv[]) 
{
	int server; 
	//cout<<argc;
	if(argc<3)
		return 0;
	int portNum = atoi(argv[2]) ;
	int bufSize = 1024; 
	
	char result[bufSize],buffer[1024]; 
	struct sockaddr_in server_addr;
	socklen_t size;
	cout << FGRN("\n- Starting server...") << endl;
	server = socket(AF_INET, SOCK_STREAM, 0);
	if (server < 0) {
		cout << FRED("Error establishing socket ...") << endl;
		exit(-1);
	}
	cout << FGRN("- Socket server has been created...") << endl;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_family = AF_INET;
	//server_addr.sin_addr.s_addr = htons(INADDR_ANY);
	server_addr.sin_port = htons(portNum);
	if ((bind(server, (struct sockaddr*) &server_addr, sizeof(server_addr)))< 0) 
	{
		cout << FRED("- Error binding connection, the socket has already been established...") << endl;
		exit(-1);
	}
		size = sizeof(server_addr);
	cout << FGRN("- Looking for clients...") << endl;
	listen(server, 10);
	client = accept(server, (struct sockaddr *) &server_addr, &size);
	if (client < 0)
		cout << FRED("- Error on accepting...") << endl;
	frame f;
	int seqno=1,count=0,k;
	f.seqno=0;
	while(count<=10)
	{
		cout<<"\n";
		s_from_physical_layer(client,&f);
		strcpy(buffer,f.data);
		if(seqno!=f.seqno)
		{
			if(count!=0)
			cout<<FBLU("\nAcknowledgment received at client side\n");
			s_to_network_layer(buffer,count);
			count++;
			
		}
		else
		{
			cout<<FRED("\nAcknowledgment not received at client side");
			cout<<FGRN("\nResending ACKNOWLEDGMENT frame\n");
			
		}
		s_from_network_layer(&f);		
		seqno=f.seqno;
		s_to_physical_layer(client,&f);
		//usleep(100000);
					
	}
	close(client);
	exit(1);
	close(server);
	return 0;
}
