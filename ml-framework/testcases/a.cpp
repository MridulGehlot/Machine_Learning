#include<iostream>
#include<ml.h>
using namespace std;
int main()
{
matrix a(3,3);
a-=2;
for(int i=0;i<a.rows();++i)
{
for(int j=0;j<a.columns();++j)
{
cout<<a[i][j]<<" ";
}
cout<<endl;
}
return 0;
}