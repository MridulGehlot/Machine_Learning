#include<matrix_math.h>
#include<iostream>
#include<vector>
#include<string.h>
#include<thread>
#define NUMBER_OF_ADDITION_OPERATIONS_PER_CORE 50000
#define NUMBER_OF_SUBTRACTION_OPERATIONS_PER_CORE 50000
using namespace std;
void matrix_math::multiply(double *target,double *left,uint64_t left_rows,uint64_t left_columns,double *right,uint64_t right_rows,uint64_t right_columns)
{
//always be an multi threaded operation
uint64_t target_rows=left_rows;
uint64_t target_columns=right_columns;
memset(target,0.0,(target_rows*target_columns)*sizeof(double));
uint64_t available_cores=thread::hardware_concurrency();
if(available_cores==0) available_cores=1;
uint64_t required_cores=min(available_cores,target_rows);
uint64_t batch_size=target_rows/required_cores;
uint64_t sp,ep;
int i=required_cores-1;
vector<thread *> threads(required_cores);
ep=target_rows-1;
while(i>=0)
{
sp=i*batch_size;
threads[i]=new thread([target,left,right,left_columns,right_columns,target_columns](uint64_t s,uint64_t e){
for(uint64_t c1=s;c1<=e;++c1)
{
for(uint64_t c2=0;c2<left_columns;++c2)
{
for(uint64_t c3=0;c3<right_columns;++c3)
{
//target[c1][c3]+=left[c1][c2]*right[c2][c3];
target[c1*target_columns+c3]+=left[c1*left_columns+c2]*right[c2*right_columns+c3];
}
}
}
},sp,ep);
ep=sp-1;
--i;
}
for(uint64_t k=0;k<required_cores;++k)
{
if(threads[k]->joinable()) threads[k]->join();
delete threads[k];
}
}
void matrix_math::add(double *target,double *left,double *right,size_t sz)
{
if(sz<=NUMBER_OF_ADDITION_OPERATIONS_PER_CORE)
{
for(size_t i=0;i<sz;++i) target[i]=left[i]+right[i];
return;
}
uint64_t available_cores=thread::hardware_concurrency();
if(available_cores==0) available_cores=1;
uint64_t required_cores=min(available_cores,(uint64_t)(sz/NUMBER_OF_ADDITION_OPERATIONS_PER_CORE)+1);
uint64_t batch_size=sz/required_cores;
int64_t i=required_cores-1;
vector<thread *> threads(required_cores);
uint64_t sp,ep;
ep=sz-1;
while(i>=0)
{
sp=i*batch_size;
threads[i]=new thread([target,left,right](uint64_t s,uint64_t e){
for(uint64_t k=s;k<=e;++k) target[k]=left[k]+right[k];
},sp,ep);
--i;
ep=sp-1;
}
for(i=0;i<required_cores;++i)
{
if(threads[i]->joinable()) threads[i]->join();
delete threads[i];
}
}
void matrix_math::subtract(double *target,double *left,const double *right,size_t sz)
{
if(sz<=NUMBER_OF_SUBTRACTION_OPERATIONS_PER_CORE)
{
for(size_t i=0;i<sz;++i) target[i]=left[i]-right[i];
return;
}
uint64_t available_cores=thread::hardware_concurrency();
if(available_cores==0) available_cores=1;
uint64_t required_cores=min(available_cores,(uint64_t)(sz/NUMBER_OF_SUBTRACTION_OPERATIONS_PER_CORE)+1);
uint64_t batch_size=sz/required_cores;
int64_t i=required_cores-1;
vector<thread *> threads(required_cores);
uint64_t sp,ep;
ep=sz-1;
while(i>=0)
{
sp=i*batch_size;
threads[i]=new thread([target,left,right](uint64_t s,uint64_t e){
for(uint64_t k=s;k<=e;++k) target[k]=left[k]-right[k];
},sp,ep);
--i;
ep=sp-1;
}
for(i=0;i<required_cores;++i)
{
if(threads[i]->joinable()) threads[i]->join();
delete threads[i];
}
}


