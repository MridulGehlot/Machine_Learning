#include<iostream>
#include<ml.h>
#define TRAINING_CYCLES 10
using namespace std;
int main()
{
matrix x,y_hat,y,error,model,et;
csv::load_dataset("IceCreamSales.csv",x,y);
random_initializer::init(model);
uint32_t cycle=1;
while(cycle<=TRAINING_CYCLES)
{
y_hat=x*model;
error=y_hat-y;
et=error.as_transposed();
double squared_error_sum=et*error;
cost = squared_error_sum /(2*x.rows());
model = model - (x.as_transposed()*error*LEARNING_RATE)/x.rows();
++cycle;
}
return 0;
}