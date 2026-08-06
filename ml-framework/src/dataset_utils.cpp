#include<dataset_utils.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<matrix.h>
#include<stdlib.h>
#include<vector>
#include<ml_exception.h>
#include<random>
#include<algorithm>
#include<unordered_set>
#define BUFFER_SIZE 4096
using namespace std;

void dataset_utils::split(string dataset_filename,uint8_t split_percentage,uint64_t header_lines,matrix &matrix_1,matrix &matrix_2)
{
int bytes_fetched,index,comma_count,lines_count,rows,columns;
int file_descriptor,matrix_1_rows,matrix_2_rows;
unsigned char buffer[BUFFER_SIZE];
int skip_lines=header_lines;
int split_1_percentage=split_percentage;
int pos,first_byte_of_data;
int which_column;
int index_to_place_at,v_index;
double value;
char str[101];
char last_char;
int i=0;

struct stat s;
if(stat(dataset_filename.c_str(),&s)<0) throw ml_exception(string("Unable To Open : ")+dataset_filename);
if(split_1_percentage<=0 || split_1_percentage>=100) throw ml_exception("Invalid Split Percentage");
file_descriptor=open(dataset_filename.c_str(),O_RDONLY | O_BINARY);
if(file_descriptor<0) throw ml_exception(string("Unable to open file : ")+dataset_filename);
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
if(lines_count<skip_lines) 
{
close(file_descriptor);
throw ml_exception("Not Enough Data or Invalid Header Count");
}

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
}

void dataset_utils::split(string filename,uint8_t split_percentage,uint64_t header_lines,const string &file1,const string &file2)
{
if(split_percentage<=0 || split_percentage>=100)
{
throw ml_exception("Invalid Split Percentage");
}
int file_descriptor,fd1,fd2;
int bytes_fetched,index;

struct stat s;
if(stat(filename.c_str(),&s)<0) throw ml_exception("Invalid File Size");
file_descriptor=open(filename.c_str(),O_RDONLY | O_BINARY);
if(file_descriptor<0) throw ml_exception(string("Unable To Open File : ")+filename);
fd1=open(file1.c_str(),O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, S_IREAD | S_IWRITE);
if(file_descriptor<0) throw ml_exception(string("Unable To Create File : ")+file1);
fd2=open(file2.c_str(),O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, S_IREAD | S_IWRITE);
if(file_descriptor<0) throw ml_exception(string("Unable To Create File : ")+file2);

unsigned char buffer[BUFFER_SIZE];
vector<int> lengths;
int len=0;
int fattest_line=0;
while(1)
{
bytes_fetched=read(file_descriptor,buffer,BUFFER_SIZE);
if(bytes_fetched==0) break;
for(index=0;index<bytes_fetched;++index)
{
++len;
if(buffer[index]=='\n')
{
lengths.push_back(len);
if(len>fattest_line) fattest_line=len;
len=0;
}
}
}
if(buffer[index-1]!='\n') 
{
lengths.push_back(len);
if(len>fattest_line) fattest_line=len;
}
if(lengths.size()<=header_lines)
{
close(fd1);
close(fd2);
close(file_descriptor);
throw ml_exception("Invalid Header Count");
}
int file_1_rows,file_2_rows,rows_count;
rows_count=lengths.size()-header_lines;
file_1_rows=split_percentage*rows_count/100;
file_2_rows=rows_count-file_1_rows;
if(file_1_rows<=0 || file_2_rows<=0)
{
close(fd1);
close(fd2);
close(file_descriptor);
throw ml_exception("Invalid Split Percentage");
}
lseek(file_descriptor,0,SEEK_SET);
unsigned char *ptr;
ptr=(unsigned char *)malloc(sizeof(unsigned char)*fattest_line);
if(ptr==nullptr)
{
close(fd1);
close(fd2);
close(file_descriptor);
throw ml_exception("Not Enough Memory");
}
vector<int> v(rows_count);
for(index=0;index<rows_count;++index) v[index]=index;
random_device rd;
mt19937 engine(rd());
shuffle(v.begin(),v.end(),engine);
int place_at_index;
index=0;
for(auto len:lengths)
{
read(file_descriptor,ptr,len);
if(index<header_lines)
{
write(fd1,ptr,len);
write(fd2,ptr,len);
}
else
{
place_at_index=v[index-header_lines];
if(place_at_index<file_1_rows) write(fd1,ptr,len);
else write(fd2,ptr,len);
}
++index;
}
free(ptr);
close(fd1);
close(fd2);
close(file_descriptor);
}

