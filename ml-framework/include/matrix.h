#pragma once
#include<iostream>
#include<dataset_utils.h>
#include<vector>
#include<cstdint>
#include<csv.h>
#include<random_engine.h>
#define IDENTITY 2
#define FMT_BINARY 2
#define FMT_CSV 4
using namespace std;
class matrix
{
uint64_t _rows;
uint64_t _columns;
uint64_t flags;
vector<double> collection;
void resize(uint64_t rows,uint64_t columns);
void save_to_binary(string filename) const;
void save_to_csv(string filename) const;
public:
class matrix_row;
class matrix_cell
{
uint64_t row_index,column_index;
matrix *m;
matrix_cell(uint64_t row_index,uint64_t column_index,matrix *m);
public:
operator double() const;
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
uint64_t rows() const;
uint64_t columns() const;
void set(uint64_t row_index,uint64_t column_index,double value);
double get(uint64_t row_index,uint64_t column_index) const;
matrix as_transposed() const;
matrix as_inversed() const;
void save(string filename,uint8_t flags) const;
static matrix identity(uint64_t rows);
matrix_row operator[](uint64_t row_index);
//arithmetic operators
matrix operator++(); //prefix
matrix operator++(int); //postfix
matrix operator--();
matrix operator--(int);
matrix operator*(const matrix &other) const;
matrix operator+(const matrix &other) const;
matrix operator-(const matrix &other) const;
matrix operator*=(const matrix &other);
matrix operator+=(const matrix &other);
matrix operator-=(const matrix &other);
matrix operator*(double value) const;
matrix operator+(double value) const;
matrix operator-(double value) const;
matrix operator/(double value) const;
matrix operator*=(double value);
matrix operator+=(double value);
matrix operator-=(double value);
matrix operator/=(double value);
matrix operator-() const;
matrix operator^(uint64_t value) const;
//one more mehtod required to calculate element wise power
friend class csv;
friend class scaler;
friend class random_engine;
friend class dataset_utils;
friend class ml_math;
friend matrix operator+(double value,const matrix &other);
friend matrix operator-(double value,const matrix &other);
friend matrix operator*(double value,const matrix &other);
friend matrix operator/(double value,const matrix &other);
};
//independent functions
matrix operator+(double value,const matrix &other);
matrix operator-(double value,const matrix &other);
matrix operator*(double value,const matrix &other);
matrix operator/(double value,const matrix &other);