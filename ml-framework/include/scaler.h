#pragma once
#include<matrix.h>
class scaler
{
scaler(){}
public:
static matrix min_max_scaler(matrix &mat);
static void min_max_scaler(matrix &mat,matrix &min_max_matrix);
};