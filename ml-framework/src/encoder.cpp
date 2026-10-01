#include<encoder.h>
#include<vector>
#include<ml_exception.h>
#include<unordered_set>
#include<set>
#include<unordered_map>
#include<map>
#include<sys/stat.h>
#include<fcntl.h>
#include<algorithm>

#define BUFFER_SIZE 4096

void encoder::one_hot_encode(string dataset,string filename,vector<string> &columns_name)
{
if(dataset.empty() || filename.empty()) throw ml_exception("File Name Required");
if(columns_name.empty()) throw ml_exception("Columns Name Required To Encode");
int file_descriptor,wd;
file_descriptor=open(dataset.c_str(),O_RDONLY | O_BINARY);
if(file_descriptor<0) throw ml_exception(string("Unable to Open File : ")+dataset);
unordered_set<string> columns_to_encode(columns_name.begin(),columns_name.end());
unordered_map<int,set<string>> ds;
unsigned char buffer[BUFFER_SIZE];
int bytes_read,index,last_index,column_index;
string prev="";
column_index=0;
bool flag=false;
while(true)
{
bytes_read=read(file_descriptor,buffer,BUFFER_SIZE);
if(bytes_read==0) break;
last_index=0;
for(index=0;index<bytes_read;++index)
{
if(buffer[index]==',')
{
string s(reinterpret_cast<const char*>(buffer+last_index),index-last_index);
s=prev+s;
if(!s.empty() && s.back() == '\r')
{
s.pop_back();
}
if(columns_to_encode.count(s)) ds[column_index]={};
last_index=index+1;
++column_index;
prev="";
}
if(buffer[index]=='\n') 
{
string s(reinterpret_cast<const char*>(buffer+last_index),index-last_index);
s=prev+s;
if(!s.empty() && s.back() == '\r')
{
s.pop_back();
}
if(columns_to_encode.count(s)) ds[column_index]={};
last_index=index+1;
column_index=0;
prev="";
flag=true;
break;
}
}//for loop
if(flag) break;
if(last_index!=index)
{
string tmp(reinterpret_cast<const char*>(buffer+last_index),index-last_index);
prev=tmp;
}
}//while loop

//now read data and find no. of classes for each column
//process remaining buffer
for(++index;index<bytes_read;++index)
{
if(buffer[index]==',')
{
string s(reinterpret_cast<const char*>(buffer+last_index),index-last_index);
s=prev+s;
if(!s.empty() && s.back() == '\r')
{
s.pop_back();
}
if(ds.count(column_index)) ds[column_index].insert(s);
last_index=index+1;
++column_index;
prev="";
}
if(buffer[index]=='\n') 
{
string s(reinterpret_cast<const char*>(buffer+last_index),index-last_index);
s=prev+s;
if(!s.empty() && s.back() == '\r')
{
s.pop_back();
}
if(ds.count(column_index)) ds[column_index].insert(s);
last_index=index+1;
column_index=0;
prev="";
}
}//for loop
if(last_index!=index)
{
string tmp(reinterpret_cast<const char*>(buffer+last_index),index-last_index);
prev=tmp;
}
//now read all remaining
while(true)
{
bytes_read=read(file_descriptor,buffer,BUFFER_SIZE);
if(bytes_read==0) break;
last_index=0;
for(index=0;index<bytes_read;++index)
{
if(buffer[index]==',')
{
string s(reinterpret_cast<const char*>(buffer+last_index),index-last_index);
s=prev+s;
if(!s.empty() && s.back() == '\r')
{
s.pop_back();
}
if(ds.count(column_index)) ds[column_index].insert(s);
last_index=index+1;
++column_index;
prev="";
}
if(buffer[index]=='\n') 
{
string s(reinterpret_cast<const char*>(buffer+last_index),index-last_index);
s=prev+s;
if(!s.empty() && s.back() == '\r')
{
s.pop_back();
}
if(ds.count(column_index)) ds[column_index].insert(s);
last_index=index+1;
column_index=0;
prev="";
}
}//for loop
if(last_index!=index)
{
string tmp(reinterpret_cast<const char*>(buffer+last_index),index-last_index);
prev=tmp;
}
}//while loop

//now filter dataset remove columns which have only 1 class
for(auto it = ds.begin(); it != ds.end(); )
{
if(it->second.size() == 1) it = ds.erase(it);
else  ++it;
}
if(ds.size()==0)
{
close(file_descriptor);
throw ml_exception("No Column To Encode");
}

//2nd part of code to write everything in file
lseek(file_descriptor,0,SEEK_SET);
wd=open(filename.c_str(),O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, S_IREAD | S_IWRITE);
if(wd<0)
{
close(file_descriptor);
throw ml_exception(string("Unable to Open File : ")+filename);
}

//now read and write
flag=false;
column_index=0;
prev="";
bool prev_written=false;
while(true)
{
bytes_read=read(file_descriptor,buffer,BUFFER_SIZE);
if(bytes_read==0) break;
last_index=0;
for(index=0;index<bytes_read;++index)
{
if(buffer[index]==',')
{
string str(reinterpret_cast<const char*>(buffer+last_index),index-last_index);
str=prev+str;
if(!str.empty() && str.back() == '\r')
{
str.pop_back();
}
if(ds.count(column_index)) 
{
if(flag==false) //means reading header
{
for(auto &s:ds[column_index])
{
if(prev_written) write(wd,",",1);
string header = str + "_" + s;
write(wd, header.c_str(), header.size());
prev_written=true;
}
}//header written
else //write encoded data
{
for(auto &s:ds[column_index])
{
if(prev_written) write(wd,",",1);
if(s==str) write(wd,"1",1);
else write(wd,"0",1);
prev_written=true;
}
}//write encoded data
}//if column to encode
else
{
if(prev_written) write(wd,",",1);
write(wd,str.c_str(),str.size());
prev_written=true;
}
prev="";
last_index=index+1;
prev_written=true;
++column_index;
}
if(buffer[index]=='\n')
{
string str(reinterpret_cast<const char*>(buffer+last_index),index-last_index);
str=prev+str;
if(!str.empty() && str.back() == '\r')
{
str.pop_back();
}
if(ds.count(column_index))
{
if(flag==false) //means reading header
{
for(auto &s:ds[column_index])
{
if(prev_written) write(wd,",",1);
string header = str + "_" + s;
write(wd, header.c_str(), header.size());
prev_written=true;
}
}//header written
else //write encoded data
{
for(auto &s:ds[column_index])
{
if(prev_written) write(wd,",",1);
if(s==str) write(wd,"1",1);
else write(wd,"0",1);
prev_written=true;
}
}//write encoded data
}//if column to encode
else
{
if(prev_written) write(wd,",",1);
write(wd,str.c_str(),str.size());
}
write(wd,"\n",1);
prev="";
last_index=index+1;
prev_written=false;
flag=true;
column_index=0;
}
}//for loop
if(last_index!=index)
{
string tmp(reinterpret_cast<const char*>(buffer+last_index),index-last_index);
prev=tmp;
}
}//while loop
if(prev.size()!=0)
{
write(wd,",",1);
write(wd,prev.c_str(),prev.size());
}
close(file_descriptor);
close(wd);
}
void encoder::one_hot_encode(string dataset,string filename,vector<uint64_t> &columns_index,bool has_header)
{
}

