#include<ml_math.h>
#include<math.h>
#include<matrix.h>
double ml_math::mean_squared_error(matrix &y_hat,matrix &y)
{
double error=0.0;
uint32_t rows=y.rows();
for(int i=0;i<rows;++i) error+=pow(y_hat[i][0]-y[i][0],2);
return error/rows;
}
double ml_math::r2_score(matrix &y_hat,matrix &y)
{
double y_mean,y_sum;
y_sum=0.0;
double *y_ptr=y.collection.data();
double *y_hat_ptr=y_hat.collection.data();
int sz=y_hat._rows;
for(int i=0;i<sz;++i) y_sum+=y_ptr[i];
y_mean=y_sum/sz;
double err;
double squared_error_sum=0.0;
for(int i=0;i<sz;++i) 
{
err=y_hat_ptr[i]-y_ptr[i];
squared_error_sum+=err*err;
}
double squared_variance_sum=0.0;
for(int i=0;i<sz;++i) 
{
err=y_ptr[i]-y_mean;
squared_variance_sum+=err*err;
}
return 1.0-(squared_error_sum/squared_variance_sum);
}