void dataset_utils::remove_columns(string dataset,string filename,vector<uint32_t> &columns_index)
{
if(dataset.empty() || filename.empty()) throw ml_exception("File Name Required");
if(columns_index.empty()) throw ml_exception("Invalid Columns Selection");
int file_descriptor,wd;
struct stat s;
if(stat(dataset.c_str(),&s)<0) throw ml_exception("Invalid Dataset File Size");
file_descriptor=open(dataset.c_str(),O_RDONLY | O_BINARY);
if(file_descriptor<0) throw ml_exception("Unable to Open File");
unsigned char buffer[BUFFER_SIZE];
uint64_t bytes_fetched,index;
uint64_t column_count=1;
bool flag=false;
//apply validations
while(1)
{
bytes_fetched=read(file_descriptor,buffer,BUFFER_SIZE);
if(bytes_fetched==0) break;
for(index=0;index<bytes_fetched;++index)
{
if(buffer[index]==',')
{
++column_count;
}
else if(buffer[index]=='\n') 
{
flag=true;
break;
}
} //for loop ends here
if(flag) break;
} //while loop ends here
sort(columns_index.begin(),columns_index.end());
//remove duplicates
auto new_end=unique(columns_index.begin(),columns_index.end());
columns_index.erase(new_end,columns_index.end());
if(columns_index.size()==0 || columns_index[columns_index.size()-1]>=column_count)
{
close(file_descriptor);
throw ml_exception("Invalid Column Indexes");
}
unordered_set<uint32_t> st(columns_index.begin(),columns_index.end());
lseek(file_descriptor,0,0);
wd=open(filename.c_str(),O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, S_IREAD | S_IWRITE);
if(wd<0)
{
close(file_descriptor);
throw ml_exception(string("Cannot Create File : ")+filename);
}
column_count=0;
uint32_t last_index;
//erase columns and write in file
while(1)
{
bytes_fetched=read(file_descriptor,buffer,BUFFER_SIZE);
if(bytes_fetched==0) break;
last_index=0;
flag=false;
for(index=0;index<bytes_fetched;++index)
{
if(buffer[index]==',')
{
if(st.count(column_count)==false) 
{
if(flag) write(wd,",",1);
write(wd,buffer+last_index,index-last_index);
flag=true;
}
last_index=index+1;
++column_count;
}
else if(buffer[index]=='\n') 
{
if(st.count(column_count)==false)
{
if(flag) write(wd,",",1);
write(wd,buffer+last_index,index-last_index+1);
}
else if(flag) write(wd,"\n",1);
flag=false;
last_index=index+1;
column_count=0;
}
} //for loop ends here
if(index!=last_index && st.count(column_count)==false) 
{
if(flag) write(wd,",",1);
write(wd,buffer+last_index,index-last_index);
flag=false;
}
} //while loop ends here
close(wd);
close(file_descriptor);
}

void dataset_utils::remove_columns(string dataset,string filename,vector<string> &columns_name)
{
if(dataset.empty() || filename.empty()) throw ml_exception("File Name Required");
if(columns_name.empty()) throw ml_exception("No Columns Name Specified To Remove");
int file_descriptor;
file_descriptor=open(dataset.c_str(),O_RDONLY | O_BINARY);
if(file_descriptor<0) throw ml_exception(("Unable To Open File")+dataset);
auto new_end=unique(columns_name.begin(),columns_name.end());
columns_name.erase(new_end,columns_name.end());
for(std::string &str:columns_name)
{
for(char &c:str)
{
c = std::tolower(static_cast<unsigned char>(c));
}
}
unordered_set<string> st(columns_name.begin(),columns_name.end());
vector<uint32_t> columns_index;
uint32_t column_index=0;
unsigned char buffer[BUFFER_SIZE];
int bytes_fetched,last_index,index;
bool flag=false;
last_index=0;
string prev="";
while(1)
{
bytes_fetched=read(file_descriptor,buffer,BUFFER_SIZE);
if(bytes_fetched==0) break;
last_index=0;
for(index=0;index<bytes_fetched;++index)
{
if(buffer[index]==',')
{
string str(reinterpret_cast<const char*>(buffer+last_index),index-last_index);
str=prev+str;
prev="";
std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {return std::tolower(c);});
if(st.count(str))
{
columns_index.push_back(column_index);
}
++column_index;
last_index=index+1;
}
else if(buffer[index]=='\n')
{
string str(reinterpret_cast<const char*>(buffer+last_index),index-last_index);
str=prev+str;
if(!str.empty() && str.back() == '\r')
{
str.pop_back();
}
prev="";
std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {return std::tolower(c);});
if(st.count(str))
{
columns_index.push_back(column_index);
}
++column_index;
last_index=index+1;
flag=true;
break;
}
}
if(flag) break;
if(index!=last_index)
{
string s(reinterpret_cast<const char*>(buffer+last_index),index-last_index);
prev=s;
}
}
close(file_descriptor);
remove_columns(dataset,filename,columns_index);
}