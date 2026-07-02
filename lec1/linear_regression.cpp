#include<iostream>
#include<stdlib.h>
using namespace std;
int main()
{
char ch;
double value;
char str[51];
int i,x_rows,y_rows,x_columns,y_columns;
int dataset_row_count,dataset_column_count;
int which_row_from_dataset,which_column_from_dataset;
double *x,*y,*model;
FILE *f;
f=fopen("IceCreamSales.csv","rb"); //we will read raw byte by byte
if(f==nullptr)
{
printf("Files Does Not Exists\n");
return 1;
}
//step1 was to count number of rows and columns in dataset
dataset_column_count=1;
while(1)
{
ch=fgetc(f);
if(feof(f)) break;
if(ch==',') ++dataset_column_count;
if(ch=='\n') break;
}
dataset_row_count=0;
while(1)
{
ch=fgetc(f);
if(feof(f)) break;
if(ch=='\n') ++dataset_row_count;
}
fseek(f,-1,2);
ch=fgetc(f);
if(ch!='\n') ++dataset_row_count;
rewind(f);

x_rows=dataset_row_count;
x_columns=dataset_column_count;
y_rows=dataset_row_count;
y_columns=1;

//step2 allocate memory for x and y 
// and fill x and y from dataset
x=(double *)malloc(sizeof(double)*(x_rows*x_columns));
if(x==nullptr)
{
fclose(f);
cout<<"Not Enough Memory"<<endl;
return 1;
}
y=(double *)malloc(sizeof(double)*y_rows);
if(y==nullptr)
{
free(x);
fclose(f);
cout<<"Not Enough Memory"<<endl;
return 1;
}

while(1)
{
ch=fgetc(f);
if(feof(f)) break;
if(ch=='\n') break;
}
which_row_from_dataset=0;
which_column_from_dataset=0;
i=0;
while(1)
{
ch=fgetc(f);
if(feof(f)) break;
if(ch=='\r') continue;
if(ch==',' || ch=='\n')
{
str[i]='\0';
i=0;
value=atof(str);
if(which_column_from_dataset!=dataset_column_count-1) //feature
{
//x[row][col+1]=value;
x[which_row_from_dataset*x_columns+(which_column_from_dataset+1)]=value;
++which_column_from_dataset;
}
else //it is out target column
{
y[which_row_from_dataset]=value;
x[which_row_from_dataset*x_columns+0]=1.0;
++which_row_from_dataset;
which_column_from_dataset=0;
}
}
else str[i++]=ch;
}

//This is dirty work should not do
str[i]='\0';
value=atof(str);
y[which_row_from_dataset]=value;
x[which_row_from_dataset*x_columns+0]=1.0;

for(int ri=0;ri<x_rows;++ri)
{
for(int ci=0;ci<x_columns;++ci)
{
//cout<<x[ri][ci]<<" ";
cout<<x[ri*x_columns+ci]<<" ";
}
cout<<y[ri]<<endl;
}

cout<<dataset_row_count<<","<<dataset_column_count<<endl;
if(x!=NULL) free(x);
fclose(f);
return 0;
}