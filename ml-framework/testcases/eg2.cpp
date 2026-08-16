#include<ml.h>
int main()
{
matrix m;
csv::load_csv("IceCreamSales.csv",m,1);
cout<<"Number of Rows : "<<m.rows()<<endl;
cout<<"Number of Columns : "<<m.columns()<<endl;
for(int i=0;i<m.rows();++i)
{
for(int j=0;j<m.columns();++j)
{
cout<<m.get(i,j)<<" ";
}
cout<<endl;
}
return 0;
}