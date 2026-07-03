matrix x,y,model,error;
while(cycles)
{
y_hat = x*model;
error = y_hat - y;
double squared_error = error * error_transpose

//double cost = squared_error / n or (x_rows);
double cost = squared_error / (2.0*x_rows);

model = model - ((x_transpose * error) * 0.001)/ n(x_rows);

}