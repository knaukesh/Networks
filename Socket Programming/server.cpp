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
#include<errno.h>
#include <sys/stat.h>
using namespace std;
int server, client; 
int portNum; 
char server_path[400],error_msg[100],rfc[2000],stc[2000],temp[2000],filename[2000];
bool is_file(const char* path) {
    struct stat buf;
    stat(path, &buf);
    return S_ISREG(buf.st_mode);
}
void refresh_()
{
	for(int i=0;i<2000;i++)
	{
		rfc[i]='\0';
		stc[i]='\0';
		filename[i]='\0';
	}
}
void refresh_temp()
{
	for(int i=0;i<2000;i++)
	{
		temp[i]='\0';
	}
}
void refresh_server()
{
	for(int i=0;i<2000;i++)
	{
		server_path[i]='\0';
	}
}
bool is_dir(const char* path) {
    struct stat buf;
    stat(path, &buf);
    return S_ISDIR(buf.st_mode);
}
static void lookup(char *path)
{
    DIR *dirp;
    struct dirent *dp;


    if ((dirp = opendir(path)) == NULL) {
        perror("couldn't open '.'");
        return;
    }


    do {
        if ((dp = readdir(dirp)) != NULL) {
		strcat(stc,dp->d_name);
		strcat(stc,"\n");
		
            
           
        }
    } while (dp != NULL);
	(void) closedir(dirp);
    return;
}
void connection()
{
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
	else
	{
		/*recv(client,rfc,2000,0);
		send(client,stc,2000,0);*/
		while(true)
		{
			refresh_();
			recv(client,rfc,2000,0);
			refresh_temp();
			strcpy(temp,rfc);
			refresh_();
			if((temp[0]=='g'&& temp[1]=='e'&&temp[2]=='t'&&temp[3]==' ')||(temp[0]=='p'&& temp[1]=='u'&&temp[2]=='t'&&temp[3]==' ')||(strcmp(temp,"ls")==0)||(temp[0]=='c'&& temp[1]=='d'&&temp[2]==' ')||(strcmp(temp,"!ls")==0)||(temp[0]=='!' &&temp[1]=='c'&& temp[2]=='d'&&temp[3]==' ')||(strcmp(temp,"quit")==0))
				strcpy(stc,"1");
			else
				strcpy(stc,"0");

			send(client,stc,2000,0);
			refresh_();
			if(temp[0]=='g'&& temp[1]=='e'&&temp[2]=='t'&&temp[3]==' ')
			{
				recv(client,rfc,2000,0);
				int i=0,j;
				for(;i<strlen(rfc);i++)
					if(rfc[i]=='/')
						j=i+1;
				i=0;
				while(j<strlen(rfc))
				{
					filename[i]=rfc[j];
					i++;
					j++;
				}
				filename[i]='\0';
				strcpy(stc,server_path);
				strcat(stc,"/");
				strcat(stc,filename);
				cout<<"\n Source file path is "<<stc;
				cout<<"\nfilename"<<filename;
				if (!ifstream(stc))
				{
	    				send(client,"0",2, 0);
     				}
				else
				{
					send(client,"1",2, 0);
					string ch;
					ifstream file1;
					file1.open(stc);
					ofstream file2;
					file2.open(rfc);
					while(!file1.eof())
					{
						getline(file1,ch);
						file2<<ch<<"\n";
					}
					file1.close();
					file2.close();
				}
				refresh_();
			}
			else if(temp[0]=='p'&& temp[1]=='u'&&temp[2]=='t'&&temp[3]==' ')
			{		
				strcpy(stc,server_path);
				send(client,stc,2000,0);
				refresh_();
			}
			else if(strcmp(temp,"ls")==0)
			{
				stc[0]='\0';
				lookup(server_path);
				send(client,stc,2000,0);
				refresh_();
			}
			else if(temp[0]=='c'&& temp[1]=='d'&&temp[2]==' ')
			{	
				strcpy(stc,server_path);
				int j;
				send(client,stc,2000,0);
				if(strcmp(temp,"cd ..")==0)
				{
					j=strlen(server_path)-1;
					while(server_path[j]!='/')
						j--;
					server_path[j]='\0';
					strcpy(stc,server_path);
					send(client,stc,2000,0);
				}
				else 
				{
					for(j=3;j<strlen(temp);j++)	
						filename[j-3]=temp[j];
					filename[j]='\0';
						
						if(filename[0]=='/')
						{
							refresh_server();
							strcpy(server_path,filename);
						}
						else
						{
							strcat(server_path,"/");
							strcat(server_path,filename);
						}
						strcpy(stc,server_path);
						send(client,stc,2000,0);
					
				}
				refresh_();
			}
			else if(strcmp(temp,"!ls")==0)
			{	
				stc[0]='\0';
				recv(client,rfc,2000,0);
				lookup(rfc);//rfc=client_path
				send(client,stc,2000,0);
				refresh_();
			}
			else if(temp[0]=='!' &&temp[1]=='c'&& temp[2]=='d'&&temp[3]==' ')
			{
				recv(client,rfc,2000,0);//client_path
				cout<<"\n Client path"<<rfc;
				int j;
				cout<<"temp = "<<temp;
				if(strcmp(temp,"!cd ..")==0)
				{
					j=strlen(rfc)-1;
					while(rfc[j]!='/')
						j--;
					rfc[j]='\0';
					strcpy(stc,rfc);
					send(client,stc,2000,0);
				}
				else
				{
					for(j=4;j<strlen(temp);j++)	
						filename[j-4]=temp[j];
					filename[j]='\0';
					if(filename[0]!='/')
					{
						strcat(rfc,"/");
						strcat(rfc,filename);
					}
					else
						strcpy(rfc,filename);
					cout<<"rfc "<<rfc;
				

						strcpy(stc,rfc);
						send(client,stc,2000,0);
					
				}
				refresh_();
			}
			else if(strcmp(temp,"quit")==0)
				break;
			else
				cout<<"\n Invalid FTP command";
			
		}
	}
}
int main(int argc,char *argv[])
{
if(argc<3)
	return 0;
portNum = atoi(argv[2]) ;
strcpy(server_path,getcwd(NULL, 0));
strcpy(error_msg,"File doesn't exist");
if ( server_path != NULL)
{
cout<<"server path : "<<server_path<<"\n";	
connection();
close(client);
exit(1);
close(server);
}
return 0;
}
