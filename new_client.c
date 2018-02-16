#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>          
#include<sys/socket.h>
#include<time.h>
#include<netinet/in.h>
#include "deSecrets_client.h"
#include<math.h>


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


int main(){

	int sockid=socket(AF_INET, SOCK_STREAM, 0), portno;
	
	printf("Enter PORT: \n");
	scanf("%d", &portno);

	if(sockid<0)
		error("Error creating socket\n");
	else
		printf("Socket Created\n\n");

	struct sockaddr_in serv_addr;
	
	serv_addr.sin_family= AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port= htons(portno);
	
	if (connect(sockid,(struct sockaddr*)&serv_addr, sizeof(serv_addr))>=0)
		printf("Connected to Server Socket at port %d\n\n", portno);
	else
		error("Error Connecting to server\n");

//Start

	struct message m; //Create message 'm'
	handshake fhs;  //Handshake object is gonna send the first init info to server

	char buffer[200];   //Create a buffer for exchange

//Phase1: Exchange Key Info, random numbers, Ciphering, Algo and Protocol V.

		type=HANDSHAKE;	//HANDSHAKE type

		m.type=type;  //Set m type

		fhs= create_handshakeDE();

		memcpy((void*)m.payload, (void*)&fhs, sizeof(fhs));	
		memcpy(buffer, (void*)&m, sizeof(m));	

		if((bytes= send(sockid, buffer, 36, 0))>0);
			//printf("bytes sent %d",bytes);

//After sending, wait for a REQ_CERTIFICATE
	
		if((bytes=recv(sockid, (char*)&m, sizeof(m), 0))>0){
			//printf("bytes recv %d",bytes);
		
			type= REQ_CERTIFICATE;

			if(m.type!=type){
				error("REQ_CERTIFICATE not found");

			}
			//if req certificate is not returned, then exit
						
			//else	send the DE
			//set the type
			type= DE_XCHG;
			m.type=type;

			DHkeys dhk; dhk.P=p; dhk.G=g; dhk.secret= power(g, B, p);//Create the Key message
			
			//Copy the message to buffer			
			memcpy((void*)m.payload, (void*)&dhk, sizeof(dhk));	
			memcpy(buffer, (void*)&m, sizeof(m));	
			
			//send the message
			if((bytes= send(sockid, buffer, 36, 0))>0);
				//printf("bytes sent %d",bytes);

		}

		//Key sent. Wait for G^A. 
		type=G_SECRET;

		if((bytes=recv(sockid, (char*)&m, 36, 0))>0){
			//printf("bytes recv %d",bytes);
			if(m.type!=type)
				error("G_SECRET not found");

			DHkeys *dhk=(DHkeys*) m.payload;
			printf("G^A  %ld", dhk->secret);
			
			//Send kb to server for auth. 
			dhk->secret= power(dhk->secret, B,p);			

			//Copy the message to buffer			
			memcpy((void*)m.payload, (void*)dhk, sizeof(dhk));	
			memcpy(buffer, (void*)&m, sizeof(m));	
			
			//send the message
			if((bytes= send(sockid, buffer, 36, 0))>0);
			
		}
		
		//Set the type to CONN_AUTH_SUCCESS
		type=CONN_AUTH_SUCCESS;

		user u;
		if((bytes=recv(sockid, (char*)&m, 36, 0))>0){
			//printf("bytes recv %d",bytes);
			if(m.type!=type)
				error("Key Exchange failed");

			//else send user struct 
			type= USER_INFO;
			m.type=type;

			//create the user object
			scanf("%s",u.username);
			scanf("%s",u.password);

			//create the user object
			if(!(strcmp(u.username, "admin"))){
				type=ADMIN; printf("asking for admin");
				m.privLevel=type;
			}
			else{
				type=USER;			
				m.privLevel=type;
			}
			memcpy(m.payload, (void*)&u, sizeof(u));	
			memcpy(buffer, (void*)&m, sizeof(m));	

			//send message
			if((bytes= send(sockid, buffer, 36, 0))>0);

		}


		//Get the session Token
		type=SESSION_TOKEN_SENT;
		
		if((bytes=recv(sockid, (char*)&m, 36, 0))>0){
			if(m.type!=type)
				error("Auth Failed");			

			printf("\nServer: %s", m.payload);
		}


//RSA handshake

		type=HANDSHAKE;	
		m.type=type;
		fhs= create_handshake();

		memcpy((void*)m.payload, (void*)&fhs, sizeof(fhs));	
		memcpy(buffer, (void*)&m, sizeof(m));	

		if((bytes= send(sockid, buffer, 36, 0))>0)
			printf("bytes sent %d",bytes);
		
		type=KEYS_SAVED;
		keys *k;

//Get the Keys
		if((bytes=recv(sockid, (char*)&m, 36, 0))>0){

			if(m.type!=type)
				error("KEYS Saved not ACK");
			k= (keys*) m.payload;
			printf("\nn= %lf, phi= %lf\n", k->N, k->PHI);

		}
	
	close(sockid);
}
