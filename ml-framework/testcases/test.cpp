 #include<iostream>
 #include<chrono>
 #include<ml.h>
 using namespace std;
 
 
 int main()
 {
 matrix x,y;
 csv::load_x_y("IceCreamSales.csv",x,y,1);
 
 matrix model(x.columns(),1);
 random_engine::fill(model,0.0,1.0);
 
 int epoch=100000;
 double learning_rate=0.0001;
 
 int i=1;
 
 auto startTime=chrono::high_resolution_clock::now();
 
 matrix y_hat;
 
 matrix error;
 
 matrix x_transposed=x.as_transposed();
 
 matrix squared_error_sum,error_transposed;
 matrix calculatedModelValues;
 double mse;
 while(i<=epoch)
 {
 y_hat=x*model;
 
 mse=ml_math::mean_squared_error(y_hat,y);
 
 mse=mse/2;
 
 error=y_hat-y;
 
 model=model-((learning_rate/x.rows())*(x_transposed*error));
 
 printf("%d.  %.16lf\n",i,mse);
 
 ++i;
 }
 auto endTime=chrono::high_resolution_clock::now();
 chrono::duration<double> elapsed=endTime-startTime;
 cout<<"Time taken for "<<epoch<<" iterations: "<<elapsed.count()<<" seconds"<<endl;
 
 return 0;
 }