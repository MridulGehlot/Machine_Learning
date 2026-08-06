#include<iostream>
#include<stdlib.h>
#include<ml_exception.h>
#include<dataset_utils.h>
using namespace std;
int main(int argc, char *argv[])
{
//usage : remove_columns dataset_name file_name header_indexes_to_cut 
if(argc<5)
{
cout<<"[Invalid Number of Arguments]"<<endl;
cout<<"split_dataset Required Minimum 3 Arguments, Passed : "<<argc<<endl;
cout<<"First Argument : Dataset File Name"<<endl;
cout<<"Second Argument : File Name"<<endl;
cout<<"Third Argument Type : name/index"<<endl;
cout<<"N Arguments : Header Indexes/Names To Cut"<<endl;
return EXIT_FAILURE;
}
string dataset_filename(argv[1]);
string filename(argv[2]);
string type(argv[3]);
if(dataset_filename.size()==0)
{
cout<<"Data Set File Name Required"<<endl;
return EXIT_FAILURE;
}
if(filename.size()==0)
{
cout<<"File Name Required"<<endl;
return EXIT_FAILURE;
}
vector<uint32_t> index_columns_indexes;
if(type=="index")
{
uint32_t x;
for(int i=4;i<argc;i++)
{
x=atoi(argv[i]);
index_columns_indexes.push_back(x);
}
}
vector<string> name_columns_indexes;
if(type=="name")
{
for(int i=4;i<argc;i++)
{
string s(argv[i]);
name_columns_indexes.push_back(s);
}
}

try
{
if(type=="index") dataset_utils::remove_columns(dataset_filename,filename,index_columns_indexes);
if(type=="name") dataset_utils::remove_columns(dataset_filename,filename,name_columns_indexes);
}catch(ml_exception &ex)
{
cout<<ex.what()<<endl;
}
return EXIT_SUCCESS;
}