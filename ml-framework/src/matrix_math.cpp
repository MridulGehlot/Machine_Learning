#include<matrix_math.h>
#include<omp.h>
#include<string.h>
#include<iostream>
#include<ml_exception.h>
using namespace std;
void matrix_math::multiply(double *target,double *left,uint64_t left_rows,uint64_t left_columns,double *right,uint64_t right_rows,uint64_t right_columns)
{
uint64_t target_columns=right_columns;
uint64_t target_size=left_rows*right_columns;
uint64_t c1,c2,c3;
memset(target,0.0,sizeof(double)*target_size);
#pragma omp parallel for private(c2,c3)
for(c1=0;c1<left_rows;++c1)
{
for(c2=0;c2<left_columns;++c2)
{
for(c3=0;c3<right_columns;++c3)
{
//target[c1][c3]+=left[c1][c2]*right[c2][c3];
target[c1*target_columns+c3]+=left[c1*left_columns+c2]*right[c2*right_columns+c3];
}
}
}
}
void matrix_math::add(double *target,double *left,double *right,size_t sz)
{
#pragma omp parallel for
for(uint64_t k=0;k<sz;++k) target[k]=left[k]+right[k];
}
void matrix_math::subtract(double *target,double *left,const double *right,size_t sz)
{
#pragma omp parallel for
for(uint64_t k=0;k<sz;++k) target[k]=left[k]-right[k];
}


void matrix_math::multiply(double *target,double *left,uint64_t left_rows,uint64_t left_columns,double right)
{
uint64_t sz=left_rows*left_columns;
#pragma omp parallel for
for(uint64_t k=0;k<sz;++k) target[k]=left[k]*right;
}
void matrix_math::add(double *target,double *left,uint64_t left_rows,uint64_t left_columns,double right)
{
uint64_t sz=left_rows*left_columns;
#pragma omp parallel for
for(uint64_t k=0;k<sz;++k) target[k]=left[k]+right;
}
void matrix_math::divide(double *target,double *left,uint64_t left_rows,uint64_t left_columns,double right)
{
if(right==0.0) throw ml_exception("Division By 0");
uint64_t sz=left_rows*left_columns;
#pragma omp parallel for
for(uint64_t k=0;k<sz;++k) target[k]=left[k]/right;
}
void matrix_math::subtract(double *target,double *left,uint64_t left_rows,uint64_t left_columns,double right)
{
uint64_t sz=left_rows*left_columns;
#pragma omp parallel for
for(uint64_t k=0;k<sz;++k) target[k]=left[k]-right;
}


void matrix_math::multiply(double *target,double left,double *right,uint64_t right_rows,uint64_t right_columns)
{
uint64_t sz=right_rows*right_columns;
#pragma omp parallel for
for(uint64_t i=0;i<sz;++i) target[i]=left*right[i];
}
void matrix_math::add(double *target,double left,double *right,uint64_t right_rows,uint64_t right_columns)
{
uint64_t sz=right_rows*right_columns;
#pragma omp parallel for
for(uint64_t i=0;i<sz;++i) target[i]=left+right[i];
}
void matrix_math::subtract(double *target,double left,double *right,uint64_t right_rows,uint64_t right_columns)
{
uint64_t sz=right_rows*right_columns;
#pragma omp parallel for
for(uint64_t i=0;i<sz;++i) target[i]=left-right[i];
}
void matrix_math::divide(double *target,double left,double *right,uint64_t right_rows,uint64_t right_columns)
{
uint64_t sz=right_rows*right_columns;
#pragma omp parallel for
for(uint64_t i=0;i<sz;++i) target[i]=left/right[i];
}