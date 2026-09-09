#include<ml.h>
int main()
{
matrix m(4,3);

/*
18,  50000, 3.0
35,  75000, 1.0
45, 120000, 5.0
27,  35000, 2.0
*/
m[0][0]=18;
m[0][1]=50000;
m[0][2]=3.0;

m[1][0]=35;
m[1][1]=75000;
m[1][2]=1.0;

m[2][0]=45;
m[2][1]=120000;
m[2][2]=5.0;

m[3][0]=27;
m[3][1]=35000;
m[3][2]=2.0;


cout<<"About to scale"<<endl;
matrix ss=scaler::standard_scaler(m);
cout<<"scaled successfully"<<endl;

//print
for(int i=0;i<4;++i)
{
for(int j=0;j<3;++j)
{
cout<<m[i][j]<<" ";
}
cout<<endl;
}


return 0;
}