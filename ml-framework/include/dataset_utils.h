#pragma once
#include<matrix.h>
#include<cstdint>
#include<iostream>
using namespace std;
class matrix;
class dataset_utils
{
dataset_utils(){}
public:
static void split(string filename,uint8_t split_percentage,uint64_t header_lines,matrix &matrix_1,matrix &matrix_2);
static void split(string filename,uint8_t split_percentage,uint64_t header_lines,const string &file1,const string &file2);
};