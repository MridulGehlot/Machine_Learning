#pragma once
#include<stdexcept>
#include<iostream>
using namespace std;
class ml_exception : public runtime_error
{
public:
ml_exception(string message);
};