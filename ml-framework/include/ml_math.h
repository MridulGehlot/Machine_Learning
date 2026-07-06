#pragma once
#include<iostream>
#include<matrix.h>
using namespace std;
class ml_math
{
private:
ml_math(){}
public:
static double mean_squared_error(matrix &y_hat,matrix &y);
};