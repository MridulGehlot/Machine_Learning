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
try
{
FILE *f;
matrix x,y,y_hat,error,xt;
csv::load_x_y("training.csv",x,y,1);
matrix min_max=scaler::min_max_scaler(x);
//save y and scaled data in x
f=fopen("scaled_data_points.csv","w");
uint64_t r,c;
uint64_t rows,cols;
rows=x.rows();
cols=x.columns();
for(r=0;r<rows;++r)
{
for(c=1;c<cols;++c) //not 0 beacuse it is bias
{
fprintf(f,"%lf,",x.get(r,c));
}
fprintf(f,"%lf\n",y.get(r,0));
}
fclose(f);
matrix model(2,1);
random_engine::fill(model,0.0,1.0);
uint64_t cycle=1;
double mse,cost;
xt=x.as_transposed();
//now to plot cost
f=fopen("cost_data_points.csv","w");
while(cycle<=TRAINING_CYCLES)
{
y_hat=x*model;
error=y_hat-y;
mse=ml_math::mean_squared_error(y_hat,y);
cost=mse/2.0;
//issue
//fprintf(f,"%d,%lf\n",cycle,cost);
if(cycle%STOPPER==0) 
{
printf("Cost - %30.20lf\n",cost);
fprintf(f,"%llu,%.15lf\n",(unsigned long long)cycle,cost);
}
model = model - (((xt*error)*LEARNING_RATE)/x.rows());
++cycle;
}
fclose(f);
//save model
model.save("IceCreamSales_trained_model.csv",FMT_CSV);
//save min_max
min_max.save("IceCreamSales_min_max_scaler_data.csv",FMT_CSV);
//save x and y_hat in file to plot line
f=fopen("line_data_points.csv","w");
rows=x.rows();
cols=x.columns();
for(r=0;r<rows;++r)
{
for(c=1;c<cols;++c) //not 0 beacuse it is bias
{
fprintf(f,"%lf,",x.get(r,c));
}
fprintf(f,"%lf\n",y_hat.get(r,0));
}
fclose(f);

//r2_score
matrix y_test,x_test,y_hat_test;
csv::load_x_y("testing.csv",x_test,y_test,1);
scaler::min_max_scaler(x_test,min_max);
y_hat_test=x_test*model;
double r2=ml_math::r2_score(y_hat_test,y_test);
cout<<"R2 Score : "<<r2<<endl;
}catch(ml_exception &ex)
{
cout<<ex.what()<<endl;
}
return 0;
}