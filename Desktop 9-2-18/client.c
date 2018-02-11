#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>         
#include<sys/socket.h>
#include<time.h>
#include<netinet/in.h>
#include<math.h>

void error(const char* err){
	printf("%s", err);
	exit(0);
}


const int genSize=13;

int rem[12];

int* xor(int div[], int gen[]){

	int i;

	for (i=1;i<=genSize-1;i++)
		if(div[i]==gen[i])
			rem[i-1]=0;
		else
			rem[i-1]=1;

	return rem;
}


void print_arr(int arr[], int size){

	int i=0;
	printf("\n");
	for(i=0;i<size;i++)
		printf("%d", arr[i]);
	printf("\n");
}


int char_val(char c){

	printf("\nCharacter %c: ASCII %d\n", c,(int)c);
	return (int)(c);
}

int bin[8];

int* bin_val(int val){
	int remainder, i=7;
	while(i!=-1){
		if(val!=0)
			bin[i]=val%2;
		else
			bin[i]=0;
		i--;
		val/=2;
	}
	printf("Bin Val: ");
	print_arr(bin,8);	
	return bin;
}


int dec_val(int arr[]){
	int i;
	int val=0;
	for(i=7;i>=0;i--){
		if(arr[i]==1)
			val+=pow(2,7-i);
	}
	printf("\nASCII Val: %d",val);
	return val;
}


char c_func(int a){
	printf("\n\nChar: %c",(char)a);
	return (char)(a);

}


int main(){
	

	int dw[100], gen[12];
	int dwSize;
	int zeroword[]={0,0,0,0,0,0,0,0,0,0,0,0,0};
	int div[20];
	int i, charSize;
	char sentWord[20];

	printf("Enter Generator polynomial:");	
	for(i=0;i<genSize;i++)
		scanf("%d", &gen[i]);

	printf("Gen polynomial: ");
	print_arr(gen,genSize);	

	int flag=0;

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
	char rem[200];
	strcpy(rem,"000000000000");


	l:

	if(recv(sockid, buffer, sizeof(buffer), 0))
			printf("\nServer says: %s", buffer);	

	charSize=36;

	dwSize=charSize-genSize-1;

	int j;
	int binArrr[8];
	char word[10];
	strcpy(word,"");
	char buffer2[1];
	for(i=0;i<3;i++){

		for(j=0;j<8;j++){
			if(buffer[i*8+j]=='0')
				binArrr[j]=0;		
			else
				binArrr[j]=1;
		}

		printf("\n\nbinArr");
		print_arr(binArrr,8);

		sprintf(buffer2, "%c", c_func(dec_val(binArrr)));
		strcat(word,buffer2);
	}

	printf("\n\nReceived Word: %s",word);

	printf("\n\nAfter performing division, Remainder: %s", rem);


	if(send(sockid, word, sizeof(word), 0)>0)
			printf("\nMessage Sent");

	goto l;

	close(sockid);

}
