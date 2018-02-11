#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>          
#include<sys/socket.h>
#include<time.h>
#include<netinet/in.h>
#include "rsa_secrets.h"

enum message_type {SUCCESS=20, FAILURE=30, SIP=1, ASYMMETRIC= 2, SYMMETRIC=3, RSA=4, DE=5, HANDSHAKE=6, REQ_CERTIFICATE=7, RES_CERTIFICATE=8};

enum message_type type;

struct test{
	int a,b;
	char str[30];


};

struct message{
	int type;
	char payload[30];
};

struct keys{
	long int n;
	int phi;
};

typedef struct handshake{
	long int num;
	int protocol_v;
	int cipher_suite;
	int algorithm;
}handshake;

handshake create_handshake(){
	handshake hs;
	type= SIP;
	hs.protocol_v=type;	//Syscall Interfacing Protocol
	type= ASYMMETRIC;
	hs.cipher_suite=type;
	type= RSA;
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
	
	char send_msg[200], recv_msg[200];
	
	serv_addr.sin_family= AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port= htons(portno);
	
	if (connect(sockid,(struct sockaddr*)&serv_addr, sizeof(serv_addr))>=0)
		printf("Connected to Server Socket at port %d\n\n", portno);
	else
		error("Error Connecting to server\n");


	char buffer[256];
	struct test t;
	struct message m;
	handshake fhs;
	
	l:

	type=HANDSHAKE;	
	m.type=type;
	fhs= create_handshake();

	memcpy((void*)m.payload, (void*)&fhs, sizeof(fhs));	
	memcpy(buffer, (void*)&m, sizeof(m));	

	if(send(sockid, buffer, sizeof(buffer), 0)>0)
		;
	
	if(recv(sockid, (char*)&m, sizeof(m), 0)){

		printf("\nServer: %s", m.payload);
	}

	type= REQ_CERTIFICATE;
	if(m.type== REQ_CERTIFICATE){
		
	}
	
//	goto l;

	
	close(sockid);
}
