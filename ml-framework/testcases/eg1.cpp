#include<iostream>
#include<matrix.h>
#include<cstdint>
#include<chrono>
using namespace std;
int main()
{
uint64_t rows,cols;
rows=3000;
cols=2000;
matrix a(rows,cols);
matrix b(cols,rows);
matrix c;
double value=1.0;
for(uint64_t i=0;i<rows;i++)
{
for(uint64_t j=0;j<cols;j++)
{
a.set(i,j,value);
++value;
}
}

value=1.0;
for(uint64_t i=0;i<cols;i++)
{
for(uint64_t j=0;j<rows;j++)
{
b.set(i,j,value);
++value;
}
}



auto start_time=chrono::high_resolution_clock::now();
c=a*b;
auto end_time=chrono::high_resolution_clock::now();
chrono::duration<double> tt=end_time-start_time;
cout<<"Total Time In Seconds : "<<tt.count()<<endl;

/*
for(uint64_t i=0;i<3;i++)
{
for(uint64_t j=0;j<3;j++)
{
cout<<c.get(i,j)<<" ";
}
cout<<endl;
}
*/

return 0;
}