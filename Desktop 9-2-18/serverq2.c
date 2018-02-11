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



	printf("DataWord: ");
	print_arr(dw,dwSize);	


	int *ptr;

	for(i=0;i<8;i++)
		div[i]=dw[i];
		
	printf("Before Starting: Checksum= ");
	print_arr(div,8);	

	for(i=0;i<=charSize-2;i++){
		int j;
		printf("Step: %d\n",i);

		printf("Before XOR, Checksum= ");
		print_arr(div,8);	

		for(j=0;j<8;j++)
			gen[j]=dw[(i+1)*8+j];

		printf("XOR word= ");
		print_arr(gen,8);	

		ptr=xor(div,gen);

		for(j=0;j<genSize-1;j++)
			div[j]=*(ptr+j);

		printf("After XOR: New Div= ");
		print_arr(div,8);	

	}
	
	printf("Resultant Checksum bits:");
	for(i=0;i<8;i++){
		dw[i+dwSize]=div[i];
		printf("%d", div[i]);
	}

	int NewSize=dwSize+8; 
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
		printf("\nClient: %s", buffer);
	
	goto l;
			
	close(sockid);
	close(sd2);	
}
