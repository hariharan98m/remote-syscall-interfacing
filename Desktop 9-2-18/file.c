#include<stdio.h>
#include<stdlib.h>
#include<signal.h>

void interruptChange()
{
    signal(SIGINT, interruptChange);
    printf("\nctrl - C pressed\n");
}
void interruptExit()
{
    printf("\nCtrl - \\ pressed\n");
    exit(EXIT_SUCCESS);
}
int main()
{
    signal(SIGINT, interruptChange);
    signal(SIGQUIT,interruptExit);
    printf("\nCtrl - C is disabled. Use Ctrl - \\ to quit\n");
    while(1){}
}

