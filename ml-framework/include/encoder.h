#pragma once
#include<iostream>
#include<vector>
#include<cstdint>
using namespace std;
class encoder
{
encoder(){}
public:
static void one_hot_encode(string dataset,string filename,vector<string> &columns_name);
static void one_hot_encode(string dataset,string filename,vector<uint64_t> &columns_index,bool has_header);

static void target_encode(string dataset,string filename,vector<string> &columns_name);
static void target_encode(string dataset,string filename,vector<uint64_t> &columns_index,bool has_header);

static void sum_encode(string dataset,string filename,vector<string> &columns_name);
static void sum_encode(string dataset,string filename,vector<uint64_t> &columns_index,bool has_header);
};