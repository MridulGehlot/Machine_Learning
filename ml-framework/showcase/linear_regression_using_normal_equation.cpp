#include<ml.h>
int main()
{
try
{
matrix x,y,min_max,x_test,y_test,y_hat,model;
csv::load_x_y("training.csv",x,y,1);
min_max=scaler::min_max_scaler(x);

//train model
model=linear_regression::normal_equation_fit(x,y);

csv::load_x_y("testing.csv",x_test,y_test,1);
scaler::min_max_scaler(x_test,min_max);

y_hat=x_test*model;
double mse=ml_math::mean_squared_error(y_hat,y_test);
double r2=ml_math::r2_score(y_hat,y_test);

cout<<"MSE : "<<mse<<endl;
cout<<"R2 Score : "<<r2<<endl;
}catch(ml_exception &ex)
{
cout<<ex.what()<<endl;
}
return 0;
}