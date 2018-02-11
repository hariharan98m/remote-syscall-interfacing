#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<time.h>
#include<netinet/in.h>
#include<math.h>

double e=2;
 
// Returns gcd of a and b
int gcd(int a, int h)
{
    int temp;
    while (1)
    {
        temp = a%h;
        if (temp == 0)
          return h;
        a = h;
        h = temp;
    }
}

enum message_type {SUCCESS=20, FAILURE=30, SIP=1, ASYMMETRIC= 2, SYMMETRIC=3, RSA=4, DE=5, HANDSHAKE=6, REQ_CERTIFICATE=7, RES_CERTIFICATE=8, KEYS_SAVED=9, ENC_MSG=10, DECR_MSG=11, CONN_AUTH_SUCCESS=12, SESSION_TOKEN_SENT=13, USER_INFO=14, OK=15};

enum message_type type;

struct test{
	int a,b;
	char str[30];
};

struct message{
	int type;
	char payload[30];
	char sessionToken[30];
};


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


typedef struct enc_msg{

	double enc;
	double e;

}enc_msg;


struct keys{
	double N;
	double PHI;
};


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
	struct keys *k;
	int counter=0;
	int bytes=0;
	int contFlag=0;
	enc_msg em, *dm;
	user *u;
	double c,e=2.0, msg=12.0,n,phi;

	l:	

//	switch(counter){		

//	case 0:		
		if((bytes= recv(sd2, (char*)&m, sizeof(m), 0))>0){
			fhs=(handshake*)m.payload;
			printf("\nBytes %d\nprotocol: %d\ncipher_suite : %d\nAlgorithm %d\n",bytes, fhs->protocol_v, fhs->cipher_suite, fhs->algorithm);	
		}

		type=RSA;		

		if(fhs->algorithm==type){

			m.type=REQ_CERTIFICATE;
			strcpy(m.payload, "Client accepted");

			memcpy(buffer, (void*)&m, sizeof(m));
			if((bytes= send(sd2, buffer, 36, 0))>0);
				//printf("bytes sent %d", bytes);
		}
//		break;

//	case 1:
		type=RES_CERTIFICATE;		
		m.type=type;
		
		if((bytes=recv(sd2, (char*)&m, sizeof(m), 0))>0){
			k=(struct keys*)m.payload;
			if(type==m.type)
				contFlag=1;
			printf("\nBYTES %d\nN: %lf\nPHI : %lf\n",bytes, k->N, k->PHI);	
			n=k->N;
			phi=k->PHI;
			type=KEYS_SAVED;
			m.type=type;

			strcpy(m.payload, "Key Saved");

			memcpy(buffer, (void*)&m, sizeof(m));
			if((bytes= send(sd2, buffer, 36, 0))>0);
				//printf("bytes sent %d", bytes);

		}

//		break;
	
//	case 3:
		//send the encrypted data

			type=ENC_MSG;
			m.type=type;

			while (e < phi)
			    {
				// e must be co-prime to phi and
				// smaller than phi.
				if (gcd(e, phi)==1)
				    break;
				else
				    e++;
			    }

			c = pow(msg, e);
			//printf("c after pow %lf", c);
			c = fmod(c,n);
			//printf("c after fmod %lf", c);

			em.enc=c;
			em.e=e;

			memcpy(m.payload, (void*)&em, sizeof(em));			
			memcpy(buffer, (void*)&m, sizeof(m));
			if((bytes= send(sd2, buffer, 36, 0))>0);
				//printf("bytes sent %d", bytes);
//			break;

//	case 4:

		type=DECR_MSG;		
		m.type=type;
		
		if((bytes=recv(sd2, (char*)&m, sizeof(m), 0))>0){
			dm=(struct enc_msg*)m.payload;
			if(type==m.type)
				contFlag=1;
			
			if(dm->enc==msg){

				type=CONN_AUTH_SUCCESS;
				m.type=type;

				strcpy(m.payload, "AUTH Success");

				memcpy(buffer, (void*)&m, sizeof(m));

				if((bytes= send(sd2, buffer, 36, 0))>0);
					//printf("bytes sent %d", bytes);
			}
		}

		

		type= USER_INFO;
		m.type= type;


		if((bytes=recv(sd2, (char*)&m, sizeof(m), 0))>0){
			u=(user*)m.payload;
			if(type==m.type)
				contFlag=1;
			
			if(u->privLevel==1){
				//write to a file to carry out commands as admin
			}

			type=SESSION_TOKEN_SENT;
			m.type=type;

			strcpy(m.payload, "SecretTokenUser");

			memcpy(buffer, (void*)&m, sizeof(m));

			if((bytes= send(sd2, buffer, 36, 0))>0);
				//printf("bytes sent %d", bytes);
		}




//	case 

		if((bytes= recv(sd2, (char*)&m, sizeof(m), 0))>0){
			fhs=(handshake*)m.payload;
			printf("\nBytes %d\nprotocol: %d\ncipher_suite : %d\nAlgorithm %d\n",bytes, fhs->protocol_v, fhs->cipher_suite, fhs->algorithm);	
		}

		type=DE;		

		if(fhs->algorithm==type){

			m.type=OK;
			strcpy(m.payload, "Into the DE en&de mode $");

			memcpy(buffer, (void*)&m, sizeof(m));
			if((bytes= send(sd2, buffer, 36, 0))>0);
				//printf("bytes sent %d", bytes);
		}


//		break;


//	counter++;

//	goto l;
	
	close(sockid);
	close(sd2);	
}
