#include<linear_regression.h>
#include<matrix.h>
#include<ml_math.h>

matrix linear_regression::normal_eqation_fit(matrix &x,matrix &y)
{
matrix x_t,model,tt,temp;
x_t=x.as_transposed();
tt=x_t*x;
temp=tt.as_inversed();
model=temp*x_t*y;
return model;
}

matrix linear_regression::batch_gradient_fit(matrix &x,matrix &y,double learning_rate,double regularization_parameter,uint64_t epoch,cost_function_type cost_function,int (*callback_function)(double cost,uint64_t training_cycle_number,const matrix &x,matrix &y,matrix &model,double learning_rate,double regularization_parameter,uint64_t number_of_epoches))
{
matrix y_hat,error,x_transpose;
double cost;
matrix model(x.columns(),1);
random_engine::fill(model,0.0,1.0);
x_transpose=x.as_transposed();
for(uint64_t cycle=1;cycle<=epoch;++cycle)
{
y_hat=x*model;
error=y_hat-y;
if(cost_function==MEAN_SQUARED_ERROR) cost=ml_math::mean_squared_error(y_hat,y)/2.0;
model = model - (learning_rate * (x_transpose*error))/x.rows();
if(!callback_function(cost,cycle,x,y,model,learning_rate,regularization_parameter,epoch)) break;
}
return model;
}
