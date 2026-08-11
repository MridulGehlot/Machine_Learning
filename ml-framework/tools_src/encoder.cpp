#include<iostream>
#include<stdlib.h>
#include<ml_exception.h>
#include<dataset_utils.h>
using namespace std;
int main(int argc, char *argv[])
{
//usage : encoder dataset_name file_name Encoder_Type header_name_to_encode
if(argc<5)
{
cout<<"[Invalid Number of Arguments]"<<endl;
cout<<"Encoder Required Minimum 4 Arguments, Passed : "<<argc<<endl;
cout<<"First Argument : Dataset File Name"<<endl;
cout<<"Second Argument : File Name"<<endl;
cout<<"Third Argument Type : Encoder_Type"<<endl;
cout<<"N Arguments : Header Names To Encode"<<endl;
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
vector<string> name_columns_indexes;
if(type=="one_hot")
{
for(int i=4;i<argc;i++)
{
string s(argv[i]);
name_columns_indexes.push_back(s);
}
}

try
{
if(type=="one_hot") dataset_utils::one_hot_encode(dataset_filename,filename,name_columns_indexes);
}catch(ml_exception &ex)
{
cout<<ex.what()<<endl;
}
return EXIT_SUCCESS;
}