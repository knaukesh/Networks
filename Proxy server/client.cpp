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

using namespace std;

int main(int argc, char *argv[]) 
{
if(argc<2)
	return 0;
int client;
char client_path[400],filename[100];
strcpy(client_path,getcwd(NULL, 0));
if ( client_path != NULL)
    	{
		cout<<"\nclient path : "<<client_path<<"\n";		
	}
int portNum = atoi(argv[1]);
char b[500],r[500];
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
	cout << "- Can't connect to the server port number: " << portNum << endl;
while(true)
{
cout<<"\n Enter file name:";
cin>>filename;
if(strcmp(filename,"stop")==0)
{
	
	strcpy(b,filename);
	send(client,b, 500, 0);
	close(client);
	return 0;
}
else
{
strcpy(b,client_path);
strcat(b,"/");
strcat(b,filename);
send(client, b, 500, 0);
recv(client, r, 500, 0);
if(strcmp(r,"1")==0)
	cout<<"\n File downloaded";
else
	cout<<"\n File doesn't exist";
}
}

return 0;
}
