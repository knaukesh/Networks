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
#include <errno.h>
#include <sys/stat.h>
using namespace std;
int client; 
int portNum;
char client_path[400],rfs[2000],sts[2000],a[2000],temp[2000],filename[2000];
void refresh_client()
{
	for(int i=0;i<400;i++)
		client_path[i]='\0';
}
void refresh_()
{
	for(int i=0;i<2000;i++)
	{
		rfs[i]='\0';
		sts[i]='\0';
		temp[i]='\0';
		filename[i]='\0';
	}
}
void refresh_rfs()
{
	for(int i=0;i<2000;i++)
	{
		rfs[i]='\0';
	}
}
/*bool is_file(const char* path) {
    struct stat buf;
    stat(path, &buf);
    return S_ISREG(buf.st_mode);
}

bool is_dir(const char* path) {
    struct stat buf;
    stat(path, &buf);
    return S_ISDIR(buf.st_mode);
}*/
/*static void lookup(char *path)
{
    DIR *dirp;
    struct dirent *dp;


    if ((dirp = opendir(path)) == NULL) {
        perror("couldn't open '.'");
        return;
    }


    do {
        if ((dp = readdir(dirp)) != NULL) {
		if(is_file(dp->d_name)){
		strcat(buffer,dp->d_name);
		strcat(buffer,"\n");}
		
            
           
        }
    } while (dp != NULL);
	(void) closedir(dirp);
    return;
}*/
void connection()
{
char ip[] = "127.0.0.1";
	struct sockaddr_in server_addr;
	cout << "\n- Starting client..." << endl;
	client = socket(AF_INET, SOCK_STREAM, 0);
	if (client < 0) 
	{
		cout << "\n-Error establishing socket..." << endl;
		exit(-1);
	}
	cout << "\n- Socket client has been created...\n";
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portNum);	
	if (connect(client, (struct sockaddr *) &server_addr, sizeof(server_addr))< 0)
		cout << "- Connection failed to port number : " << portNum << endl;
	else
	{
		/*cin>>sts;
		send(client,sts,2000,0);
		recv(client,rfs,2000,0);		
		cout<<rfs;*/
		while(true)
		{
			refresh_();
			cout<<"\n 1. get\n 2. put\n 3. ls\n 4. !ls\n 5. quit\n ";
			gets(a);
			strcpy(sts,a);
			send(client,sts,2000,0);
			refresh_();
			recv(client,rfs,2000,0);
			if(strcmp(rfs,"1")==0)
			{
				refresh_();
				if(a[0]=='g')
				{
					strcpy(sts,client_path);
					strcat(sts,"/");
					int i;
					for(i=0;i<strlen(a)-3;i++)
						filename[i]=a[i+4];
					filename[i]='\0';
					strcat(sts,filename);
					cout<<"\n file path : "<<sts;
					send(client,sts,2000,0);
					recv(client,rfs,2000,0);
					if(strcmp(rfs,"0")==0)
						cout<<"\n File doesn't exist";
					else
						cout<<"\n"<<filename<<" downloaded";
					refresh_();
				}	
				else if(a[0]=='p')
				{
					recv(client,rfs,2000,0);//server_path
					strcpy(temp,rfs);
					strcat(temp,"/");
					int i;
					/*for(i=strlen(temp);i<strlen(a)-3+strlen(temp);i++)
					{
						temp[i]=a[i-strlen(temp)+4];
						filename[i-strlen(temp)]=a[i-strlen(temp)+4];
					}
					temp[i]='\0';
					filename[i-strlen(temp)]='\0';*/
					for(i=0;i<strlen(a)-4;i++)
						filename[i]=a[i+4];
					filename[i]='\0';
					refresh_rfs();
					strcpy(rfs,client_path);
					strcat(rfs,"/");
					strcat(rfs,filename);
					cout<<"source file path "<<rfs;
					strcat(temp,filename);
					cout<<"\n file path at server side is :"<<temp;
					string ch;
					if (!ifstream(rfs))
					{
			     			cout << "File doesn't exist";
			     		}
					else
					{
						ifstream file1;
						file1.open(rfs);
						ofstream file2;
						file2.open(temp);
						while(!file1.eof())
						{
							getline(file1,ch);
							file2<<ch<<"\n";
						}
						file1.close();
						file2.close();
						cout<<"\n"<<filename<<" uploaded to server";
					}
				}
				else if(a[0]=='l')
				{
					refresh_();
					recv(client,rfs,2000,0);
					cout<<"\n list of files on server side:"<<"\n"<<rfs<<"\n";
		
				}
				/*else if(a[0]=='c')
				{
					refresh_();
					recv(client,rfs,2000,0);//server_path
					cout<<"\n Current path of server is :"<<rfs;
					refresh_();
					recv(client,rfs,2000,0);
					cout<<"\n New server path :"<<rfs;
					
				}*/
				else if(a[0]=='!' && a[1]=='l')
				{
					refresh_();
					strcpy(sts,client_path);
					send(client,sts,2000,0);
					recv(client,rfs,2000,0);
					cout<<"\n list of files on client side:"<<"\n"<<rfs<<"\n";
				}
				/*else if(a[0]=='!' && a[1]=='c')
				{
					refresh_();
					strcpy(sts,client_path);
					send(client,sts,2000,0);//client_path
					cout<<"\n Current path of client is :"<<sts;
					recv(client,rfs,2000,0);//new client_path 
					cout<<"\n New client path :"<<rfs;
					refresh_client();
					strcpy(client_path,rfs);
				
				}*/
				else if( a[0]=='q')
					break;
					
			}
			else
				cout<<"\n Invalid FTP command";		
	
			
			 
		}
		
	}
}
int main(int argc,char *argv[])
{
if(argc<3)
	return 0; 
strcpy(client_path,getcwd(NULL, 0));
	if ( client_path != NULL)
    	{
		cout<<"client path"<<client_path<<"\n";		
	}
portNum=atoi(argv[2]);
connection();
cout<<"bye\n";
close(client);
return 0;
}
