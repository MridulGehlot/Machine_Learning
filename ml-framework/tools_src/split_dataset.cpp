#include<iostream>
#include<stdlib.h>
#include<ml_exception.h>
#include<dataset_utils.h>
using namespace std;
int main(int argc, char *argv[])
{
//usage : split_dataset filename split_1_percentage header_lines_to_skip file1 file2
if(argc!=6)
{
cout<<"[Invalid Number of Arguments]"<<endl;
cout<<"split_dataset Required 5 Arguments, Passed : "<<argc<<endl;
cout<<"First Argument : Dataset File Name To Split"<<endl;
cout<<"Second Argument : Split Percentage To Put in First File"<<endl;
cout<<"Third Argument : Header Lines To Skip"<<endl;
cout<<"Fourth Argument : First File Name"<<endl;
cout<<"Fifth Argument : Second File Name"<<endl;
return EXIT_FAILURE;
}
string dataset_filename(argv[1]);
int split_percentage=atoi(argv[2]);
int header_lines=atoi(argv[3]);
string file1(argv[4]);
string file2(argv[5]);
if(split_percentage<=0 || split_percentage>=100)
{
cout<<"Invalid Split Percentage"<<endl;
return EXIT_FAILURE;
}
if(dataset_filename.size()==0)
{
cout<<"Data Set File Name Required"<<endl;
return EXIT_FAILURE;
}
if(header_lines<0)
{
cout<<"Invalid Number of Header Lines to Skip"<<endl;
return EXIT_FAILURE;
}
if(file1.size()==0)
{
cout<<"First File Name Required"<<endl;
return EXIT_FAILURE;
}
if(file2.size()==0)
{
cout<<"Second File Name Required"<<endl;
return EXIT_FAILURE;
}

try
{
dataset_utils::split(dataset_filename,split_percentage,header_lines,file1,file2);
}catch(ml_exception &ex)
{
cout<<ex.what()<<endl;
}
return EXIT_SUCCESS;
}