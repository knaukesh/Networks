#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstdio>
#include <netdb.h>
#include<fstream>
using namespace std;
char ps_path[400],cache_[5][100];
void handler(int pc,int client,int ps)
{
char c[500];
	char d[500],filename[100],r[500];
int l=0;


	while(true)
	{
		
		recv(client,c,500,0);
		if(strcmp(c,"stop")!=0)
		{
			int i,j;
			cout<<"\n\nstarting ........\n";
			cout<<"\n Received from client : "<<c;
			i=0,j;
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
			if (!ifstream(filename))
			{
				
				cout<<"\n File is not present in the Proxy server.\n Fetching from server....";
				strcpy(r,ps_path);
				strcat(r,"/");
				strcat(r,filename);
				cout<<"\n File path in Proxy server:"<<r;
				send(pc, r, 500, 0);
				recv(pc, d, 500, 0);
				if(strcmp(d,"1")==0)
				{
					cout<<"\n File received from server \n Sending to client...";
					if (!ifstream(filename))
					{
						cout<<"\n File doesn't exist";
						strcpy(d,"0");
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
						cout<<"\n "<<filename<<" sent to client";
						remove(cache_[l]);
						strcpy(cache_[l],filename);
						l++;
						l=l%5;
					}
				}
				else
				{
					cout<<"\n File doesn't exist";		
					strcpy(d,"0");
				}
			}
			else
			{
				send(pc, "File request received", 500, 0);
				recv(pc,r, 500, 0);
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
				cout<<"\n "<<filename<<" sent to client";
			}
			send(client,d,500,0);
		}
		else
		{
			for(int i=0;i<5;i++)
				remove(cache_[i]);
			send(pc, c, 500, 0);
				
			close(client);
			exit(1);
			close(ps);
			close(pc);
			return ;
		}
		cout<<"\n LIST OF FILES IN PROXY SERVER FOLDER\n";
			for(int i=0;i<5;i++)
				cout<<" "<<i+1<<". "<<cache_[i]<<"\n";
		cout<<"\n END\n\n";
	}
}
int main(int argc, char *argv[])
{
	if(argc<3)
	return 0;
	//start of proxy server
	int ps, client; 
	int portNum1 = atoi(argv[1]); 
	//char ps_path[400];
	strcpy(ps_path,getcwd(NULL, 0));
	struct sockaddr_in server_addr1;
	socklen_t size1;
	cout << "\n- Starting proxy server..." << endl;
	ps = socket(AF_INET, SOCK_STREAM, 0);
	if (ps < 0) {
		cout << "Error establishing socket ..." << endl;
		exit(-1);
	}
	cout << "- Socket server has been created..." << endl;
	server_addr1.sin_family = AF_INET;
	server_addr1.sin_addr.s_addr = htons(INADDR_ANY);
	server_addr1.sin_port = htons(portNum1);
	if ((bind(ps, (struct sockaddr*) &server_addr1, sizeof(server_addr1)))< 0) 
	{
		cout << "- Error binding connection, the socket has already been established..." << endl;
		exit(-1);
	}
		size1 = sizeof(server_addr1);
	cout << "- Looking for client..." << endl;
	listen(ps, 10);
	/*c = sizeof(struct sockaddr_in);
	*/
	client = accept(ps, (struct sockaddr *) &server_addr1, &size1);
	if (client < 0)
	{
		cout << "- Error on accepting..." << endl;
		exit(-1);
	}
	
	//while(true)
	//{
		
	int pc; 
	int portNum2=atoi(argv[2]);
	char ip[] = "127.0.0.1";
	struct sockaddr_in server_addr2;
	cout << "\n- Starting proxy server..." << endl;
	pc = socket(AF_INET, SOCK_STREAM, 0);
	if (pc < 0) 
	{
		cout << "\n-Error establishing socket..." << endl;
		exit(-1);
	}
	cout << "\n- Socket proxy server has been created..." << endl;
	server_addr2.sin_family = AF_INET;
	server_addr2.sin_port = htons(portNum2);
	if (connect(pc, (struct sockaddr *) &server_addr2, sizeof(server_addr2))< 0)
	{
		cout << "- Can't connect to the server port number: " << portNum2 << endl;
		exit(-1);
	}
	handler(pc,client,ps);
	
	
return 0;
}
