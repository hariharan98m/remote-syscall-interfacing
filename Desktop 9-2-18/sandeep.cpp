#include<iostream>
#include <time.h>
#include <pthread.h>
#include <assert.h>
using namespace std;

void *func1(void* msg);
void *func2(void* msg);
int a=5;
int b=4;
int main()
{
     pthread_t t1,t2;
     clock_t begin1,end1;
     double time1;
     clock_t begin2, end2;
     double time2;
     int  t1ret, t2ret;
     int a=5,b=4;
     char *message1 = "Thread1 is created successfully and addition of two nos is done";
     char *message2 = "Thread2 is created successfully and subtraction of two nos is done" ;
     begin1= clock();
     t1ret = pthread_create(&t1,NULL,func1,(void*)message1);
     end1= clock();
     begin2 = clock();
     t2ret = pthread_create(&t2,NULL,func2,(void*)message2);
     end2 = clock();
     pthread_join(t1,NULL);
     pthread_join(t2,NULL); 
     time1= (double)(end1­-  begin1)/CLOCKS_PER_SEC;
     //cout<<CLOCKS_PER_SEC;
      cout<<endl;
     cout<<"Time taken for thread 1: "<<time1<<" seconds";
     cout<<endl;
     time2 = (double)(end2 ­- begin2)/CLOCKS_PER_SEC;
      cout<<endl;
     cout<<"Time taken for thread 2: "<<time2<<" seconds";
     cout<<endl;
     cout<<"Thread 1 returns:"<<t1ret;
     cout<<endl;
     cout<<"Thread 2 returns:"<<t2ret;
     cout<<endl;
     return 0;
}
void *func1( void *msg)
{
     cout<<endl;
     char *message;
     message = (char*)msg;
     cout<<message<<endl;
     cout<<"ADDITION OPERATION OF 5 and 4: "<<a+b<<endl;
     //pthread_exit(0);//terminates the calling thread explicitly
}
void *func2( void *msg)
{
     cout<<endl;
     char *message;
     message = (char*)msg;
     cout<<message<<endl;
     cout<<"SUBTRACTION OPERATION OF 5 and 4: "<<a­-b<<endl;
     //pthread_exit(0);//terminates the calling thread explicitly
}