//target Encoder
void encoder::target_encode(string dataset,string filename,vector<string> &columns_name)
{
if(dataset.empty()) throw ml_exception("Dataset File Name Required");
if(filename.empty()) throw ml_exception("File Name Required");
if(columns_name.empty()) throw ml_exception("No Columns To Encode");
struct stat s;
if(stat(dataset.c_str(),&s)<0)
{
throw ml_exception("Dataset File Does Not Exists");
}
FILE *source_file;
source_file=fopen(dataset.c_str(),"rb");
if(source_file==nullptr)
{
throw ml_exception("Unable To Open Dataset File");
}

//variables for reading
set<string> st(columns_name.begin(),columns_name.end());
char buffer[BUFFER_SIZE];
uint64_t bytes_to_read,bytes_read,read_size,idx;
read_size=BUFFER_SIZE;
bytes_to_read=s.st_size;
string header="";
while(bytes_to_read>0)
{
if(bytes_to_read<read_size) read_size=bytes_to_read;
bytes_read=fread(buffer,sizeof(char),read_size,source_file);
bytes_to_read-=bytes_read;
for(idx=0;idx<bytes_read;++idx)
{
if(buffer[idx]=='\n')
{
header=header+string(buffer,idx);
break;
}
}//for loop ends
if(idx<bytes_read && buffer[idx]=='\n') break;
header+=string(buffer,bytes_read);
}//while loop ends
fclose(source_file);
vector<uint64_t> columns_index;
int column=0;
int last=0;
for(int i=0;i<header.size();++i)
{
if(header[i]==',')
{
string s=header.substr(last,i-last);
if(st.count(s)) columns_index.push_back(column);
++column;
last=i+1;
}
}
string tmp=header.substr(last);
while(tmp.back()=='\n' || tmp.back()=='\r') tmp.pop_back();
if(st.count(tmp)) columns_index.push_back(column);
if(columns_index.empty()) throw ml_exception("No Columns to encode");
encoder::target_encode(dataset,filename,columns_index,true);
}

