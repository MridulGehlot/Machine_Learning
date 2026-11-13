#include<linear_regression.h>
#include<matrix.h>
#include<ml_math.h>
#include<ml_exception.h>
#include<algorithm>
#include<random>
using namespace std;

matrix linear_regression::normal_equation_fit(matrix &x,matrix &y)
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

matrix linear_regression::mini_batch_gradient_fit(matrix &x,matrix &y,double learning_rate,double regularization_parameter,uint64_t epoch,uint64_t batch_size,cost_function_type cost_function,int (*callback_function)(double cost,uint64_t training_cycle_number,const matrix &x,matrix &y,matrix &model,double learning_rate,double regularization_parameter,uint64_t number_of_epoches))
{
if(batch_size>x.rows()) throw ml_exception("Invalid Batch Size");
matrix y_hat,error;
double cost;
matrix model(x.columns(),1);
random_engine::fill(model,0.0,1.0);
vector<uint64_t> index(x.rows(),0);
for(int i=0;i<x.rows();++i) index[i]=i;

std::random_device rd;
std::mt19937 g(rd());

uint64_t total_batches=x.rows()/batch_size;
if(x.rows()%batch_size) ++total_batches;

matrix small_x,small_y;
uint64_t update_number=0;

for(uint64_t cycle=1;cycle<=epoch;++cycle)
{
uint64_t cc=0;
small_x.resize(batch_size,x.columns());
for(int i=0;i<batch_size;++i) small_x.set(i,0,1);
small_y.resize(batch_size,1);
shuffle(index.begin(),index.end(),g);
for(uint64_t b=0;b<total_batches;++b)
{
//prepare Batch matrix
int j;
for(j=0;j<batch_size && cc<index.size();++j)
{
for(uint64_t k=1;k<x.columns();++k) small_x.set(j,k,x.get(index[cc],k));
small_y.set(j,0,y.get(index[cc],0));
++cc;
}
if(j!=batch_size)
{
small_x.resize(j,x.columns());
small_y.resize(j,1);
}
//Now solve for this batch
y_hat=small_x*model;
error=y_hat-small_y;
matrix small_x_transpose=small_x.as_transposed();
model = model - ((small_x_transpose*error)*learning_rate/j);
++update_number;
if(cost_function==MEAN_SQUARED_ERROR) cost=ml_math::mean_squared_error(y_hat,small_y)/2.0;
//if(!callback_function(cost,cycle,x,y,model,learning_rate,regularization_parameter,epoch)) break;
if(!callback_function(cost,update_number,x,y,model,learning_rate,regularization_parameter,epoch)) break;
}
}
return model;
}