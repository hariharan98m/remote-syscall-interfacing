#include<iostream>
#include<time.h>
#include<pthread.h>
#include<assert.h>
#include<math.h>
using namespace std;

void *func1(void* msg);
void *func2(void* msg);
void *func3(void* msg);

int r=2;

int main()
{
     pthread_t t[3];	

     clock_t begin[3],end[3];
     double time[3];
     int ret[3];
     const char *message1 = "Thread1 Created. Function: Number Squaring";
     const  char *message2 = "Thread2 Created. Function: Area of a Circle, whose radius squared is the RET from Thread1" ;
     const char *message3 = "Thread3 Created. Function: Display sys Clock Time" ;
     
     begin[0]= clock();
     ret[0] = pthread_create(&t[0],NULL,func1,(void*)message1);
	end[0]= clock();

     begin[1]= clock();
     ret[1] = pthread_create(&t[0],NULL,func2,(void*)message2);
	end[1]= clock();

     begin[2]= clock();
     ret[2] = pthread_create(&t[0],NULL,func3,(void*)message3);
	end[2]= clock();
	
     for (int i=0;i<3;i++){
	pthread_join(t[i],NULL);
	cout<<"\nTime taken for thread "<<i+1<<": "<<(double)(end[i]­ - begin[i])/CLOCKS_PER_SEC<<" seconds\n";
	cout<<"Thread "<<i+1<<" returns:"<<ret[i]<<'\n';
     }
	return 0;

}

void *func1( void *msg)
{
	cout<<(char*)msg<<'\n';
cout<<"Square of r=2 is: "<<r*r<<'\n';
}

void *func2( void *msg)
{
	cout<<(char*)msg<<'\n';
	cout<<"Area of circle of radius=2 is: "<<pi*r*r<<'\n';
//pthread_exit(0);//terminates the calling thread explicitly
}

void *func3( void *msg)
{

	cout<<(char*)msg<<'\n';
    time_t mytime;
    mytime = time(NULL);
    cout<<"Current Time : "<<ctime(&mytime);

}

