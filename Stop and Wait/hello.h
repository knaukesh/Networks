#include<stdio.h>
#include<iostream>
#include<fcntl.h>
#include<string.h>
#include "colors.h"
using namespace std;
typedef struct 
{
 int seqno;
 int ackno;
 char data[50];
}frame;
char str[][20]={"nkm","dfsd","dfs","xc","sdfs","dsfs","rew","ewr","oi","ppp"};
//client
void c_from_network_layer(char buffer[],int i)
{
 strcpy(buffer,str[i]);
}
void c_to_physical_layer(int pid1,frame *f)
{
 write(pid1,f,sizeof(frame));
}
void c_from_physical_layer(int pid1,frame *f)
{
 read(pid1,f,sizeof(frame));
}
int c_to_network_layer(frame *f,int finish,int start,int seqno)
{ 
 if(f->seqno==f->ackno)
 {
 	cout<<FGRN("\nAcknowledgment received for sequence number : ")<<seqno;
 	cout<<"\n"<<finish-start<<FGRN(" ticks elapsed before acknowledgment was received");
	seqno=seqno^1;
	return seqno;
 }
 return seqno;
}
//server
void s_from_network_layer(frame *f)
{
//cout<<FBLU("\nAcknowledgment received at client side\n");
 f->data[0]='\0';
 f->ackno=f->seqno; 
 cout<<FGRN("\nAcknowledgment sent for sequence number : ")<<f->seqno;
}
void s_to_physical_layer(int pid1,frame *f)
{
 write(pid1,f,sizeof(frame));
}
void s_from_physical_layer(int pid1,frame *f)
{
 read(pid1,f,sizeof(frame));
}
void s_to_network_layer(char buffer[],int count)
{

//cout<<count;
 cout<<FGRN("\nFrame received");
 cout<<FGRN("\nClient : ")<<buffer;
	
}
