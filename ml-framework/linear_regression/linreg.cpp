#include<iostream>
#include<stdio.h>
#include<ml.h>
#include<chrono>
#define TRAINING_CYCLES 20000
#define LEARNING_RATE 0.01
#define STOPPER 1000
using namespace std;
int main()
{
matrix x,y,y_hat,error,xt;
csv::load_x_y("training.csv",x,y,1);
matrix min_max=scaler::min_max_scaler(x);
matrix model(2,1);
random_engine::fill(model,0.0,1.0);
uint64_t cycle=1;
double mse,cost;
xt=x.as_transposed();
while(cycle<=TRAINING_CYCLES)
{
y_hat=x*model;
error=y_hat-y;
mse=ml_math::mean_squared_error(y_hat,y);
cost=mse/2.0;
if(cycle%STOPPER==0) printf("Cost - %30.20lf\n",cost);
model = model - (((xt*error)*LEARNING_RATE)/x.rows());
++cycle;
}
//r2_score
matrix y_test,x_test,y_hat_test;
csv::load_x_y("testing.csv",x_test,y_test,1);
scaler::min_max_scaler(x_test,min_max);
y_hat_test=x_test*model;
double r2=ml_math::r2_score(y_hat_test,y_test);
cout<<"R2 Score : "<<r2<<endl;
//model.save("trained_model.data");
return 0;
}