#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include<fstream>
using namespace std;

int main(int argc, char *argv[]) 
{
	if(argc<2)
		return 0;
	int server, ps; 
	int portNum = atoi(argv[1]); 
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
	cout << "- Looking for proxy server..." << endl;
	listen(server, 10);
	ps = accept(server, (struct sockaddr *) &server_addr, &size);
	if (ps < 0)
		cout << "- Error on accepting..." << endl;
	else
	{
	char c[500];
	char d[500],filename[100],r[500];
	while(true)
	{
		cout<<"\n\nstarting .......\n";
		recv(ps,c,500,0);
		if(strcmp(c,"stop")==0)
		{
			close(ps);
			exit(-1);
			close(server);
			return 0;
		}
		else if(strcmp(c,"File request received")==0)
			strcpy(d,"1");
		else
		{	
			cout<<"\n Received from proxyserver:"<<c;
			int i=0,j;
			while(i<strlen(c))
			{
				if(c[i]=='/')
				j=i+1;
				i++;
			}
			i=0;
			while(j<strlen(c))
			{
				filename[i]=c[j];
				j++;
				i++;
			}
			filename[i]='\0';
			cout<<"\n File name : "<<filename;
			//strcat(c,"/");
			//strcat(c,filename);
			cout<<"\n File path in proxy server : "<<c;
			if (!ifstream(filename))
			{
				strcpy(d,"0");
				cout<<"\n File doesn't exist";
			}
			else
			{
				string ch;
				ifstream file1;
				file1.open(filename);
				ofstream file2;
				file2.open(c);
				while(!file1.eof())
				{
					getline(file1,ch);
					file2<<ch<<"\n";
				}
				file1.close();
				file2.close();
				strcpy(d,"1");
			}
		}
		send(ps,d,500,0);
	cout<<"\n END\n\n";
	}
	}
	/**/
	
}
