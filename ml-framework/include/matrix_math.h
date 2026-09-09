#pragma once
#include<cstdint>
#include<cstddef>
class matrix_math
{
private:
matrix_math(){}
public:
static void multiply(double *target,const double *left,const uint64_t left_rows,const uint64_t left_columns,const double *right,const uint64_t right_rows,const uint64_t right_columns);
static void add(double *target,const double *left,const double *right,const size_t size);
static void subtract(double *target,const double *left,const double *right,const size_t size);

static void multiply(double *target,const double *left,const uint64_t left_rows,const uint64_t left_columns,const double right);
static void add(double *target,const double *left,const uint64_t left_rows,const uint64_t left_columns,const double right);
static void divide(double *target,const double *left,const uint64_t left_rows,const uint64_t left_columns,const double right);
static void subtract(double *target,const double *left,const uint64_t left_rows,const uint64_t left_columns,const double right);

static void multiply(double *target,const double left,const double *right,const uint64_t right_rows,const uint64_t right_columns);
static void add(double *target,const double left,const double *right,const uint64_t right_rows,const uint64_t right_columns);
static void subtract(double *target,const double left,const double *right,const uint64_t right_rows,const uint64_t right_columns);
static void divide(double *target,const double left,const double *right,const uint64_t right_rows,const uint64_t right_columns);

static void inverse(double *target,const double *source,const uint64_t rows,const uint64_t columns);
};