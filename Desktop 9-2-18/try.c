#include<stdio.h>

int main(){

int p=fork();
char snum[30];

if(p==0){
	printf("in child");
	execv("/bin/ls");
}else{
	printf("in parent");

}
return 0;

}
