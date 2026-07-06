#include<matrix.h>
#include<iostream>
#include<vector>
#include<ml_exception.h>
#include<matrix_math.h>
#include<string.h>
using namespace std;

//private method
void matrix::resize(uint64_t rows,uint64_t columns)
{
this->_rows=rows;
this->_columns=columns;
this->flags=0;
this->collection.resize(this->_rows*this->_columns);
}

//constructors & operator=
matrix::matrix()
{
this->_rows=0;
this->_columns=0;
this->flags=0;
collection.resize(0);
}
matrix::matrix(uint64_t rows,uint64_t columns)
{
this->_rows=rows;
this->_columns=columns;
this->flags=0;
collection.resize(this->_rows*this->_columns);
}
matrix::matrix(uint64_t rows,uint64_t columns,uint64_t flags)
{
this->_rows=rows;
this->_columns=columns;
this->flags=flags;
}
matrix::matrix(const matrix &other)
{
this->_rows=other._rows;
this->_columns=other._columns;
this->flags=other.flags;
this->collection=other.collection;
}
matrix::matrix(matrix && other)
{
this->_rows=other._rows;
this->_columns=other._columns;
this->flags=other.flags;
this->collection=move(other.collection);
other._rows=0;
other._columns=0;
other.flags=0;
}
matrix & matrix::operator=(const matrix &other)
{
this->_rows=other._rows;
this->_columns=other._columns;
this->collection=other.collection;
this->flags=other.flags;
return *this;
}
matrix & matrix::operator=(matrix &&other)
{
this->_rows=other._rows;
this->_columns=other._columns;
this->flags=other.flags;
this->collection=move(other.collection);
other._rows=0;
other._columns=0;
other.flags=0;
return *this;
}
matrix::~matrix()
{
//do nothing
}

//public methods
uint64_t matrix::rows()
{
return this->_rows;
}
uint64_t matrix::columns()
{
return this->_columns;
}
void matrix::set(uint64_t row_index,uint64_t column_index,double value)
{
if(row_index>=this->_rows || column_index>=this->_columns)
{
throw new ml_exception("Index out of bounds");
}
if(flags & IDENTITY)
{
if(value==0.0 && row_index!=column_index) return;
else if(value==1.0 && row_index==column_index) return;
//remove identity flag
this->flags^=IDENTITY;
this->collection.resize(this->_rows,this->_columns);
//set everything to 0
memset(this->collection.data(),0.0,this->collection.size()*sizeof(double));
uint64_t idx;
for(idx=0;idx<this->collection.size();idx+=(this->_columns+1)) this->collection[idx]=1.0;
}
this->collection[row_index*this->_columns+column_index]=value;
}
double matrix::get(uint64_t row_index,uint64_t column_index)
{
if(row_index>=this->_rows || column_index>=this->_columns)
{
throw new ml_exception("Index out of bounds");
}
if(flags & IDENTITY)
{
if(row_index==column_index) return 1.0;
else return 0.0;
}
return this->collection[row_index*this->_columns+column_index];
}
matrix matrix::as_transposed()
{
matrix tmp(this->_columns,this->_rows);
for(uint64_t i=0;i<this->_rows;++i)
{
for(uint64_t j=0;j<this->_columns;++j)
{
//tmp[j][i]=this->collection[i][j];
tmp.collection[j*tmp._columns+i]=this->collection[i*this->_columns+j];
}
}
return tmp;
}
void matrix::save(string filename)
{
//Little Endian and Big Endian
//we will covenrt to network byte order and write and use same to read
//for now i am not implementing this
FILE *f=fopen(filename.c_str(),"wb");
if(f==nullptr) throw new ml_exception("Unable to save in a file : "+filename);
fwrite(&(this->_rows),sizeof(uint64_t),1,f);
fwrite(&(this->_columns),sizeof(uint64_t),1,f);
fwrite(this->collection.data(),this->_rows*this->_columns*sizeof(double),1,f);
fclose(f);
}
matrix matrix::identity(uint64_t rows)
{
return matrix(rows,rows,IDENTITY);
}


//inner class implementations
matrix::matrix_cell::matrix_cell(uint64_t row_index,uint64_t column_index,matrix *m)
{
this->row_index=row_index;
this->column_index=column_index;
this->m=m;
}
matrix::matrix_cell:: operator double()
{
return this->m->get(this->row_index,this->column_index);
}
matrix::matrix_cell & matrix::matrix_cell::operator=(double value)
{
this->m->set(this->row_index,this->column_index,value);
return *this;
}
//matrix_row
matrix::matrix_row::matrix_row(uint64_t row_index,matrix *m)
{
this->row_index=row_index;
this->m=m;
}

//for operator[] access
matrix::matrix_row matrix::operator[](uint64_t row_index)
{
if(row_index>=this->_rows) throw new ml_exception("Index out of bounds");
return matrix::matrix_row(row_index,this);
}
matrix::matrix_cell matrix::matrix_row::operator[](uint64_t column_index)
{
if(column_index>=this->m->_columns) throw new ml_exception("Index out of bounds");
return matrix::matrix_cell(this->row_index,column_index,this->m);
}

