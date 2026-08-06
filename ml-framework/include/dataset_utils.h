#pragma once
#include<matrix.h>
#include<cstdint>
#include<vector>
#include<iostream>
using namespace std;
class matrix;
class dataset_utils
{
dataset_utils(){}
public:
static void split(string filename,uint8_t split_percentage,uint64_t header_lines,matrix &matrix_1,matrix &matrix_2);
static void split(string filename,uint8_t split_percentage,uint64_t header_lines,const string &file1,const string &file2);
static void remove_columns(string dataset,string filename,vector<uint32_t> &columns_index);
static void remove_columns(string dataset,string filename,vector<string> &columns_name);
};