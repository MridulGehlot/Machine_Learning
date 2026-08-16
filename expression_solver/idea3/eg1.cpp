#pragma GCC diagnostic error "-Wunused-result"
#include<iostream>
using namespace std;
__attribute__((warn_unused_result)) int sam()
{
return 100;
}
int main()
{
sam();
return 0;
}