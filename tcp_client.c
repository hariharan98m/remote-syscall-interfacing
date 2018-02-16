#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>          
#include<sys/socket.h>
#include<time.h>
#include<netinet/in.h>
#include "rsa_secrets.h"
#include<math.h>

long int P;


enum message_type {b=4, SUCCESS=20, FAILURE=30, SIP=1, ASYMMETRIC= 2, SYMMETRIC=3, RSA=4, DE=5, HANDSHAKE=6, REQ_CERTIFICATE=7, RES_CERTIFICATE=8, KEYS_SAVED=9, ENC_MSG=10, DECR_MSG=11, CONN_AUTH_SUCCESS=12, SESSION_TOKEN_SENT=13, USER_INFO=14, OK=15, MSG_TYPE=16};

enum message_type type;

struct test{
	int a,b;
	char str[30];
};

typedef struct DHkeys{
	long int P;
}DHkeys;


struct message{
	int type;
	char payload[30];
	char sessionToken[30];
	int size;

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


typedef struct user{
	char username[20];
	char password[20];
	long int sessionToken;
	int privLevel;
}user;

typedef struct encDecMsg{
	int cmd[20];
}encDecMsg;


typedef struct enc_msg{

	double enc;
	double e;

}enc_msg;


handshake create_handshakeDE(){
	handshake hs;
	type= SIP;
	hs.protocol_v=type;	//Syscall Interfacing Protocol
	type= SYMMETRIC;
	hs.cipher_suite=type;
	type= DE;
	hs.algorithm=type;
	return hs;
}


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


int ascii_val(char c){
	return (long int)c;

}


int power(long int a, long int b,
                                     long int P)
{ 
    if (b == 1)
        return a;
 
    else
        return (int)(((long int)pow(a, b)) % P);
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


	char buffer[256], sessionToken[20];
	struct test t;
	struct message m,m2;
	handshake fhs;
	struct keys k;
	int counter=0;
	int bytes=0, contFlag=0;
	user u;
	enc_msg *em, dm;
	long int P, G;


	l:

//	switch(counter){

//	case 0 :  //SIP handshake

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
//		break;
	
//	case 1:
		
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
		
//	break;

//	case 2:

		
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
 
			double md = pow(em->enc, d);
			
			md = fmod(md, n);
			printf("\nd: %lf\ne= %lf\nenc_msg %lf\n", d, em->e, em->enc);
			
			type= DECR_MSG;
			m.type=type;
			
			dm.enc=md;
			printf("md = %lf", md);
			memcpy(m.payload, (void*)&dm, sizeof(dm));	
			memcpy(buffer, (void*)&m, sizeof(m));	

			if((bytes= send(sockid, buffer, 36, 0))>0);
				//printf("bytes sent %d",bytes);
		}		

//		break;		

//	case 3:
	
		type=CONN_AUTH_SUCCESS;
		
		if((bytes=recv(sockid, (char*)&m, sizeof(m), 0))>0){
			if(m.type==type){			
				//printf("bytes recv %d",bytes);
				printf("\nServer: %s", m.payload);
			}
		}

//	case 4:
		type= USER_INFO;
		m.type=type;

		scanf("%s",u.username);
		scanf("%s",u.password);
		if(!(strcmp(u.username, "user")))
			u.privLevel=0;
		memcpy(m.payload, (void*)&u, sizeof(u));	
		memcpy(buffer, (void*)&m, sizeof(m));	

		if((bytes= send(sockid, buffer, 36, 0))>0);
		
		type=SESSION_TOKEN_SENT;
		
		if((bytes=recv(sockid, (char*)&m, sizeof(m), 0))>0){
			if(m.type==type){			
				//printf("bytes recv %d",bytes);
				printf("\nServer: %s", m.payload);
				strcpy(sessionToken, m.sessionToken);
				contFlag=1;
			}
		}

//	case 5:
		type=HANDSHAKE;	
		m.type=type;
		fhs= create_handshakeDE();

		memcpy((void*)m.payload, (void*)&fhs, sizeof(fhs));	
		memcpy(buffer, (void*)&m, sizeof(m));	

		if((bytes= send(sockid, buffer, 36, 0))>0);
			//printf("bytes sent %d",bytes);
	
		if((bytes=recv(sockid, (char*)&m, 36, 0))>0){
			printf("bytes recv %d",bytes);
			DHkeys *dhk=(DHkeys*)m.payload;
			printf("\nP: %ld", dhk->P);
			P=dhk->P;
			type= OK;

			if(m.type==type)
				contFlag=1;
		}


//	send_message()



//	Create message
	type=MSG_TYPE;
	m.type=type;

	m.size=2;

	strcpy(buffer, "hi");

	encDecMsg edm;

	int i=0;
	for(i=0;i<strlen(buffer);i++){
		edm.cmd[i]=power(ascii_val(buffer[i]), A, P);	
		printf("msg[i] %d\n", edm.cmd[i]);
	}

	memcpy((void*)m.payload, (void*)&edm, sizeof(edm));
	memcpy(buffer, (void*)&m, sizeof(m));

	if((bytes=send(sockid, buffer, 36, 0))>0){
		printf("bytes sent %d",bytes);
		if(m.type==type)
			contFlag=1;
	}



//	}	
//	counter++;

//	goto l;	

	close(sockid);
}

