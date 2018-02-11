#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>          /* See NOTES */
#include<sys/socket.h>
#include<time.h>
#include<netinet/in.h>


void error(const char* err){
	printf("%s", err);
	exit(0);
}


#include<stdio.h>
#include<string.h>

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
	
	l:

	printf("\n\n\nEnter num of Characters in message:");
	scanf("%d", &charSize);

	printf("Enter dataword STRING:");	
	scanf("%s",sentWord);

	int j=0;
	dwSize=8*charSize;
	int *binArr;
	int index;

	for(i=0;i<charSize;i++){
		binArr= bin_val(char_val(sentWord[i]));
		printf("binArr: ");

		for(j=0;j<8;j++){
			index= i*8+j;
			//printf("index: %d", index);
			dw[index]=*(binArr+j);
		}

	}
	
//	for(i=0;i<dwSize;i++)
//		scanf("%d", &dw[i]);


	for(i=0;i<genSize;i++)
		dw[i+dwSize]=0;

	printf("DataWord: ");
	print_arr(dw,dwSize+genSize-1);	


	int *ptr;

	for(i=0;i<genSize;i++)
		if(i<dwSize)
			div[i]=dw[i];
		else
			div[i]=0;
	
	printf("Before Starting: Div= ");
	print_arr(div,genSize);	


	for(i=0;i<=dwSize-1;i++){
		int j;
		printf("Step: %d\n",i);

		printf("Before XOR, Div= ");
		print_arr(div,genSize);	


		if(div[0]==0){
			ptr=xor(div,zeroword);
			for(j=0;j<genSize-1;j++)
				div[j]=*(ptr+j);
			}
		else{
			ptr=xor(div,gen);
			for(j=0;j<genSize-1;j++)
				div[j]=*(ptr+j);
		}

		div[genSize-1]=dw[i+genSize];

		printf("After XOR: New Div= ");
		print_arr(div,genSize);	

	}
	
	printf("Resultant CRC word:");
	for(i=0;i<genSize-1;i++){

		dw[i+dwSize]=div[i];
		printf("%d",div[i]);
	}

	int NewSize=dwSize+ genSize-1; 
	char OutputDataWord[100];
	strcpy(OutputDataWord, "");
	char buffer2[1];

	printf("Resultant Encoded Data word:");
	for(i=0;i<NewSize;i++){
		sprintf(buffer2, "%d", dw[i]);
		strcat(OutputDataWord,buffer2);
		printf("%d",dw[i]);
	}

	printf("\nAs String: %s", OutputDataWord);

	if(send(sd2, OutputDataWord, NewSize, 0)>0)
		printf("\n\nMessage Sent");
	
	//printf("i am here");

	if(recv(sd2, buffer, sizeof(buffer), 0))
		printf("\nServer: %s", buffer);
	
	goto l;
			
	close(sockid);
	close(sd2);	
}
