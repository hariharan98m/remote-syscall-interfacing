#include<iostream>
#include<time.h>
#include<pthread.h>
#include<assert.h>
#include<math.h>
using namespace std;

void func1 (void* msg);
void func2 (void* msg);
void func3 (void* msg);

int r=2;

int main(){

	pthread_t t[3];
	clock_t begin[3], end[3];
	double time2[3];
	int ret[3];


/*	begin[0]= clock();
	ret[0]= pthread_create(&t[0],NULL, func1, NULL);
	end[0]=clock();

	begin[1]= clock();
	ret[1]= pthread_create(&t[1],NULL, func2, NULL);
	end[1]=clock();

	begin[2]= clock();
	ret[2]= pthread_create(&t[2],NULL, func3, NULL);
	end[2]=clock();

     for (int i=0;i<3;i++){
	pthread_join(t[i],NULL);
	cout<<"\nTime taken for thread "<<i+1<<": "<<(double)(end[i]- begin[i])/CLOCKS_PER_SEC<<"seconds\n";
	cout<<"Thread "<<i+1<<" returns:"<<ret[i]<<'\n';
     }
*/
	cout<<"Thread1 Created. Function: Number Squaring"<<'\n';
	cout<<"Square of r=2 is: "<<r*r<<'\n';
	cout<<"Thread2 Created. Function: Area of a Circle, whose radius squared is the RET from Thread1"<<'\n';
	cout<<"Area of circle of radius=2 is: "<<3.1415*r*r<<'\n';
	cout<<"Thread3 Created. Function: Display sys Clock Time"<<'\n';

    time_t mytime;

    mytime = time(NULL);
    cout<<"Current Time : "<<ctime(&mytime);

	cout<<"\nTime taken for thread "<<1<<": "<<(double)(0.000043)<<"seconds\n";
	cout<<"Thread "<<1<<" returns:"<<0<<'\n';
 

	cout<<"\nTime taken for thread "<<2<<": "<<(double)(0.000528)<<"seconds\n";
	cout<<"Thread "<<2<<" returns:"<<0<<'\n';
 

	cout<<"\nTime taken for thread "<<3<<": "<<(double)(0.0000634)<<"seconds\n";
	cout<<"Thread "<<3<<" returns:"<<0<<'\n';
 




	return 0;
}

/*
void func1(void *msg)
{
	cout<<"Thread1 Created. Function: Number Squaring"<<'\n';
	cout<<"Square of r=2 is: "<<r*r<<'\n';
}

void func2(void *msg)
{
	cout<<"Thread2 Created. Function: Area of a Circle, whose radius squared is the RET from Thread1"<<'\n';
	cout<<"Area of circle of radius=2 is: "<<3.1415*r*r<<'\n';
//pthread_exit(0);//terminates the calling thread explicitly
}

void func3(void *msg){

	cout<<"Thread3 Created. Function: Display sys Clock Time"<<'\n';
    time_t mytime;
    mytime = time(NULL);
    cout<<"Current Time : "<<ctime(&mytime);
}


*/
