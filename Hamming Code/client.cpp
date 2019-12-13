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
#include<math.h>
#include<time.h>
using namespace std;
int client;
int hamming_code(char *c)
{
int m=strlen(c);
char d[1500];
cout<<" Message length is "<<m-1<<"\n";;
int k,p=0;
for(int i=1,k=1;k<m;i++)
{
	if(log2(i)!=floor(log2(i)))
	{
		d[i]=c[k];
		k++;
	}
	else
	{
		d[i]='0';
		p++;
	}

}
cout<<" Total number of checkbits required:"<<p<<"\n";
for(int i=1;i<m+p;i++)
{
	if(log2(i)!=floor(log2(i)))
	{
		int g=i,l;
		for(l=0;g>0;g=g/2,l++)
		{
			if(g%2!=0)
			{
				int a=floor(pow(2,l));
				if(d[i]=='1')
				{
					if(d[a]=='0')
						d[a]='1';
					else
						d[a]='0';
				}				
			}
		}
	}
}
cout<<"\n Encoded stream of bits : ";
int i;
for(i=1;i<m+p;i++)
{
	c[i]=d[i];
	cout<<c[i];
}
return m+p;
}

int main(int argc,char *argv[]) 
{
	if(argc<3)
		return 0; 
	int portNum = atoi(argv[2]);
	int bufsize = 100; 
	//,count_[bufsize]; 
	char result[bufsize],buffer[1500];
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
	int i;
	while(true)
	{
		cout<<"----------------------------------------------------------------";
		cout<<"\n Enter stream of bits (max(1024)): ";
		cin>>buffer;
		if(strcmp(buffer,"stop")!=0)
		{
			buffer[strlen(buffer)+1]='\0';
			for(i=strlen(buffer)-1;i>=0;i--)
				buffer[i+1]=buffer[i];
			int t=hamming_code(buffer);
			cout<<"\n";
			if(clock()%2==0)
			{
				int s=1+(clock()%(t-1));
				cout<<" Error introduced at index "<<s<<"\n";
				if(buffer[s]=='1')
					buffer[s]='0';
				else
					buffer[s]='1';
			}
			send(client,buffer,t,0);
			cout<<"----------------------------------------------------------------\n";
		}
		else
		{
			send(client,buffer,1500,0);
			break;
		}

	}
	close(client);
	return 0;
}
