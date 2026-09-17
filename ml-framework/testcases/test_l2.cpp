#include<ml.h>
int main()
{
matrix y(3,1);
y[0][0]=2.0;
y[1][0]=4.0;
y[2][0]=5.0;

matrix y_hat(3,1);
y_hat[0][0]=2.5;
y_hat[1][0]=3.8;
y_hat[2][0]=5.5;

matrix model(3,1);
model[0][0]=1.5;
model[1][0]=-2.0;
model[2][0]=0.8;

double lambda=0.1;

double l2_error=ml_math::l2_error(y_hat,y,model,lambda);
cout<<"L2 Error : "<<l2_error<<endl;
return 0;
}