#include<iostream>
#include<stdlib.h>
#include<time.h>
using namespace std;
int main()
{
srand(time(NULL)); //seed
int min,max;
min=0;
max=1;
cout<<min+(((double)rand()/RAND_MAX)*(max-min))<<endl;
return 0;
}