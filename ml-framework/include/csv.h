#pragma once
#include<iostream>
#include<matrix.h>
#include<cstdint>
using namespace std;

class matrix;

class csv
{
private:
csv(){}
public:
static void load_x_y(string datasetFileName,matrix &x,matrix &y,uint64_t skipLines);
};