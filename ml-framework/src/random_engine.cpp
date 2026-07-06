#include<iostream>
#include<random_engine.h>
#include<matrix.h>
#include<random>
using namespace std;
void random_engine::fill(matrix &other,double lower_bound,double upper_bound)
{
random_device random_device_object;
mt19937 engine(random_device_object());
uniform_int_distribution<int> d(lower_bound,upper_bound);
for(uint64_t idx=0;idx<other.collection.size();++idx) other.collection[idx]=d(engine);
}