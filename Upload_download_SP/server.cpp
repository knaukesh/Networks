#include <iostream>
#include <fstream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include<dirent.h>
using namespace std;
int main() 
{
	int server, client; 
	int portNum = 4444; 
	int bufSize = 1024; 
	char *server_add=NULL,error_msg[50];
	server_add = getcwd(NULL, 0); 
	strcpy(error_msg,"File doesn't exist");
	if ( server_add != NULL)
    		{
		cout<<"server path"<<server_add;
		strcat(server_add,"/");}
	char buffer[bufSize]; 
	struct sockaddr_in server_addr;
	socklen_t size;
	cout << "\n- Starting server..." << endl;
	server = socket(AF_INET, SOCK_STREAM, 0);
	if (server < 0) {
		cout << "Error establishing socket ..." << endl;
		exit(-1);
	}
	cout << "- Socket server has been created..." << endl;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);
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
	while(true)
	{
	recv(client, buffer, bufSize, 0);
	//send(client, buffer, bufSize, 0);
	if((strcmp(buffer,"1"))==0)
	{
		
		send(client, server_add,strlen(server_add), 0);
	}
	else if((strcmp(buffer,"2"))==0)
	{
		char client_add[50],*filename;
		string ch;
		recv(client, client_add, 50, 0);
		cout<<"File downloaded at : "<<client_add<<"\n";
		int i=0;
		while(i++ <strlen(client_add))
			if(client_add[i]=='/')
				filename=client_add+i+1;
		cout<<"filename: "<<filename<<"\n";
		if (!ifstream(filename))
		{
    
			send(client,"0",2, 0);
     			
		}
		else
		{
			send(client,"1",2, 0);
			ifstream file1;
			file1.open(filename);
			ofstream file2;
			file2.open(client_add);
			while(!file1.eof())
			{
				getline(file1,ch);
				file2<<ch<<"\n";
			}
			file1.close();
			file2.close();
			client_add[0]='\0';
			ch[0]='\0';
			filename[0]='\0';
		}
	}
	else
		break;
	}
	close(client);
	exit(1);
	close(server);
	return 0;
}
