#include<csv.h>
#include<stdlib.h>
#include<cstdint>
#include<ml_exception.h>
#include<matrix.h>
void csv::load_x_y(string datasetFileName,matrix &x,matrix &y,uint64_t skipLines)
{
char str[51];
double value;
char ch;
FILE *f;
f=fopen(datasetFileName.c_str(),"rb"); //we will read raw byte by byte
if(f==nullptr) throw new ml_exception("Files Does Not Exists\n");
//step1 was to count number of rows and columns in dataset
uint64_t dataset_column_count,dataset_row_count;
uint64_t which_column_from_dataset,which_row_from_dataset;
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
dataset_row_count-=(skipLines-1); //bcz i already skipped 1 so -1
rewind(f);

uint64_t x_rows=dataset_row_count;
uint64_t x_columns=dataset_column_count;
uint64_t y_rows=dataset_row_count;
uint64_t y_columns=1;

//step2 allocate memory for x and y 
// and fill x and y from dataset
x.resize(x_rows,x_columns);
y.resize(y_rows,y_columns);

uint64_t ii=1;
while(ii<=skipLines)
{
while(1)
{
ch=fgetc(f);
if(feof(f)) break;
if(ch=='\n') 
{
++ii;
break;
}
}
}

which_row_from_dataset=0;
which_column_from_dataset=0;
int i=0;
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
//x[which_row_from_dataset*x_columns+(which_column_from_dataset+1)]=value;
x.collection[which_row_from_dataset*x_columns+(which_column_from_dataset+1)]=value;
//x.set(which_row_from_dataset,which_column_from_dataset+1,value);
++which_column_from_dataset;
}
else //it is out target column
{
//y[which_row_from_dataset]=value;
y.collection[which_row_from_dataset]=value;
//y.set(which_row_from_dataset,0,value);
//x[which_row_from_dataset*x_columns+0]=1.0;
x.collection[which_row_from_dataset*x_columns+0]=1.0;
//x.set(which_row_from_dataset,0,1.0);
++which_row_from_dataset;
which_column_from_dataset=0;
}
}
else str[i++]=ch;
}

//This is dirty work should not do
str[i]='\0';
fseek(f,-1,2);
ch=fgetc(f);
if(ch!='\n')
{
value=atof(str);
//y[which_row_from_dataset]=value;
y.collection[which_row_from_dataset]=value;
//y.set(which_row_from_dataset,0,value);
//x[which_row_from_dataset*x_columns+0]=1.0;
x.collection[which_row_from_dataset*x_columns+0]=1.0;
//x.set(which_row_from_dataset,0,1.0);
}

fclose(f);
}
