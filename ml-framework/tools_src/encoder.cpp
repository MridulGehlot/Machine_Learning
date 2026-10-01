#include<iostream>
#include<stdlib.h>
#include<ml_exception.h>
#include<encoder.h>
#include<set>
using namespace std;
int main(int argc, char *argv[])
{
//usage : encoder dataset_name file_name Encoder_Type header_name_to_encode
if(argc<6)
{
cout<<"[Invalid Number of Arguments]"<<endl;
cout<<"Encoder Required Minimum 5 Arguments, Passed : "<<(argc-1)<<endl;
cout<<"First Argument : Dataset File Name"<<endl;
cout<<"Second Argument : Target File Name"<<endl;
cout<<"Third Argument Type : Encoder_Type(one_hot,target,sum,ordinal)"<<endl;
cout<<"Fourth Argument Type : EncodeBy(name,index)"<<endl;
cout<<"N Arguments : Header Names/Indexes To Encode"<<endl;
return EXIT_FAILURE;
}

try
{

string dataset_filename(argv[1]);
string filename(argv[2]);
string encoder_type(argv[3]);
string encode_by(argv[4]);

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
set<string> st;
st.insert("one_hot");
st.insert("sum");
st.insert("target");
st.insert("ordinal");
if(!st.count(encoder_type))
{
cout<<"Invalid Encoder Type"<<endl;
return 1;
}
if(!(encode_by=="name" || encode_by=="index"))
{
cout<<"Invalid Encode By Type"<<endl;
return 1;
}

vector<string> name_columns_indexes;
for(int i=5;i<argc;i++)
{
string s(argv[i]);
name_columns_indexes.push_back(s);
}

//prepare index
char ch;
bool has_header=true;
vector<uint64_t> columns_index;
if(encode_by=="index")
{
cout<<"Does dataset have header (y/n) ? : ";
cin>>ch;
if(ch=='n' || ch=='N') has_header=false;
for(string &s:name_columns_indexes)
{
int x=stoi(s);
columns_index.push_back(x);
}
}

if(encoder_type=="ordinal")
{
//yet to implement
}

//encode
if(encoder_type=="one_hot" && encode_by=="name") encoder::one_hot_encode(dataset_filename,filename,name_columns_indexes);
else if(encoder_type=="one_hot" && encode_by=="index") encoder::one_hot_encode(dataset_filename,filename,columns_index,has_header);
else if(encoder_type=="target" && encode_by=="name") encoder::target_encode(dataset_filename,filename,name_columns_indexes);
else if(encoder_type=="target" && encode_by=="index") encoder::target_encode(dataset_filename,filename,columns_index,has_header);
else if(encoder_type=="sum" && encode_by=="name") encoder::sum_encode(dataset_filename,filename,name_columns_indexes);
else if(encoder_type=="sum" && encode_by=="index") encoder::sum_encode(dataset_filename,filename,columns_index,has_header);
}catch(ml_exception &ex)
{
cout<<ex.what()<<endl;
}
return EXIT_SUCCESS;
}