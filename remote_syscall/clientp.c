#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>         
#include<sys/socket.h>
#include<time.h>
#include<netinet/in.h>
#include "rsa_secrets.h"

enum message_type {CONNECT_OK=1, TRACK=10, TRACK_CONFIRM=15, STOP=20, STOP_CONFIRM=25, SUCCESS=30, FAILURE=40, QUIT = 50, PAYLOAD_SIZE=50, SIP=40, SYMMETRIC=5, ASYMMETRIC=6, RSA=61, DH=62, HANDSHAKE=112};

enum message_type type;


typedef struct message{
	int type;
	char payload[PAYLOAD_SIZE];
	char text[30];
}message;

typedef struct user{
	char username[20];
	char password[20];
	long int sessionToken;
	int privLevel;
}user;

typedef struct handshake{
	long int num;
	int protocol_v;
	int cipher_suite;
	int algorithm;
}handshake;

typedef struct rsa_key{
	int N,e;
}rsa_key;

handshake create_handshake(){
	handshake hs;
	type= SIP;//40
	hs.protocol_v=type;	//Session Init Protocol
	type= ASYMMETRIC;//6
	hs.cipher_suite=type;
	type= RSA;//61
	hs.algorithm=type;
	return hs;
}

void error(const char* err){
	printf("%s", err);
	exit(0);
}

int main(){

	int sockid=socket(AF_INET, SOCK_STREAM, 0), portno;
	
	printf("Enter port no\n");
	scanf("%d", &portno);

	if(sockid<0)
		error("Error creating socket\n");
	else
		printf("Socket Created successfully\n\n");

	struct sockaddr_in serv_addr;
	
	
	serv_addr.sin_family= AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port= htons(portno);
	
	if (connect(sockid,(struct sockaddr*)&serv_addr, sizeof(serv_addr))>=0)
		printf("Connected to Server Socket at port %d\n\n", portno);
	else
		error("Error Connecting to server\n");

	char buffer[256];
	message hello, *Mdata;
	handshake data, *datacheck;
	char* ptr;

	l:

	type= HANDSHAKE; 
	hello.type=type;

	data= create_handshake();
	printf("sizeof data: %d", sizeof(data));
	memcpy((char*)hello.payload, (handshake*)&data, sizeof(data));
	memcpy(buffer, (message*)&hello, sizeof(hello));
	printf("buffer %s\n", buffer);

	memcpy((void*)Mdata, buffer, sizeof(buffer));
	memcpy((void*) datacheck, (void*)Mdata, sizeof(Mdata));

	printf("protocol_v: %d\ncipher suite: %d\nalgo: %d", datacheck->protocol_v, datacheck->cipher_suite, datacheck->algorithm); 
/*
	ptr= (char*)&hello;
	Mdata= (message*)ptr;

	printf("hello payload %s", Mdata->payload);
	
	datacheck=(handshake*)Mdata->payload;


*/
	
	if(send(sockid, buffer, sizeof(buffer), 0)>0) printf("sent");

//	if(recv(sockid, (char*)buffer, sizeof(buffer), 0));

//	goto l;
	
	printf("Hi reached");
	
	close(sockid);
}
