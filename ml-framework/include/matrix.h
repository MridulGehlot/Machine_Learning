#pragma once
#include<iostream>
#include<vector>
#include<cstdint>
#include<csv.h>
#include<random_engine.h>
#define IDENTITY 2
using namespace std;
class matrix
{
uint64_t _rows;
uint64_t _columns;
uint64_t flags;
vector<double> collection;
void resize(uint64_t rows,uint64_t columns);  //not safe
public:
class matrix_row;
class matrix_cell
{
uint64_t row_index,column_index;
matrix *m;
matrix_cell(uint64_t row_index,uint64_t column_index,matrix *m);
public:
operator double();
matrix_cell & operator=(double value);
friend class matrix_row;
};
class matrix_row
{
uint64_t row_index;
matrix *m;
matrix_row(uint64_t row_index,matrix *m);
public:
matrix_cell operator[](uint64_t column_index);
friend class matrix;
};
matrix();
matrix(uint64_t rows,uint64_t columns);
matrix(uint64_t rows,uint64_t columns,uint64_t flags);
matrix(const matrix &other);
matrix(matrix && other);
matrix & operator=(const matrix &other);
matrix & operator=(matrix &&other);
~matrix();
uint64_t rows();
uint64_t columns();
void set(uint64_t row_index,uint64_t column_index,double value);
double get(uint64_t row_index,uint64_t column_index);
matrix as_transposed();
void save(string filename);
static matrix identity(uint64_t rows);
matrix_row operator[](uint64_t row_index);
//arithmetic operators
matrix operator++(); //prefix
matrix operator++(int); //postfix
matrix operator--();
matrix operator--(int);
matrix operator*(matrix &other);
matrix operator+(matrix &other);
matrix operator-(const matrix &other);
matrix operator*=(matrix &other);
matrix operator+=(matrix &other);
matrix operator-=(matrix &other);
matrix operator*(double value);
matrix operator+(double value);
matrix operator-(double value);
matrix operator/(double value);
matrix operator*=(double value);
matrix operator+=(double value);
matrix operator-=(double value);
matrix operator/=(double value);
matrix operator-();
matrix operator^(uint64_t value);
//one more mehtod required to calculate element wise power
friend class csv;
friend class random_engine;
friend matrix operator+(double value,matrix &other);
friend matrix operator-(double value,matrix &other);
friend matrix operator*(double value,matrix &other);
friend matrix operator/(double value,matrix &other);
};
//independent functions
matrix operator+(double value,matrix &other);
matrix operator-(double value,matrix &other);
matrix operator*(double value,matrix &other);
matrix operator/(double value,matrix &other);