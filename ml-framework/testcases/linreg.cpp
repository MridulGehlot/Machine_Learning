#include<iostream>
#include<stdio.h>
#include<ml.h>
#define TRAINING_CYCLES 100000
#define LEARNING_RATE 0.0001
#define STOPPER 10000
using namespace std;
int main()
{
matrix x,y,y_hat,error;
csv::load_x_y("IceCreamSales.csv",x,y,1);
matrix model(2,1);
random_engine::fill(model,0.0,1.0);
uint64_t cycle=1;
double mse,cost;
while(cycle<=TRAINING_CYCLES)
{
y_hat=x*model;
error=y_hat-y;
mse=ml_math::mean_squared_error(y_hat,y);
cost=mse/(2*x.rows());
if(cycle%STOPPER==0) printf("Cost - %30.20lf\n",cost);
model = model - (((x.as_transposed()*error)*LEARNING_RATE)/x.rows());
++cycle;
}
//model.save("trained_model.data");
return 0;
}