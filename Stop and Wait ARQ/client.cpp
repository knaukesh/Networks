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
#include "hello.h"
using namespace std;
int client;


int main(int argc,char *argv[]) 
{
	if(argc<3)
		return 0; 
	int portNum = atoi(argv[2]);
	int bufsize = 100; 
	//,count_[bufsize]; 
	char result[bufsize],buffer[1024];
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
	frame f;
	int start,finish,i=1,seqno=0;
	float avg=0;
	while(i<=10)
	{
		
		cout<<"\n";
		c_from_network_layer(buffer,i-1);
		strcpy(f.data,buffer);
		f.seqno=seqno;
		f.ackno=2;
		cout<<"\n Frame";
		cout<<"\n-------------------------------";
		cout<<"\n Seqno : "<<seqno;
		cout<<"\n Ackno : "<<f.ackno;
		cout<<"\n Message : "<<buffer;
		cout<<"\n-------------------------------\n\n";
		c_to_physical_layer(client,&f);
		if(strcmp(f.data,"stop")==0)
		break;
		int c=(rand()%20);
		cout<<"\n "<<c;
		start=clock();
		cout<<"\n Timer started at : "<<start;
		cout<<"\n Timeout in "<<c<<" ticks";
		finish=clock();
		cout<<"\n Finish time is : "<<finish;
		//cout<<clock();
		if(c+start<finish)
		{
			cout<<"\n\n Timeout\n Acknowledgment not received for sequence number : "<<f.seqno;
			seqno=f.seqno;	
			continue;
		}
		else
		{
			c_from_physical_layer(client,&f);
			int k;
			k=f.seqno;
			seqno=c_to_network_layer(&f,finish,start,seqno);
			i++;
			//if(k==seqno)
			//{
		//		continue;	
		//	}
			
		}
		avg=avg+finish-start;
		//cout<<"\n New average is : "<<avg/i;	
	}
	usleep(5000000);
	close(client);
	return 0;
}
