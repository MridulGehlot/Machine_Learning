#include<scaler.h>
#include<ml_exception.h>
#include<cstdint>
#include<algorithm>
#include<cmath>

//min max
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
if(min_max_matrix._rows!=2 || mat._columns!=min_max_matrix._columns) throw ml_exception("Invalid Dimensions of Scaler");
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

//robust
matrix scaler::robust_scaler(matrix &mat)
{
if(mat._columns==0 || mat._rows<=1) throw ml_exception("Cannot Scale The Matrix");
matrix temp(2,mat._columns);
//temp matrix will have cols=features & rows=2 where rows[0]=median and rows[1]=IQR
vector<double> arr(mat._rows);
double median;
double iqr,percent_25th,percent_75th;
double value;
size_t idx_25th,idx_75th;
double *ptr=mat.collection.data();
double *temp_ptr=temp.collection.data();
uint64_t r,c;
for(c=1;c<mat._columns;++c)
{
for(r=0;r<mat._rows;++r)
{
arr[r]=ptr[r*mat._columns+c];
}
//now i have copy of vector
sort(arr.begin(),arr.end());
if(arr.size()%2==0) //even
{
median=(arr[arr.size()/2]+arr[arr.size()/2-1])/2.0;
}
else //odd
{
median=arr[arr.size()/2];
}
idx_25th=static_cast<size_t>(arr.size()*0.25);
idx_75th=static_cast<size_t>(arr.size()*0.75);
percent_25th=arr[idx_25th];
percent_75th=arr[idx_75th];
iqr=percent_75th-percent_25th;
if(iqr==0.0) iqr=1.0;
temp_ptr[0*mat._columns+c]=median;
temp_ptr[1*mat._columns+c]=iqr;
}
//now scale values
for(c=1;c<mat._columns;++c)
{
for(r=0;r<mat._rows;++r)
{
value=ptr[r*mat._columns+c];
value=(value-temp_ptr[0*mat._columns+c])/temp_ptr[1*mat._columns+c];
ptr[r*mat._columns+c]=value;
}
}
return temp;
}
void scaler::robust_scaler(matrix &mat,matrix &robust_scaler_parameters)
{
if(mat._columns!=robust_scaler_parameters._columns || robust_scaler_parameters._rows!=2) throw ml_exception("Invlaid Dimensions of Scaler");
double *ptr=mat.collection.data();
double *temp_ptr=robust_scaler_parameters.collection.data();
//now scale values
uint64_t r,c;
double value;
for(c=1;c<mat._columns;++c)
{
for(r=0;r<mat._rows;++r)
{
value=ptr[r*mat._columns+c];
value=(value-temp_ptr[0*mat._columns+c])/temp_ptr[1*mat._columns+c];
ptr[r*mat._columns+c]=value;
}
}
}

//standard scaler
matrix scaler::standard_scaler(matrix &mat)
{
if(mat._columns==0 || mat._rows<=1) throw ml_exception("Cannot Scale The Matrix");
matrix temp(2,mat._columns);
//temp matrix will have cols=features & rows=2 where rows[0]=mean and rows[1]=std_deviation;
double mean,std_deviation;
double value,sum,pow;
double *ptr=mat.collection.data();
double *temp_ptr=temp.collection.data();
uint64_t r,c;
for(c=1;c<mat._columns;++c)
{
mean=0.0;
for(r=0;r<mat._rows;++r)
{
mean+=ptr[r*mat._columns+c];
}
mean/=mat._rows;
sum=0.0;
for(r=0;r<mat._rows;++r)
{
pow=ptr[r*mat._columns+c]-mean;
pow*=pow;
sum+=pow;
}
std_deviation=sqrt(sum/mat._rows);
if(std_deviation==0.0) std_deviation=1.0;
temp_ptr[0*mat._columns+c]=mean;
temp_ptr[1*mat._columns+c]=std_deviation;
cout<<"Mean - "<<mean<<", std-deviation - "<<std_deviation<<endl;
}
//now scale values
for(c=1;c<mat._columns;++c)
{
for(r=0;r<mat._rows;++r)
{
value=ptr[r*mat._columns+c];
value=(value-temp_ptr[0*mat._columns+c])/temp_ptr[1*mat._columns+c];
ptr[r*mat._columns+c]=value;
}
}
return temp;
}
void scaler::standard_scaler(matrix &mat,matrix &standard_scaler_parameters)
{
if(mat._columns!=standard_scaler_parameters._columns || standard_scaler_parameters._rows!=2) throw ml_exception("Invlaid Dimensions of Scaler");
double *ptr=mat.collection.data();
double *temp_ptr=standard_scaler_parameters.collection.data();
//now scale values
uint64_t r,c;
double value;
for(c=1;c<mat._columns;++c)
{
for(r=0;r<mat._rows;++r)
{
value=ptr[r*mat._columns+c];
value=(value-temp_ptr[0*mat._columns+c])/temp_ptr[1*mat._columns+c];
ptr[r*mat._columns+c]=value;
}
}
}