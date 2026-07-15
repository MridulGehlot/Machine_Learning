#include<stdio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<matrix.h>
#include<stdlib.h>
#include<ml_exception.h>
#include<random>
#include<algorithm>
#define BUFFER_SIZE 1024
using namespace std;
int main()
{
string dataset_filename="IceCreamSales2.csv";
matrix matrix_1;
matrix matrix_2;
int bytes_fetched,index,comma_count,lines_count,rows,columns;
int file_descriptor,matrix_1_rows,matrix_2_rows;
unsigned char buffer[BUFFER_SIZE];
int skip_lines=1;
int split_1_percentage=70;
int pos,first_byte_of_data;
int which_column;
int index_to_place_at,v_index;
double value;
char str[101];
char last_char;
int i=0;

//below code is reusable
struct stat s;
if(stat(dataset_filename.c_str(),&s)<0)
{
throw ml_exception(string("Unable To Open : ")+dataset_file_name);
return EXIT_FAILURE;
}
if(split_1_percentage<=0 || split_1_percentage>=100) throw ml_exception("Invalid Split Percentage");
file_descriptor=open(dataset_filename.c_str(),O_RDONLY | O_BINARY);
if(file_descriptor<0)
{
printf("Unable to open file : %s\n",dataset_filename.c_str());
return EXIT_FAILURE;
}
comma_count=1;
lines_count=0;
pos=0;
first_byte_of_data=0;
while(1)
{
bytes_fetched=read(file_descriptor,buffer,BUFFER_SIZE);
if(bytes_fetched==0) break;
for(index=0;index<bytes_fetched;++index)
{
++pos;
if(buffer[index]=='\n')
{
++lines_count;
if(lines_count==skip_lines) first_byte_of_data=pos;
}
if(buffer[index]==',')
{
if(lines_count==skip_lines) ++comma_count;
}
} //for loop ends here
} //while(1) ends here

//handle edge case of last \n
last_char=(char)0;
last_char=buffer[index-1];
if(last_char!='\n') ++lines_count;

rows=lines_count-skip_lines;
columns=comma_count;
matrix_1_rows=(split_1_percentage*rows)/100;
matrix_2_rows=rows-matrix_1_rows;
if(matrix_1_rows==0 || matrix_2_rows==0) throw ml_exception("Invalid Split Percentage");
matrix_1._rows=matrix_1_rows;
matrix_1._columns=columns;
matrix_1.collection.resize(matrix_1_rows*columns);
matrix_2._rows=matrix_2_rows;
matrix_2._columns=columns;
matrix_2.collection.resize(matrix_2_rows*columns);
//to move file pointer to 1st byte of data
lseek(file_descriptor,first_byte_of_data,SEEK_SET);

//generate shuffle to split
random_device rd;
mt19937 engine(rd());
vector<int> v(rows);
for(index=0;index<rows;++index) v[index]=index;
shuffle(v.begin(),v.end(),engine);

//read and fill data
which_column=0;
i=0;
v_index=0;
index_to_place_at=v[v_index];
while(1)
{
bytes_fetched=read(file_descriptor,buffer,BUFFER_SIZE);
if(bytes_fetched==0) break;
for(index=0;index<bytes_fetched;++index)
{
if(buffer[index]==',')
{
str[i]='\0';
i=0;
value=atof(str);
if(index_to_place_at<matrix_1_rows) //put in matrix1
{
matrix_1.collection[index_to_place_at*columns+which_column]=value;
}
else //put in matrix2
{
matrix_2.collection[(index_to_place_at-matrix_1_rows)*columns+which_column]=value;
}
++which_column;
}
else if(buffer[index]=='\n')
{
str[i]='\0';
i=0;
value=atof(str);
if(index_to_place_at<matrix_1_rows) //put in matrix1
{
matrix_1.collection[index_to_place_at*columns+which_column]=value;
}
else //put in matrix2
{
matrix_2.collection[(index_to_place_at-matrix_1_rows)*columns+which_column]=value;
}
which_column=0;
++v_index;
if(v_index<v.size()) index_to_place_at=v[v_index];
}
else str[i++]=buffer[index];
} //for loop ends here
} //while(1) ends here
if(last_char!='\n')
{
str[i]='\0';
i=0;
value=atof(str);
if(index_to_place_at<matrix_1_rows) //put in matrix1
{
matrix_1.collection[index_to_place_at*columns+which_column]=value;
}
else //put in matrix2
{
matrix_2.collection[(index_to_place_at-matrix_1_rows)*columns+which_column]=value;
}
}

close(file_descriptor);
return EXIT_SUCCESS;
}