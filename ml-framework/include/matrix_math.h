#pragma once
#include<cstdint>
#include<cstddef>
class matrix_math
{
private:
matrix_math(){}
public:
static void multiply(double *target,double *left,uint64_t left_rows,uint64_t left_columns,double *right,uint64_t right_rows,uint64_t right_columns);
static void add(double *target,double *left,double *right,size_t size);
static void subtract(double *target,double *left,const double *right,size_t size);

static void multiply(double *target,double *left,uint64_t left_rows,uint64_t left_columns,double right);
static void add(double *target,double *left,uint64_t left_rows,uint64_t left_columns,double right);
static void divide(double *target,double *left,uint64_t left_rows,uint64_t left_columns,double right);
static void subtract(double *target,double *left,uint64_t left_rows,uint64_t left_columns,double right);

static void multiply(double *target,double left,double *right,uint64_t right_rows,uint64_t right_columns);
static void add(double *target,double left,double *right,uint64_t right_rows,uint64_t right_columns);
static void subtract(double *target,double left,double *right,uint64_t right_rows,uint64_t right_columns);
static void divide(double *target,double left,double *right,uint64_t right_rows,uint64_t right_columns);
};