#pragma once
#include<matrix.h>
class scaler
{
scaler(){}
public:
//mix-max
static matrix min_max_scaler(matrix &mat);
static void min_max_scaler(matrix &mat,matrix &min_max_matrix);
//robust
static matrix robust_scaler(matrix &mat);
static void robust_scaler(matrix &mat,matrix &robust_scaler_parameters);
//standard
static matrix standard_scaler(matrix &mat);
static void standard_scaler(matrix &mat,matrix &standard_scaler_parameters);
};