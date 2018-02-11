#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<time.h>
#include<netinet/in.h>

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

typedef struct handshake{
	long int num;
	int protocol_v;
	int cipher_suite;
	int algorithm;
}handshake;

void error(const char* err){
	printf("%s", err);
	exit(0);
}

int main(){

	int sockid=socket(AF_INET, SOCK_STREAM, 0), sd2, portno;

	printf("Enter port no\n");
	scanf("%d", &portno);
	
	if(sockid<0)
		error("Error creating socket\n");
	else
		printf("Socket Created successfully\n");

	struct sockaddr_in addr, client_addr;
	
	char send_msg[200], recv_msg[200];
	
	addr.sin_family= AF_INET;
	addr.sin_addr.s_addr=htonl(INADDR_ANY);
	addr.sin_port= htons(portno);
	
	if (bind(sockid, (struct sockaddr*)&addr, sizeof(addr))>=0)
		printf("Socket Binded to portno successfully\n");
	else
		error("Error binding socket\n");
	
	listen(sockid, 2);
	printf("Listening at Socket Port %d\n", portno);
	
	int clilen= sizeof(client_addr);
	sd2= accept(sockid, (struct sockaddr*)&client_addr, &clilen);

	if(sd2<0)
		error("Error Accepting Client\n");
	else
		printf("Rendzevous with Client Success\n\n Client Address: %d\n\n", inet_ntoa(((struct sockaddr_in)client_addr).sin_addr));

	char buffer[256];		
	struct message m;
//	struct test *t;
	struct handshake *fhs;

	l:	
		
		if(recv(sd2, (char*)&m, sizeof(m), 0)){
			fhs=(handshake*)m.payload;
			printf("\nprotocol: %d\ncipher_suite : %d\nAlgorithm %d\n",fhs->protocol_v, fhs->cipher_suite, fhs->algorithm);	
		}

		type=RSA;		

		if(fhs->algorithm==type){

			m.type=REQ_CERTIFICATE;
			strcpy(m.payload, "Client accepted");

			memcpy(buffer, (void*)&m, sizeof(m));
			if(send(sd2, buffer, sizeof(buffer), 0)>0)
				;
		}

		if(recv(sd2, (char*)&m, sizeof(m), 0)){
			fhs=(handshake*)m.payload;
			printf("\nprotocol: %d\ncipher_suite : %d\nAlgorithm %d\n",fhs->protocol_v, fhs->cipher_suite, fhs->algorithm);	
		}
				
	close(sockid);
	close(sd2);	
}
