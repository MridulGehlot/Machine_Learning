#include<iostream>
#include<stdlib.h>
#define TRAINING_CYCLES 100000
#define LEARNING_RATE 0.0001
#define STOPPER 10000
#include<chrono>
using namespace std;
int main()
{
char ch;
double value;
char str[51];
int i,x_rows,y_rows,x_columns,y_columns;
int dataset_row_count,dataset_column_count;
int which_row_from_dataset,which_column_from_dataset;
double *x,*y,*model,*y_hat,*error;
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
fclose(f);

/*
for(int ri=0;ri<x_rows;++ri)
{
for(int ci=0;ci<x_columns;++ci)
{
//cout<<x[ri][ci]<<" ";
cout<<x[ri*x_columns+ci]<<" ";
}
cout<<y[ri]<<endl;
}
*/
//cout<<dataset_row_count<<","<<dataset_column_count<<endl;

//step 3 
//compute x_transpose here
double * x_transpose=(double *)malloc(sizeof(double)*(x_rows*x_columns));
if(x_transpose==nullptr)
{
free(x);
free(y);
cout<<"Not Enough Memory"<<endl;
return 1;
}

for(int i=0;i<x_rows;++i)
{
for(int j=0;j<x_columns;++j)
{
x_transpose[j*x_rows+i]=x[i*x_columns+j];
}
}

model=(double *)malloc(sizeof(double)*(x_columns));
if(model==nullptr)
{
free(x);
free(y);
free(x_transpose);
cout<<"Not Enough Memory"<<endl;
return 1;
}

double * xte=(double *)malloc(sizeof(double)*(x_columns));
if(xte==nullptr)
{
free(x);
free(y);
free(x_transpose);
free(model);
cout<<"Not Enough Memory"<<endl;
return 1;
}

y_hat=(double *)malloc(sizeof(double)*(y_rows));
if(y==nullptr)
{
free(x);
free(y);
free(x_transpose);
free(model);
free(xte);
cout<<"Not Enough Memory"<<endl;
return 1;
}

error=(double *)malloc(sizeof(double)*(y_rows));
if(error==nullptr)
{
free(x);
free(y);
free(x_transpose);
free(model);
free(xte);
free(y_hat);
cout<<"Not Enough Memory"<<endl;
return 1;
}


int left_operand_rows,left_operand_columns,right_operand_rows,right_operand_columns;

auto start_time=chrono::high_resolution_clock::now();

//step 4 train model
int cycle=1;
double squared_sum_error;
while(cycle<=TRAINING_CYCLES)
{
//y_hat=x*model;
left_operand_rows=x_rows;
left_operand_columns=x_columns;
right_operand_rows=x_columns;
right_operand_columns=1;
for(int i=0;i<left_operand_rows;++i)
{
for(int j=0;j<right_operand_columns;++j)
{
value=0.0;
for(int k=0;k<left_operand_columns;++k)
{
//value+=x[i][k]*model[k][j];
value+=x[i*x_columns+k]*model[k*1+j];
}
//y_hat[i][j]=value;
y_hat[i*y_columns+j]=value;
}
}

//compute error
for(int i=0;i<x_rows;++i) error[i]=y_hat[i]-y[i];

//e_t * e
left_operand_rows=y_columns;
left_operand_columns=y_rows;
right_operand_rows=y_rows;
right_operand_columns=y_columns;
for(int i=0;i<left_operand_rows;++i)
{
for(int j=0;j<right_operand_columns;++j)
{
value=0.0;
for(int k=0;k<left_operand_columns;++k)
{
value+=error[i*left_operand_columns+k]*error[k*right_operand_columns+j];
}
squared_sum_error=value;
}
}

//apply modified MSE
double cost=squared_sum_error/(2*x_rows);
if(cycle%STOPPER==0) cout<<cost<<endl;

//calculate xte = x_transpose * error
left_operand_rows=x_columns;
left_operand_columns=x_rows;
right_operand_rows=y_rows;
right_operand_columns=y_columns;
for(int i=0;i<left_operand_rows;++i)
{
for(int j=0;j<right_operand_columns;++j)
{
value=0.0;
for(int k=0;k<left_operand_columns;++k)
{
value+=x_transpose[i*left_operand_columns+k]*error[k*right_operand_columns+j];
}
xte[i*right_operand_columns+j]=value;
}
}

//update weights
//model = model - (((x_transposed*error)*(LEARNING_RATE))/x_rows);
//model = model - ((xte[i]*LEARNING_RATE)/x_rows);
for(int i=0;i<x_columns;++i)
{
for(int j=0;j<y_columns;++j)
{
//model[i][j]-=((xte[i][j]*LEARNING_RATE)/x_rows);
model[i*y_columns+j]-=((xte[i*y_columns+j]*LEARNING_RATE)/x_rows);
}
}

++cycle;
}

auto end_time=chrono::high_resolution_clock::now();
chrono::duration<double> ttime=end_time-start_time;
cout<<"Time in Training - "<<ttime.count()<<endl;

if(x!=NULL) free(x);
if(y!=NULL) free(y);
if(model!=NULL) free(model);
if(xte!=NULL) free(xte);
if(y_hat!=NULL) free(y_hat);
if(error!=NULL) free(error);
if(x_transpose!=NULL) free(x_transpose);
return 0;
}