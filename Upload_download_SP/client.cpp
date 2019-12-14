#include <iostream>
#include <string.h>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstdio>
#include <netdb.h>
#include<dirent.h>

using namespace std;
int main() 
{
	int client; 
	int portNum = 4444; 
	int bufsize = 1024; 
	char buffer[bufsize];
	char *client_add=NULL;
	client_add = getcwd(NULL, 0); // or _getcwd
	if ( client_add != NULL)
    		{cout<<"client path"<<client_add;
	strcat(client_add,"/");	
	}
	char result[bufsize];
	char ip[] = "127.0.0.1";
	struct sockaddr_in server_addr;
	cout << "\n- Starting client..." << endl;
	client = socket(AF_INET, SOCK_STREAM, 0);
	if (client < 0) 
	{
		cout << "\n-Error establishing socket..." << endl;
		exit(-1);
	}
	cout << "\n- Socket client has been created..." << endl;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portNum);
	if (connect(client, (struct sockaddr *) &server_addr, sizeof(server_addr))< 0)
		cout << "- Connection to the server port number: " << portNum << endl;
	cout<<"hi";
	int c;
	while(true)
	{
		cout<<"\n 1. upload";
		cout<<"\n 2. download";
		cout<<"\n 3. exit\n   Enter your choice : ";
		cin>>c;
		sprintf(result, "%d", c);
		send(client, result, 5, 0);
		if(c==1)
		{
			char server_add[50],filename[50];
			recv(client,server_add,50, 0);
			cout<<"\n Enter file name:";
			cin>>filename;
			if (!ifstream(filename))
			{
	     			cout << "File doesn't exist";
	     		}
			else
			{
				strcat(server_add,filename);
				cout<<"uploaded file address is : "<<server_add<<"\n";
				string ch;
				ifstream file1;
				file1.open(filename);
				ofstream file2;
				file2.open(server_add);
				while(!file1.eof())
				{
					getline(file1,ch);
					file2<<ch<<"\n";
				}
				file1.close();
				file2.close();
				server_add[strlen(server_add)-strlen(filename)]='\0';
			}
		}
		else if(c==2)
		{
			char filename[50],flag[2];
			cout<<"\n Enter filename:";
			cin>>filename;
			strcat(client_add,filename);
			send(client, client_add, 50, 0);
			recv(client,flag,2, 0);
			if(strcmp(flag,"0")==0)
				cout<<"File doesn't exist";
			client_add[strlen(client_add)-strlen(filename)]='\0';
		}
		else 	
			break;		
	//send(client, buffer, bufsize, 0);
	//recv(client, result, bufsize, 0);
	result[0]='\0';
	}
	close(client);
	return 0;
}