void matrix_math::multiply(double *target,double *left,uint64_t left_rows,uint64_t left_columns,double right)
{
uint64_t target_rows=left_rows;
uint64_t target_columns=left_columns;
uint64_t available_cores=thread::hardware_concurrency();
if(available_cores==0) available_cores=1;
uint64_t required_cores=min(available_cores,target_rows);
uint64_t batch_size=target_rows/required_cores;
uint64_t sp,ep;
int i=required_cores-1;
vector<thread *> threads(required_cores);
ep=target_rows-1;
while(i>=0)
{
sp=i*batch_size;
threads[i]=new thread([target,target_columns,left,left_columns,right](uint64_t s,uint64_t e){
for(uint64_t ri=s;ri<=e;++ri)
{
for(uint64_t ci=0;ci<left_columns;++ci)
{
target[ri*target_columns+ci]=left[ri*left_columns+ci]*right;
}
}
},sp,ep);
ep=sp-1;
--i;
}
for(uint64_t k=0;k<required_cores;++k)
{
if(threads[k]->joinable()) threads[k]->join();
delete threads[k];
}
}
void matrix_math::add(double *target,double *left,uint64_t left_rows,uint64_t left_columns,double right)
{
uint64_t target_rows=left_rows;
uint64_t target_columns=left_columns;
uint64_t available_cores=thread::hardware_concurrency();
if(available_cores==0) available_cores=1;
uint64_t required_cores=min(available_cores,target_rows);
uint64_t batch_size=target_rows/required_cores;
uint64_t sp,ep;
int i=required_cores-1;
vector<thread *> threads(required_cores);
ep=target_rows-1;
while(i>=0)
{
sp=i*batch_size;
threads[i]=new thread([target,target_columns,left,left_columns,right](uint64_t s,uint64_t e){
for(uint64_t ri=s;ri<=e;++ri)
{
for(uint64_t ci=0;ci<left_columns;++ci)
{
target[ri*target_columns+ci]=left[ri*left_columns+ci]+right;
}
}
},sp,ep);
ep=sp-1;
--i;
}
for(uint64_t k=0;k<required_cores;++k)
{
if(threads[k]->joinable()) threads[k]->join();
delete threads[k];
}
}
void matrix_math::divide(double *target,double *left,uint64_t left_rows,uint64_t left_columns,double right)
{
uint64_t target_rows=left_rows;
uint64_t target_columns=left_columns;
uint64_t available_cores=thread::hardware_concurrency();
if(available_cores==0) available_cores=1;
uint64_t required_cores=min(available_cores,target_rows);
uint64_t batch_size=target_rows/required_cores;
uint64_t sp,ep;
int i=required_cores-1;
vector<thread *> threads(required_cores);
ep=target_rows-1;
while(i>=0)
{
sp=i*batch_size;
threads[i]=new thread([target,target_columns,left,left_columns,right](uint64_t s,uint64_t e){
for(uint64_t ri=s;ri<=e;++ri)
{
for(uint64_t ci=0;ci<left_columns;++ci)
{
target[ri*target_columns+ci]=left[ri*left_columns+ci]/right;
}
}
},sp,ep);
ep=sp-1;
--i;
}
for(uint64_t k=0;k<required_cores;++k)
{
if(threads[k]->joinable()) threads[k]->join();
delete threads[k];
}
}
void matrix_math::subtract(double *target,double *left,uint64_t left_rows,uint64_t left_columns,double right)
{
uint64_t target_rows=left_rows;
uint64_t target_columns=left_columns;
uint64_t available_cores=thread::hardware_concurrency();
if(available_cores==0) available_cores=1;
uint64_t required_cores=min(available_cores,target_rows);
uint64_t batch_size=target_rows/required_cores;
uint64_t sp,ep;
int i=required_cores-1;
vector<thread *> threads(required_cores);
ep=target_rows-1;
while(i>=0)
{
sp=i*batch_size;
threads[i]=new thread([target,target_columns,left,left_columns,right](uint64_t s,uint64_t e){
for(uint64_t ri=s;ri<=e;++ri)
{
for(uint64_t ci=0;ci<left_columns;++ci)
{
target[ri*target_columns+ci]=left[ri*left_columns+ci]-right;
}
}
},sp,ep);
ep=sp-1;
--i;
}
for(uint64_t k=0;k<required_cores;++k)
{
if(threads[k]->joinable()) threads[k]->join();
delete threads[k];
}
}


