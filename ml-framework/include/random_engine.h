#pragma once
#include<matrix.h>
class matrix;
class random_engine
{
random_engine(){}
public:
static void fill(matrix &other,double lower_bound,double upper_bound);
};