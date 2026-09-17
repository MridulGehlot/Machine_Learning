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

double ml_math::l2_error(matrix &y_hat,matrix &y,matrix &model,double lambda)
{
double mse=ml_math::mean_squared_error(y_hat,y);
double squared_sum=0.0;
//skip last row which is intercept c
for(uint32_t i=0;i<model.rows()-1;++i) squared_sum+=pow(model[i][0],2);
double penalty=(lambda/(2*y.rows()))*squared_sum;
return mse+penalty;
}

double ml_math::elastic_net_error(matrix &y_hat,matrix &y,matrix &model,double lambda1,double lambda2)
{
double l2_error=ml_math::l2_error(y_hat,y,model,lambda2);
double absolute_sum=0.0;
//skip last row which is intercept c
for(uint32_t i=0;i<model.rows()-1;++i) absolute_sum+=abs(model[i][0]);
double l1_penalty=(lambda1/y.rows())*absolute_sum;
return l2_error+l1_penalty;
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