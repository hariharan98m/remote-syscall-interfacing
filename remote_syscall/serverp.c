#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>          /* See NOTES */
#include<sys/socket.h>
#include<time.h>
#include<netinet/in.h>


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
	type= SIP;
	hs.protocol_v=type;	//Session Init Protocol
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

	int sockid=socket(AF_INET, SOCK_STREAM, 0), sd2, portno;

	char date_time[200];
	
	time_t mytime;
    mytime = time(NULL);
    strcpy(date_time, ctime(&mytime));

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
	message *hello;
	handshake *datacheck, *data;

	printf("DUMMY protocol_v: %d\ncipher suite: %d\nalgo: %d\n\n", data->protocol_v, data->cipher_suite, data->algorithm); 
	l:

	if(recv(sockid, buffer, sizeof(buffer), 0)){
		
		printf("buffer length %d", strlen(buffer));
	
		datacheck=(handshake*)(buffer);

		printf("protocol_v: %d\ncipher suite: %d\nalgo: %d", datacheck->protocol_v, datacheck->cipher_suite, datacheck->algorithm); 
		
	};

//	if(send(sd2, buffer, sizeof(buffer), 0)>0)
//		;
	
	
			
	close(sockid);
	close(sd2);	
}
