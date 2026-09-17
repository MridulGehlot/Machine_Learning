#pragma once
#include<iostream>
#include<matrix.h>
using namespace std;
class ml_math
{
private:
ml_math(){}
public:
//error functions
static double mean_squared_error(matrix &y_hat,matrix &y);
static double l2_error(matrix &y_hat,matrix &y,matrix &model,double lambda);
static double elastic_net_error(matrix &y_hat,matrix &y,matrix &model,double lambda1,double lambda2);

static double r2_score(matrix &y_hat,matrix &y);
};