#include<ml.h>
int main()
{
matrix m(5,3);
for(int i=0;i<5;++i)
{
for(int j=0;j<3;++j)
{
if(j==0) m[i][j]=1.0;
else if(j==2) m[i][j]=5.0;
}
}
m[0][1]=2.0;
m[1][1]=3.0;
m[2][1]=5.0;
m[3][1]=7.0;
m[4][1]=100.0;

cout<<"About to scale"<<endl;
matrix ss=scaler::robust_scaler(m);
cout<<"scaled successfully"<<endl;

//print
for(int i=0;i<5;++i)
{
for(int j=0;j<3;++j)
{
cout<<m[i][j]<<" ";
}
cout<<endl;
}


return 0;
}