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
#include<iomanip>
using namespace std;
int client;
int main(int argc,char *argv[]) 
{
	if(argc<3)
		return 0; 
	int portNum = atoi(argv[2]);
	int bufsize = 100; 
	//,count_[bufsize]; 
	char result[bufsize],buffer[1024],message[1024];
	struct sockaddr_in server_addr;
	cout << "\n- Starting client..." << endl;
	client = socket(AF_INET, SOCK_STREAM, 0);
	if (client < 0) 
	{
		cout << "\n-Error establishing socket..." << endl;
		exit(-1);
	}
	cout << "\n- Socket client has been created..." << endl;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portNum);
	if (connect(client, (struct sockaddr *) &server_addr, sizeof(server_addr))< 0)
		cout << "- Can't connect to the server port number: " << portNum << endl;
	int m,i ;
	for(i=0;i<1000;i++);
	m=3+clock()%13;
	char c[30],d[m-1];
	//int i;
	for( i=0;i<m-1;i++)
		d[i]='0';
	d[i]='\0';
	c[0]='1';
	
	for(int i=1;i<m;i++)
	{
		if(clock()%2==0)
		{
			c[i]='0';
		}
		else
		{
			c[i]='1';
		}
	}
	c[m]='\0';
	send(client,c,30,0);	
	cout<<"\n crc function : "<<c;
	//int i;
	while(true)
	{//000000001111
	 //10101
			cout<<"\n Enter bits :";	
			cin>>buffer;
			cout<<"\n Original bits : "<<buffer;
			strcat(buffer,d);
			strcpy(message,buffer);
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
			cout<<"\n after adding remainder";
			for(int j=i;j<strlen(message);j++)
			{
				if(message[j]!=buffer[j])
				{
					message[j]='1';
				}
				else
				{
					message[j]='0';
				}
				
			}
			cout<<"\n Encoded bits : "<<message;	
			if(clock()%2==0)
			{
				cout<<"\n sending bits with error:";
				int k;
				k=clock()%strlen(buffer);
				if(message[k]=='0')
					message[k]='1';
				else
					message[k]='0';
				cout<<"\n Encoded bits with error : "<<message;
			}
					
			send(client,message,1024,0);
	}
	
	close(client);
	return 0;
}
