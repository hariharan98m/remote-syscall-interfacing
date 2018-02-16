#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<time.h>
#include<netinet/in.h>
#include<math.h>
#include "rsa_secrets_server.h"

enum message_type type;
int bytes; 

long int power(long int a, long int b,
                                     long int P)
{ 
    if (b == 1)
        return a;
 
    else
        return (((long int)pow(a, b)) % P);
}


int main(){

	int sockid=socket(AF_INET, SOCK_STREAM, 0), sd2, portno;

	printf("Enter PORT: ");
	scanf("%d", &portno);
	
	if(sockid<0)
		error("Error creating socket\n");
	else
		printf("Socket Created\n");

	struct sockaddr_in addr, client_addr;
	
	addr.sin_family= AF_INET;
	addr.sin_addr.s_addr=htonl(INADDR_ANY);
	addr.sin_port= htons(portno);
	
	if (bind(sockid, (struct sockaddr*)&addr, sizeof(addr))>=0)
		printf("Socket Binded to %d\n", portno);
	else
		error("Error binding socket\n");
	
	listen(sockid, 2);
	printf("Listening on Port %d......\n", portno);
	
	int clilen= sizeof(client_addr);
	sd2= accept(sockid, (struct sockaddr*)&client_addr, &clilen);

	if(sd2<0)
		error("Error Accepting Client\n");
	else
		printf("Rendzevous with Client\n\n Client inet_ntoa: %d\n\n", inet_ntoa(((struct sockaddr_in)client_addr).sin_addr));

	char buffer[256];		
	struct message m;
	handshake *fhs; //handshake pointer for recv the Init numbers

//Get the handshake info, cipher, algo, protocol v


	if((bytes= recv(sd2, (char*)&m, 36, 0))>0){
			fhs=(handshake*)m.payload;
			printf("\nProtocol: %d\nCipher Suite : %d\nAlgorithm %d\n",fhs->protocol_v, fhs->cipher_suite, fhs->algorithm);	
		}

	type=DE;		//Check if type is DE

	if(fhs->algorithm==type){

		//if it is, then send a REQ_CERTIFICATE
		m.type=REQ_CERTIFICATE;

		memcpy(buffer, (void*)&m, sizeof(m)); //Copy the struct m to buffer

		if((bytes= send(sd2, buffer, 36, 0))>0); //Send REQ_CERTIFICATE
	}

	DHkeys *dhk;
	long int y;

	if((bytes= recv(sd2, (char*)&m, 36, 0))>0){
			dhk=(DHkeys*)m.payload;
			printf("\nP: %ld\nG : %ld\nCli %ld", dhk->P, dhk->G, dhk->secret);	
			//send G^A
			y=dhk->secret;

			type=G_SECRET;
			m.type= type;

			//Compute x and send
			dhk->secret= power(dhk->G, A, dhk->P);

			//Copy the message to buffer			
			memcpy((void*)m.payload, (void*)dhk, sizeof(dhk));	
			memcpy(buffer, (void*)&m, sizeof(m));	
			if((bytes= send(sd2, buffer, 36, 0))>0); //G^A
	
	}

	//Get kb from client
	//Compute ka on the server
	//Equate both
	if((bytes= recv(sd2, (char*)&m, 36, 0))>0){
			dhk=(DHkeys*)m.payload;
			printf("\ncliVal power a: %ld\nkb %ld\n", power(y, A, dhk->P), dhk->secret);	

		//if ka==kb then 
		if(dhk->secret==power(y, A, dhk->P)){
			//key exchange success
			//send the sessionToken
			type=CONN_AUTH_SUCCESS;
			m.type=type;

			strcpy(m.payload, "AUTH Success");

			memcpy(buffer, (void*)&m, sizeof(m));

			if((bytes= send(sd2, buffer, 36, 0))>0);
		}
	}

	type= USER_INFO;
	user *u;

	if((bytes=recv(sd2, (char*)&m, 36, 0))>0){
			u=(user*)m.payload;
			if(type!=m.type)
				error("User info not found");

			if(!strcmp(u->username, "admin")){
				printf("admin, need to write to a file");
				//write to a file to carry out commands as admin
			}

			//send the session token
			type=SESSION_TOKEN_SENT;
			m.type=type;
			strcpy(m.payload, "SecretTokenKey");

			memcpy(buffer, (void*)&m, sizeof(m));

			if((bytes= send(sd2, buffer, 36, 0))>0);
				//printf("bytes sent %d", bytes);
		}


	//Accept RSA hanshake

	if((bytes= recv(sd2, (char*)&m, 36, 0))>0){
			fhs=(handshake*)m.payload;
			printf("\nProtocol: %d\nCipher Suite : %d\nAlgorithm %d\n",fhs->protocol_v, fhs->cipher_suite, fhs->algorithm);	
	printf("fhs->algorithm %d", fhs->algorithm);
		}


	type=RSA;		//Check if type is DE

	//Ask for the Pvt Key
	
	if(fhs->algorithm==type){

		//if it is, then send the Public Keys
		printf("sending the keys");
		type= RSAKEY;

		m.type=type;
		
		keys k; k.N= n; k.PHI= phi;

		memcpy((void*)m.payload, (void*)&k, sizeof(k));	
		memcpy(buffer, (void*)&m, sizeof(m));	

		if((bytes= send(sockid, buffer, 36, 0))>0);
	}



//Read the values from client
/*
	type= ENC_MSG;

	if((bytes= recv(sd2, (char*)&m, 36, 0))>0){
		if(m.type!=type)
			error("Not an ENC MSG");
		int arr[20]= decrypt((int*)(m.payload));
	}
*/

	close(sockid);
	close(sd2);
}