//arithmetic operators
matrix matrix::operator++(void) //prefix
{
matrix_math::add(this->collection.data(),this->collection.data(),this->_rows,this->_columns,1.0);
return *this;
}
matrix matrix::operator++(int) //postfix
{
matrix tmp;
tmp=*this;
matrix_math::add(this->collection.data(),this->collection.data(),this->_rows,this->_columns,1.0);
return tmp;
}
matrix matrix::operator--()
{
matrix_math::subtract(this->collection.data(),this->collection.data(),this->_rows,this->_columns,1.0);
return *this;
}
matrix matrix::operator--(int)
{
matrix tmp;
tmp=*this;
matrix_math::subtract(this->collection.data(),this->collection.data(),this->_rows,this->_columns,1.0);
return tmp;
}
matrix matrix::operator*(matrix &other)
{
if(this->_columns!=other._rows) throw new ml_exception("Invalid Dimensions (row - column mismatch) ");
matrix tmp(this->_rows,other._columns);
matrix_math::multiply(tmp.collection.data(),this->collection.data(),this->_rows,this->_columns,other.collection.data(),other._rows,other._columns);
return tmp;
}
matrix matrix::operator+(matrix &other)
{
if(this->_rows!=other._rows || this->_columns!=other._columns) throw new ml_exception("Invalid Dimensions (row - column mismatch) ");
matrix tmp(this->_rows,this->_columns);
matrix_math::add(tmp.collection.data(),this->collection.data(),other.collection.data(),this->collection.size());
return tmp;
}
matrix matrix::operator-(const matrix &other)
{
if(this->_rows!=other._rows || this->_columns!=other._columns) throw new ml_exception("Invalid Dimensions (row - column mismatch) ");
matrix tmp(this->_rows,this->_columns);
matrix_math::subtract(tmp.collection.data(),this->collection.data(),other.collection.data(),this->collection.size());
return tmp;
}
matrix matrix::operator*=(matrix &other)
{
if(this->_columns!=other._rows) throw new ml_exception("Invalid Dimensions (row - column mismatch) ");
matrix tmp(this->_rows,this->_columns);
matrix_math::multiply(tmp.collection.data(),this->collection.data(),this->_rows,this->_columns,other.collection.data(),other._rows,other._columns);
return tmp;
}
matrix matrix::operator+=(matrix &other)
{
if(this->_rows!=other._rows || this->_columns!=other._columns) throw new ml_exception("Invalid Dimensions (row - column mismatch) ");
matrix_math::add(this->collection.data(),this->collection.data(),other.collection.data(),this->collection.size());
return *this;
}
matrix matrix::operator-=(matrix &other)
{
if(this->_rows!=other._rows || this->_columns!=other._columns) throw new ml_exception("Invalid Dimensions (row - column mismatch) ");
matrix_math::subtract(this->collection.data(),this->collection.data(),other.collection.data(),this->collection.size());
return *this;
}
matrix matrix::operator*(double value)
{
matrix tmp(this->_rows,this->_columns);
matrix_math::multiply(tmp.collection.data(),this->collection.data(),this->_rows,this->_columns,value);
return tmp;
}
matrix matrix::operator+(double value)
{
matrix tmp(this->_rows,this->_columns);
matrix_math::add(tmp.collection.data(),this->collection.data(),this->_rows,this->_columns,value);
return tmp;
}
matrix matrix::operator-(double value)
{
matrix tmp(this->_rows,this->_columns);
matrix_math::subtract(tmp.collection.data(),this->collection.data(),this->_rows,this->_columns,value);
return tmp;
}
matrix matrix::operator/(double value)
{
if(value==0) throw ml_exception("Division by 0 not allowed");
matrix tmp(this->_rows,this->_columns);
matrix_math::divide(tmp.collection.data(),this->collection.data(),this->_rows,this->_columns,value);
return tmp;
}
matrix matrix::operator*=(double value)
{
matrix_math::multiply(this->collection.data(),this->collection.data(),this->_rows,this->_columns,value);
return *this;
}
matrix matrix::operator+=(double value)
{
matrix_math::add(this->collection.data(),this->collection.data(),this->_rows,this->_columns,value);
return *this;
}
matrix matrix::operator-=(double value)
{
matrix_math::subtract(this->collection.data(),this->collection.data(),this->_rows,this->_columns,value);
return *this;
}
matrix matrix::operator/=(double value)
{
if(value==0) throw ml_exception("Division by 0 not allowed");
matrix_math::divide(this->collection.data(),this->collection.data(),this->_rows,this->_columns,value);
return *this;
}
matrix matrix::operator-()
{
return (*this)*(-1);
}
matrix matrix::operator^(uint64_t value)
{
if(this->_rows!=this->_columns) throw ml_exception("For Matrix ^power operations it should be square matrix");
if(value==0) //return identity matrix
{
return matrix::identity(this->_rows);
}
if(value==1) return *this;
matrix tmp;
tmp=*this;
for(uint64_t i=1;i<value;++i) tmp*=(*this);
return tmp;
}


//independent functions
matrix operator+(double value,matrix &other)
{
matrix tmp(other._rows,other._columns);
matrix_math::add(tmp.collection.data(),value,other.collection.data(),other._rows,other._columns);
return tmp;
}
matrix operator-(double value,matrix &other)
{
matrix tmp(other._rows,other._columns);
matrix_math::subtract(tmp.collection.data(),value,other.collection.data(),other._rows,other._columns);
return tmp;
}
matrix operator*(double value,matrix &other)
{
matrix tmp(other._rows,other._columns);
matrix_math::multiply(tmp.collection.data(),value,other.collection.data(),other._rows,other._columns);
return tmp;
}
matrix operator/(double value,matrix &other)
{
matrix tmp(other._rows,other._columns);
matrix_math::divide(tmp.collection.data(),value,other.collection.data(),other._rows,other._columns);
return tmp;
}