#include<scaler.h>
#include<ml_exception.h>
#include<cstdint>
matrix scaler::min_max_scaler(matrix &mat)
{
if(mat._rows<=1 || mat._columns==0) throw ml_exception("Cannot Scale The Matrix");
matrix tmp(2,mat._columns);
double min,max,value,scaled_value,divide_by;
uint64_t rr,cc;
double *target=tmp.collection.data();
double *ptr=mat.collection.data();
for(cc=0;cc<mat._columns;++cc) 
{
/*
target[0][c]=ptr[0][c]; //min
target[1][c]=ptr[0][c]; //max
*/
target[0*tmp._columns+cc]=ptr[cc];
target[1*tmp._columns+cc]=ptr[cc];
}
for(rr=1;rr<mat._rows;++rr)
{
for(cc=0;cc<mat._columns;++cc) 
{
value=ptr[rr*mat._columns+cc];
if(value<target[cc]) target[cc]=value;
if(value>target[tmp._columns+cc]) target[tmp._columns+cc]=value;
}
}
//now scale the values
for(rr=0;rr<mat._rows;++rr)
{
for(cc=0;cc<mat._columns;++cc) 
{
value=ptr[rr*mat._columns+cc];
min=target[cc];
max=target[cc+tmp._columns];
divide_by=max-min;
if(divide_by!=0)
{
scaled_value=(value-min)/divide_by;
ptr[rr*mat._columns+cc]=scaled_value;
}
}
}
return tmp;
}
void scaler::min_max_scaler(matrix &mat,matrix &min_max_matrix)
{
//now scale the values
uint64_t rr,cc;
double value,scaled_value,min,max,divide_by;
double *ptr=mat.collection.data();
double *target=min_max_matrix.collection.data();
for(rr=0;rr<mat._rows;++rr)
{
for(cc=0;cc<mat._columns;++cc) 
{
value=ptr[rr*mat._columns+cc];
min=target[cc];
max=target[cc+min_max_matrix._columns];
divide_by=max-min;
if(divide_by!=0)
{
scaled_value=(value-min)/divide_by;
ptr[rr*mat._columns+cc]=scaled_value;
}
}
}
}
