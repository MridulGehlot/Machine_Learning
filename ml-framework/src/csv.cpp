#include<csv.h>
#include<stdlib.h>
#include<cstdint>
#include<ml_exception.h>
#include<matrix.h>
#include<fcntl.h>
#define BUFFER_SIZE 4096
void csv::load_x_y(string datasetFileName,matrix &x,matrix &y,uint64_t skipLines)
{
char str[51];
double value;
char ch;
FILE *f;
f=fopen(datasetFileName.c_str(),"rb"); //we will read raw byte by byte
if(f==nullptr) throw ml_exception("Files Does Not Exists\n");
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

void csv::load_csv(string filename,matrix &target,uint64_t header_lines)
{
if(filename.empty()) throw ml_exception("File Name Required");
int file_descriptor;
file_descriptor=open(filename.c_str(),O_RDONLY | O_BINARY);
if(file_descriptor<0) throw ml_exception(string("Unalbe to read file : ")+filename);
unsigned char buffer[BUFFER_SIZE];
int bytes_read,index;
int rows,columns;
int pos,skip;
rows=0;
columns=1;
pos=0;
skip=0;
while(true)
{
bytes_read=read(file_descriptor,buffer,BUFFER_SIZE);
if(bytes_read==0) break;
for(index=0;index<bytes_read;++index)
{
++pos;
if(buffer[index]==',' && rows==0) ++columns;
if(buffer[index]=='\n')
{
if(rows+1==header_lines) skip=pos;
++rows;
}
}
}
if(buffer[index-1]!='\n') ++rows;
if(header_lines>=rows)
{
close(file_descriptor);
throw ml_exception("Not Enough Data");
}
rows-=header_lines;
target.resize(rows,columns);
//now reset pointer and populate data
lseek(file_descriptor,skip,SEEK_SET);
int which_row,which_column;
which_row=0;
which_column=0;
char str[51];
int i=0;
double value;
while(true)
{
bytes_read=read(file_descriptor,buffer,BUFFER_SIZE);
if(bytes_read==0) break;
for(index=0;index<bytes_read;++index)
{
if(buffer[index]==',')
{
str[i]='\0';
value=atof(str);
i=0;
target.set(which_row,which_column,value);
++which_column;
}
else if(buffer[index]=='\n')
{
str[i]='\0';
value=atof(str);
i=0;
target.set(which_row,which_column,value);
which_column=0;
++which_row;
}
else str[i++]=buffer[index];
}
}
if(buffer[index-1]!='\n')
{
str[i]='\0';
value=atof(str);
i=0;
target.set(which_row,which_column,value);
}
close(file_descriptor);
}