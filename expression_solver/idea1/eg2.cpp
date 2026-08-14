/*
Trick 
take 3 objects root,left,right inside number class 
Each Object size can be 24 bytes if 3 pointers in expression_node
or Each Object size can be 25 bytes for current Implementation

so for 3 objects size = 3*24=72 bytes;
suppose we have 1,00,000 Objects than 7.2 MB Approx

Consider Expressions Like
a= b*c + d*e ;
expression_node [operator+] (expression_node)

a= b*c + d;
expression_node [operator+] (number)

Means we have to overload all operators for expression_node too with 2 type of args (number | expression_node)

BIG ISSUE : 
a = b*c + d

we have tree
b->root *
b->left b
b->right c

now we want new node 
new node *
left     +  (root of b)
right    d  (number)

from where we will get this new node ??
Design Failure
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
expression_node root_node;
expression_node left_node;
expression_node right_node;
public:
number()
{
this->num=0;
}
number(int num)
{
this->num=num;
}
expression_node & operator+(number &right_operand)
{
this->root_node._operator='+';
this->root_node.left=&this->left_node;
this->root_node.right=&this->right_node;

this->left_node.operand=this;
this->left_node.left=NULL;
this->left_node.right=NULL;

this->right_node.operand=&right_operand;
this->right_node.left=NULL;
this->right_node.right=NULL;

return this->root_node;
}
expression_node & operator+(expression_node &right_operand)
{
this->root_node._operator='+';
this->root_node.left=&this->left_node;
this->root_node.right=&this->right_node;

this->left_node.operand=this;
this->left_node.left=NULL;
this->left_node.right=NULL;

/*
Operand is of number * type cannot assign this
Better instead we assign this right_operand to ROOT instead of taking right object
this->right_node.operand=&right_operand;
this->right_node.left=NULL;
this->right_node.right=NULL;
*/
this->root_node.right=&right_operand;

return this->root_node;
}
number & operator=(const expression_node &tree)
{
cout<<"Assignment operator = "<<endl;
return *this;
}
};

int main()
{
number a,b(10),c(20);
cout<<"Before Expression"<<endl;
a=b+c;
cout<<"After Expression"<<endl;
return 0;
}