/*
Big Issue : 
a=b+c

operator+ run for b and c is passed to it
return expression_node
But there are 3 local objects root,left,right
we return root so root is preserved until assignment=
But left and right die as soon as function ends

o/p
Before Expression
Constructor		ROOT
Constructor		LEFT
Constructor		RIGHT
Destructor		LEFT
Destructor		RIGHT
Assignment operator =	ISSUE HERE (LEFT & RIGHT) dead
Destructor		ROOT
After Expression

*/

#include<iostream>
using namespace std;

class number;

class expression_node
{
public:
char _operator;
number *operand;
expression_node *left;
expression_node *right;
expression_node()
{
cout<<"Constructor"<<endl;
this->_operator='\0';
this->left=this->right=NULL;
}
~expression_node()
{
cout<<"Destructor"<<endl;
}
};

class number
{
int num;
public:
number()
{
this->num=0;
}
number(int num)
{
this->num=num;
}
expression_node operator+(number &right_operand)
{
expression_node root,left,right;
root._operator='+';
left.operand=this;
right.operand=&right_operand;
root.left=&left;
root.right=&right;
return root;
}
expression_node operator+(expression_node &right_operand)
{
expression_node root,left,right;
root._operator='+';
left.operand=this;
root.left=&left;
root.right=&right;
return root;
}
number & operator=(const expression_node &tree)
{
cout<<"Assignment operator = "<<endl;
return *this;
}
};

int main()
{
number a,b(10),c(20),d(30);
cout<<"Before Expression"<<endl;
a=b+c;
cout<<"After Expression"<<endl;
return 0;
}