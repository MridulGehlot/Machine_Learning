#include<matrix_math.h>
#include<cmath>
#include<omp.h>
#include<string.h>
#include<iostream>
#include<ml_exception.h>
using namespace std;
void matrix_math::multiply(double *target,const double *left,const uint64_t left_rows,const uint64_t left_columns,const double *right,const uint64_t right_rows,const uint64_t right_columns)
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
void matrix_math::add(double *target,const double *left,const double *right,const size_t sz)
{
#pragma omp parallel for
for(uint64_t k=0;k<sz;++k) target[k]=left[k]+right[k];
}
void matrix_math::subtract(double *target,const double *left,const double *right,const size_t sz)
{
#pragma omp parallel for
for(uint64_t k=0;k<sz;++k) target[k]=left[k]-right[k];
}


void matrix_math::multiply(double *target,const double *left,const uint64_t left_rows,const uint64_t left_columns,const double right)
{
uint64_t sz=left_rows*left_columns;
#pragma omp parallel for
for(uint64_t k=0;k<sz;++k) target[k]=left[k]*right;
}
void matrix_math::add(double *target,const double *left,const uint64_t left_rows,const uint64_t left_columns,const double right)
{
uint64_t sz=left_rows*left_columns;
#pragma omp parallel for
for(uint64_t k=0;k<sz;++k) target[k]=left[k]+right;
}
void matrix_math::divide(double *target,const double *left,const uint64_t left_rows,const uint64_t left_columns,const double right)
{
if(right==0.0) throw ml_exception("Division By 0");
uint64_t sz=left_rows*left_columns;
#pragma omp parallel for
for(uint64_t k=0;k<sz;++k) target[k]=left[k]/right;
}
void matrix_math::subtract(double *target,const double *left,const uint64_t left_rows,const uint64_t left_columns,const double right)
{
uint64_t sz=left_rows*left_columns;
#pragma omp parallel for
for(uint64_t k=0;k<sz;++k) target[k]=left[k]-right;
}


void matrix_math::multiply(double *target,const double left,const double *right,const uint64_t right_rows,const uint64_t right_columns)
{
uint64_t sz=right_rows*right_columns;
#pragma omp parallel for
for(uint64_t i=0;i<sz;++i) target[i]=left*right[i];
}
void matrix_math::add(double *target,const double left,const double *right,const uint64_t right_rows,const uint64_t right_columns)
{
uint64_t sz=right_rows*right_columns;
#pragma omp parallel for
for(uint64_t i=0;i<sz;++i) target[i]=left+right[i];
}
void matrix_math::subtract(double *target,const double left,const double *right,const uint64_t right_rows,const uint64_t right_columns)
{
uint64_t sz=right_rows*right_columns;
#pragma omp parallel for
for(uint64_t i=0;i<sz;++i) target[i]=left-right[i];
}
void matrix_math::divide(double *target,const double left,const double *right,const uint64_t right_rows,const uint64_t right_columns)
{
uint64_t sz=right_rows*right_columns;
#pragma omp parallel for
for(uint64_t i=0;i<sz;++i) target[i]=left/right[i];
}

void matrix_math::inverse(double *target,const double *source,const uint64_t rows,const uint64_t columns)
{
uint64_t idx;
uint64_t columns_size=columns*2;
double pivot_value,factor;
double *tmp,*swap_space;
tmp=(double *)malloc(sizeof(double)*rows*columns_size);
if(tmp==nullptr) throw ml_exception("Not Enough Memory");
swap_space=(double *)malloc(sizeof(double)*columns_size);
if(swap_space==nullptr)
{
free(tmp);
throw ml_exception("Not Enough Memory");
}
for(int i=0;i<rows;++i)
{
for(int j=0;j<columns;++j)
{
tmp[i*columns_size+j]=source[i*columns+j];
}
for(int j=0;j<columns;++j)
{
if(i==j) tmp[i*columns_size+j+columns]=1.0;
else tmp[i*columns_size+j+columns]=0.0;
}
}
//now inverse the matrix
for(int r=0;r<rows;++r)
{
int pivot=r;
for(idx=r+1;idx<rows;++idx)
{
if(fabs(tmp[idx*columns_size+r])>fabs(tmp[pivot*columns_size+r]))
{
pivot=idx;
}
}
if(fabs(tmp[pivot*columns_size+r])<1e-12) 
{
free(tmp);
free(swap_space);
throw ml_exception("Matrix is singular and has no inverse");
}
if(pivot!=r)
{
//swap them
for(idx=0;idx<columns_size;++idx)
{
swap_space[idx]=tmp[pivot*columns_size+idx];
}
for(idx=0;idx<columns_size;++idx)
{
tmp[pivot*columns_size+idx]=tmp[r*columns_size+idx];
tmp[r*columns_size+idx]=swap_space[idx];
}
}
//make tmp[r][r] as 1
pivot_value=tmp[r*columns_size+r];
for(idx=0;idx<columns_size;++idx) tmp[r*columns_size+idx]/=pivot_value;
//make all other values as 0 in this column
for(idx=0;idx<rows;++idx)
{
if(r==idx) continue;
factor=tmp[idx*columns_size+r];
for(int c=0;c<columns_size;++c)
{
tmp[idx*columns_size+c]-=factor*tmp[r*columns_size+c];
}
}
} //for loop ends here for inversion
//copy in target
for(int r=0;r<rows;++r)
{
for(int c=0;c<columns;++c)
{
target[r*columns+c]=tmp[r*columns_size+c+columns];
}
}
free(tmp);
free(swap_space);
} //function ends here