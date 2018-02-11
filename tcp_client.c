#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>          
#include<sys/socket.h>
#include<time.h>
#include<netinet/in.h>
#include "rsa_secrets.h"

enum message_type {SUCCESS=20, FAILURE=30, SIP=1, ASYMMETRIC= 2, SYMMETRIC=3, RSA=4, DE=5, HANDSHAKE=6, REQ_CERTIFICATE=7, RES_CERTIFICATE=8, KEYS_SAVED=9, ENC_MSG=10, DECR_MSG=11, CONN_AUTH_SUCCESS=12};

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
	double N;
	double PHI;
};

typedef struct handshake{
	long int num;
	int protocol_v;
	int cipher_suite;
	int algorithm;
}handshake;


typedef struct enc_msg{

	double enc;

}enc_msg;


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
	struct message m,m2;
	handshake fhs;
	struct keys k;
	int counter=0;
	int bytes=0, contFlag=0;

	enc_msg *em;

	l:

	switch(counter){

	case 0 :  //SIP handshake

		type=HANDSHAKE;	
		m.type=type;
		fhs= create_handshake();

		memcpy((void*)m.payload, (void*)&fhs, sizeof(fhs));	
		memcpy(buffer, (void*)&m, sizeof(m));	

		if((bytes= send(sockid, buffer, 36, 0))>0);
			//printf("bytes sent %d",bytes);
	
		if((bytes=recv(sockid, (char*)&m, sizeof(m), 0))>0){
			//printf("bytes recv %d",bytes);
			printf("\nServer: %s", m.payload);

			type= REQ_CERTIFICATE;

			if(m.type==type)
				contFlag=1;
		}
		break;
	
	case 1:
		
		type=RES_CERTIFICATE;
		m.type=type;

		k.N= n;
		k.PHI= phi;

		memcpy((void*)m.payload, (void*)&k, sizeof(k));	
		memcpy(buffer, (void*)&m, sizeof(m));	

		if((bytes= send(sockid, buffer, 36, 0))>0);
			//printf("bytes sent %d",bytes);

		type=KEYS_SAVED;
		
		if((bytes=recv(sockid, (char*)&m, sizeof(m), 0))>0){
			if(m.type==type){			
				//printf("bytes recv %d",bytes);
				printf("\nServer: %s", m.payload);
			}
		}
		
	break;

	case 2:

		
		if((bytes=recv(sockid, (char*)&m, sizeof(m), 0))>0){
			//printf("bytes recv %d",bytes);
			//printf("\nServer: %s", m.payload);
			em= (enc_msg*)m.payload;
			printf("\nENC msg: %lf\ne= %lf\n", em->enc, em->e);

			type= ENC_MSG;
			if(m.type==type)
				contFlag=1;

			int k = 2;  // A constant value
			double d = (1 + (k*phi))/em->e;
 
			double m = pow(c, d);
			m = fmod(m, n);
			
			type= DECR_MSG;
			m.type=type;

			memcpy((void*)m.payload, (void*)&k, sizeof(k));	
			memcpy(buffer, (void*)&m, sizeof(m));	

			if((bytes= send(sockid, buffer, 36, 0))>0);
				//printf("bytes sent %d",bytes);
		}
		
		

		break;		

	case 3:
	




	}	
	counter++;

	goto l;	

	close(sockid);
}
