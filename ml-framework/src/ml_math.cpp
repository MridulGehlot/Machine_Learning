#include<ml_math.h>
#include<math.h>
#include<matrix.h>
double ml_math::mean_squared_error(matrix &y_hat,matrix &y)
{
double error=0.0;
uint32_t rows=y.rows();
for(int i=0;i<rows;++i) error+=pow(y_hat[i][0]-y[i][0],2);
return error/rows;
}