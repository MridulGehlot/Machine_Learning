#include<linear_regression.h>
#include<matrix.h>
matrix linear_regression::fit(matrix &x,matrix &y)
{
matrix x_t,model,tt,temp;
x_t=x.as_transposed();
tt=x_t*x;
temp=tt.as_inversed();
model=temp*x_t*y;
return model;
}