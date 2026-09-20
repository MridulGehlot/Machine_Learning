#pragma once
#include<matrix.h>

enum cost_function_type
{
MEAN_SQUARED_ERROR,
MEAN_ABSOLUTE_ERROR
};

class linear_regression
{
public:
static matrix normal_eqation_fit(matrix &x,matrix &y);
static matrix batch_gradient_fit(matrix &x,matrix &y,double learning_rate,double regularization_parameter,uint64_t epoch,cost_function_type cost_function,int (*callback_function)(double,uint64_t,const matrix&,matrix&,matrix&,double,double,uint64_t));
};