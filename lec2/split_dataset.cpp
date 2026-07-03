#include<iostream>
#include<stdlib.h>
#include<unordered_set>
#include<random>
#include<string.h>
#include<cstdint>
using namespace std;
int main(int a_args,char * a_vals[])
{
char ch;
char dataset_file_name[101];
char training_dataset_file_name[151];
char testing_dataset_file_name[151];
if(a_args!=4)
{
cout<<"Usage : [split_data_set dataset_file_name training_percentage number_of_header_rows]"<<endl;
return 1;
}
strcpy(dataset_file_name,a_vals[1]);
double training_percentage = atof(a_vals[2]);
int header_rows_to_skip = atoi(a_vals[3]);
if(training_percentage<=0 || training_percentage>=100)
{
cout<<"Invalid Training Percentage"<<endl;
return 1;
}
FILE * f=fopen(dataset_file_name,"rb");
if(f==nullptr)
{
cout<<"No Dataset File Exists"<<endl;
fclose(f);
return 1;
}

strcpy(training_dataset_file_name,"training_");
strcpy(testing_dataset_file_name,"testing_");
strcat(training_dataset_file_name,dataset_file_name);
strcat(testing_dataset_file_name,dataset_file_name);

cout<<training_dataset_file_name<<endl;
cout<<testing_dataset_file_name<<endl;


//the following code is highly un optimized we will improve later
uint32_t dataset_row_count=0;
while(1)
{
ch=fgetc(f);
if(feof(f)) break;
if(ch=='\n') ++dataset_row_count;
}
fseek(f,-1,2);
ch=fgetc(f);
if(ch!='\n') ++dataset_row_count;
if(header_rows_to_skip>=dataset_row_count)
{
cout<<"Invalid Number of Header Rows to skip";
fclose(f);
return 1;
}
dataset_row_count-=header_rows_to_skip;

uint32_t training_row_count=(dataset_row_count*(training_percentage/100));
uint32_t testing_row_count=dataset_row_count-training_row_count;

rewind(f);
uint32_t ri=1;
while(ri<=header_rows_to_skip)
{
while(1)
{
ch=fgetc(f);
if(feof(f))
{
fclose(f);
cout<<"Malformed Dataset"<<endl;
return 1;
}
if(ch=='\n') break; 
}
++ri;
}

random_device rd;
uniform_int_distribution<int> d(1,dataset_row_count);
mt19937 engine(rd());
unordered_set<int> st;
for(int i=0;i<testing_row_count;++i) st.insert(d(engine));
cout<<st.size()<<endl;

FILE *training_file_name=fopen(training_dataset_file_name,"wb");
FILE *testing_file_name=fopen(testing_dataset_file_name,"wb");

ri=1;
while(1)
{
ch=fgetc(f);
if(feof(f)) break;
if(st.count(ri)) fputc(ch,testing_file_name);
else fputc(ch,training_file_name);
if(ch=='\n')
{
++ri;
}
}

fclose(training_file_name);
fclose(testing_file_name);
fclose(f);
return 0;
}