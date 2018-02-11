#include<stdio.h>

extern int addNumbers(int, int);

int main(){

	int x=9;
	int y=4;
	printf("Sum of %d and %d is %d", x, y, addNumbers(x,y));

}