void encoder::target_encode(string dataset,string filename,vector<uint64_t> &columns_index,bool has_header)
{
if(dataset.empty()) throw ml_exception("Dataset File Name Required");
if(filename.empty()) throw ml_exception("File Name Required");
if(columns_index.empty()) throw ml_exception("No Columns To Encode");
struct stat s;
if(stat(dataset.c_str(),&s)<0)
{
throw ml_exception("Dataset File Does Not Exists");
}
FILE *source_file;
source_file=fopen(dataset.c_str(),"rb");
if(source_file==nullptr)
{
throw ml_exception("Unable To Open Dataset File");
}
FILE *target_file;
target_file=fopen(filename.c_str(),"wb");
if(target_file==nullptr)
{
fclose(source_file);
throw ml_exception("Unable To Open Target File");
}
//variables for reading
vector<uint64_t> lines_info;
char buffer[BUFFER_SIZE];
uint64_t bytes_to_read,bytes_read,read_size,idx,column_count,sz,fattest_length;
uint64_t row,column;
read_size=BUFFER_SIZE;
bytes_to_read=s.st_size;
sz=0;
fattest_length=0;
row=0;
column=1;
int last=0;
while(bytes_to_read>0)
{
if(bytes_to_read<read_size) read_size=bytes_to_read;
bytes_read=fread(buffer,sizeof(char),read_size,source_file);
bytes_to_read-=bytes_read;
for(idx=0;idx<bytes_read;++idx)
{
++sz;
if(buffer[idx]==',') 
{
++column;
}
if(buffer[idx]=='\n')
{
if(row==0 && !has_header) column_count=column;
if(row==1 && has_header) column_count=column;
if(sz>fattest_length) fattest_length=sz;
lines_info.push_back(sz);
sz=0;
column=1;
++row;
}
}//for loop ends
}//while loop ends
if(sz!=0)
{
lines_info.push_back(sz);
sz=0;
}
//ds for encoding
double value;
int i,j;
char * line = new char [fattest_length+1];
char ** ptr = new char *[column_count];
sort(columns_index.begin(),columns_index.end());
auto it=unique(columns_index.begin(),columns_index.end());
columns_index.erase(it,columns_index.end());
for(auto x:columns_index)
{
if(x>=column_count)
{
fclose(source_file);
fclose(target_file);
throw ml_exception("Invalid Column Index To Encode : "+to_string(x));
}
if(x==column_count-1)
{
fclose(source_file);
fclose(target_file);
throw ml_exception("Invalid Column Index Target Column Can't be Encoded and it should be numeric");
}
}
map<string,double> map_of_sum;
map<string,int> map_of_count;
rewind(source_file);
idx=0;
while(idx<lines_info.size())
{
sz=lines_info[idx];
fread(line,sizeof(char),sz,source_file);
if(has_header && idx==0) 
{
++idx;
continue;
}
//process line
j=sz-1;
while(line[j]=='\n' || line[j]=='\r') --j;
line[j+1]='\0';
j=0;
ptr[j++]=&line[0];
for(i=0;line[i]!='\0';++i)
{
if(line[i]==',')
{
line[i]='\0';
ptr[j++]=&line[i+1];
}
}
//now i have splits ready
string target(ptr[column_count-1]);
//validate s the target column
for(char ch:target)
{
if(!((ch>='0' && ch<='9') || ch=='.'))
{
fclose(source_file);
fclose(target_file);
throw ml_exception("Target Column Must be Numeric");
}
}
value=stod(target);
int k=0;
for(i=0;i<column_count-1;++i)
{
string s(ptr[i]);
if(columns_index[k]==i)
{
if(map_of_sum.count(s)==false) map_of_sum[s]=0;
map_of_sum[s]+=value;
map_of_count[s]++;
if(k<columns_index.size()) ++k;
}
}
++idx;
}
//now the ds is ready we just have to write everything in file
//write header

rewind(source_file);
sz=lines_info[0];
fread(line,sizeof(char),sz,source_file);
if(has_header) fwrite(line,sizeof(char),sz,target_file);

//write data
idx=1;
while(idx<lines_info.size())
{
sz=lines_info[idx];
fread(line,sizeof(char),sz,source_file);
//process line
j=sz-1;
while(line[j]=='\n' || line[j]=='\r') --j;
line[j+1]='\0';
j=0;
ptr[j++]=&line[0];
for(i=0;line[i]!='\0';++i)
{
if(line[i]==',')
{
line[i]='\0';
ptr[j++]=&line[i+1];
}
}
//now i have splits ready
int k=0;
for(i=0;i<column_count-1;++i)
{
string s(ptr[i]);
if(columns_index[k]==i)
{
value=map_of_sum[s]/map_of_count[s];
fprintf(target_file,"%lf,",value);
if(k<columns_index.size()) ++k;
}
else fprintf(target_file,"%s,",s.c_str());
}
fprintf(target_file,"%s\n",ptr[column_count-1]);
++idx;
}

delete [] ptr;
delete [] line;

fclose(target_file);
fclose(source_file);
}

//sum encoder
void encoder::sum_encode(string dataset,string filename,vector<string> &columns_name)
{}
void encoder::sum_encode(string dataset,string filename,vector<uint64_t> &columns_index,bool has_header)
{}