void matrix_math::multiply(double *target,double left,double *right,uint64_t right_rows,uint64_t right_columns)
{
uint64_t target_rows=right_rows;
uint64_t target_columns=right_columns;
uint64_t available_cores=thread::hardware_concurrency();
if(available_cores==0) available_cores=1;
uint64_t required_cores=min(available_cores,target_rows);
uint64_t batch_size=target_rows/required_cores;
uint64_t sp,ep;
int i=required_cores-1;
vector<thread *> threads(required_cores);
ep=target_rows-1;
while(i>=0)
{
sp=i*batch_size;
threads[i]=new thread([target,target_columns,left,right,right_columns](uint64_t s,uint64_t e){
for(uint64_t ri=s;ri<=e;++ri)
{
for(uint64_t ci=0;ci<right_columns;++ci)
{
target[ri*target_columns+ci]=left*right[ri*right_columns+ci];
}
}
},sp,ep);
ep=sp-1;
--i;
}
for(uint64_t k=0;k<required_cores;++k)
{
if(threads[k]->joinable()) threads[k]->join();
delete threads[k];
}
}
void matrix_math::add(double *target,double left,double *right,uint64_t right_rows,uint64_t right_columns)
{
uint64_t target_rows=right_rows;
uint64_t target_columns=right_columns;
uint64_t available_cores=thread::hardware_concurrency();
if(available_cores==0) available_cores=1;
uint64_t required_cores=min(available_cores,target_rows);
uint64_t batch_size=target_rows/required_cores;
uint64_t sp,ep;
int i=required_cores-1;
vector<thread *> threads(required_cores);
ep=target_rows-1;
while(i>=0)
{
sp=i*batch_size;
threads[i]=new thread([target,target_columns,left,right,right_columns](uint64_t s,uint64_t e){
for(uint64_t ri=s;ri<=e;++ri)
{
for(uint64_t ci=0;ci<right_columns;++ci)
{
target[ri*target_columns+ci]=left+right[ri*right_columns+ci];
}
}
},sp,ep);
ep=sp-1;
--i;
}
for(uint64_t k=0;k<required_cores;++k)
{
if(threads[k]->joinable()) threads[k]->join();
delete threads[k];
}
}
void matrix_math::subtract(double *target,double left,double *right,uint64_t right_rows,uint64_t right_columns)
{
uint64_t target_rows=right_rows;
uint64_t target_columns=right_columns;
uint64_t available_cores=thread::hardware_concurrency();
if(available_cores==0) available_cores=1;
uint64_t required_cores=min(available_cores,target_rows);
uint64_t batch_size=target_rows/required_cores;
uint64_t sp,ep;
int i=required_cores-1;
vector<thread *> threads(required_cores);
ep=target_rows-1;
while(i>=0)
{
sp=i*batch_size;
threads[i]=new thread([target,target_columns,left,right,right_columns](uint64_t s,uint64_t e){
for(uint64_t ri=s;ri<=e;++ri)
{
for(uint64_t ci=0;ci<right_columns;++ci)
{
target[ri*target_columns+ci]=left-right[ri*right_columns+ci];
}
}
},sp,ep);
ep=sp-1;
--i;
}
for(uint64_t k=0;k<required_cores;++k)
{
if(threads[k]->joinable()) threads[k]->join();
delete threads[k];
}
}
void matrix_math::divide(double *target,double left,double *right,uint64_t right_rows,uint64_t right_columns)
{
uint64_t target_rows=right_rows;
uint64_t target_columns=right_columns;
uint64_t available_cores=thread::hardware_concurrency();
if(available_cores==0) available_cores=1;
uint64_t required_cores=min(available_cores,target_rows);
uint64_t batch_size=target_rows/required_cores;
uint64_t sp,ep;
int i=required_cores-1;
vector<thread *> threads(required_cores);
ep=target_rows-1;
while(i>=0)
{
sp=i*batch_size;
threads[i]=new thread([target,target_columns,left,right,right_columns](uint64_t s,uint64_t e){
for(uint64_t ri=s;ri<=e;++ri)
{
for(uint64_t ci=0;ci<right_columns;++ci)
{
target[ri*target_columns+ci]=left/right[ri*right_columns+ci];
}
}
},sp,ep);
ep=sp-1;
--i;
}
for(uint64_t k=0;k<required_cores;++k)
{
if(threads[k]->joinable()) threads[k]->join();
delete threads[k];
}
}