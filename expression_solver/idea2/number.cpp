#include<iostream>
using namespace std;

class number;

typedef union _element
{
char _operator;
const number * _operand;
}element;

class expression_node
{
public:
element elem;
expression_node *root_node;
expression_node *left;
expression_node *right;
expression_node *parent;
expression_node();
~expression_node();
expression_node & operator+(number &);
expression_node & operator+(expression_node &);
expression_node & operator*(number &);
expression_node & operator*(expression_node &);
};

class number
{
int num;
expression_node root_node;
expression_node left_node;
expression_node right_node;
public:
number();
number(int);
expression_node & operator+(const number &);
expression_node & operator+(expression_node &);
expression_node & operator*(const number &);
expression_node & operator*(expression_node &);
number & operator=(expression_node &);
void traverse_post_order(expression_node *);
friend class expression_node;
};

//definations
//expression_node
expression_node::expression_node()
{
this->left=NULL;
this->right=NULL;
this->parent=NULL;
this->root_node=NULL;
}
expression_node::~expression_node()
{
delete this->root_node;
}
expression_node & expression_node::operator+(number &right_operand)
{
cout<<"expression_node & expression_node::operator+(number &right_operand)"<<endl;
right_operand.root_node.elem._operator='+';
right_operand.root_node.left=this;
right_operand.root_node.right=&(right_operand.right_node);
right_operand.right_node.elem._operand=&right_operand;
right_operand.right_node.left=NULL;
right_operand.right_node.right=NULL;
right_operand.root_node.parent=NULL;
//set parents
this->parent=&(right_operand.root_node);
right_operand.right_node.parent=&(right_operand.root_node);
return right_operand.root_node;
}
expression_node & expression_node::operator+(expression_node &right_operand)
{
cout<<"expression_node & expression_node::operator+(expression_node &right_operand)"<<endl;
if(right_operand.root_node)
{
//memory is already allocated lets reuse it
right_operand.root_node->elem._operator='+';
right_operand.root_node->left=this;
right_operand.root_node->right=&(right_operand);
this->parent=right_operand.root_node;
right_operand.parent=right_operand.root_node;
return *(right_operand.root_node);
}
else
{
this->root_node=new expression_node;
this->root_node->elem._operator='+';
this->root_node->left=this;
this->root_node->right=&right_operand;
this->parent=this->root_node;
right_operand.parent=this->root_node;
return *(this->root_node);
}
}
expression_node & expression_node::operator*(number &right_operand)
{
cout<<"expression_node & expression_node::operator*(number &right_operand)"<<endl;
right_operand.root_node.elem._operator='*';
right_operand.root_node.left=this;
right_operand.root_node.right=&(right_operand.right_node);
right_operand.right_node.elem._operand=&right_operand;
right_operand.right_node.left=NULL;
right_operand.right_node.right=NULL;
right_operand.root_node.parent=NULL;
//set parents
this->parent=&(right_operand.root_node);
right_operand.right_node.parent=&(right_operand.root_node);
return right_operand.root_node;
}
expression_node & expression_node::operator*(expression_node &right_operand)
{
cout<<"expression_node & expression_node::operator*(expression_node &right_operand)"<<endl;
if(right_operand.root_node)
{
//memory is already allocated lets reuse it
right_operand.root_node->elem._operator='*';
right_operand.root_node->left=this;
right_operand.root_node->right=&(right_operand);
this->parent=right_operand.root_node;
right_operand.parent=right_operand.root_node;
return *(right_operand.root_node);
}
else
{
this->root_node=new expression_node;
this->root_node->elem._operator='*';
this->root_node->left=this;
this->root_node->right=&right_operand;
this->parent=this->root_node;
right_operand.parent=this->root_node;
return *(this->root_node);
}
}

//number
number::number()
{
this->num=0;
}
number::number(int num)
{
this->num=num;
}
expression_node & number::operator+(const number &right_operand)
{
cout<<"expression_node & number::operator+(const number &right_operand)"<<endl;
this->root_node.elem._operator='+';
this->root_node.parent=NULL;
this->left_node.elem._operand=this;
this->left_node.left=NULL;
this->left_node.right=NULL;
this->right_node.elem._operand=&right_operand;
this->right_node.left=NULL;
this->right_node.right=NULL;

this->left_node.parent=&(this->root_node);
this->right_node.parent=&(this->root_node);

this->root_node.left=&(this->left_node);
this->root_node.right=&(this->right_node);
return this->root_node;
}
expression_node & number::operator+(expression_node &right_operand)
{
cout<<"expression_node & number::operator+(expression_node &right_operand)"<<endl;
this->root_node.elem._operator='+';
this->root_node.parent=NULL;
this->left_node.elem._operand=this;
this->left_node.left=NULL;
this->left_node.right=NULL;

this->left_node.parent=&(this->root_node);
right_operand.parent=&(this->root_node);

this->root_node.left=&(this->left_node);
this->root_node.right=&right_operand;
return this->root_node;
}
expression_node & number::operator*(const number &right_operand)
{
cout<<"expression_node & number::operator*(const number &right_operand)"<<endl;
this->root_node.elem._operator='*';
this->root_node.parent=NULL;
this->left_node.elem._operand=this;
this->left_node.left=NULL;
this->left_node.right=NULL;
this->right_node.elem._operand=&right_operand;
this->right_node.left=NULL;
this->right_node.right=NULL;

this->left_node.parent=&(this->root_node);
this->right_node.parent=&(this->root_node);

this->root_node.left=&(this->left_node);
this->root_node.right=&(this->right_node);
return this->root_node;
}
expression_node & number::operator*(expression_node &right_operand)
{
cout<<"expression_node & number::operator*(expression_node &right_operand)"<<endl;
this->root_node.elem._operator='*';
this->root_node.parent=NULL;
this->left_node.elem._operand=this;
this->left_node.left=NULL;
this->left_node.right=NULL;

this->left_node.parent=&(this->root_node);
right_operand.parent=&(this->root_node);

this->root_node.left=&(this->left_node);
this->root_node.right=&right_operand;
return this->root_node;
}
number & number::operator=(expression_node &tree)
{
cout<<"number & number::operator=(expression_node &tree)"<<endl;
traverse_post_order(&tree);
return *this;
}
void number::traverse_post_order(expression_node *root)
{
if(!root) return;
traverse_post_order(root->left);
traverse_post_order(root->right);
if(root->left==NULL && root->right==NULL) cout<<"operand : "<<root->elem._operand->num<<endl;
else cout<<"operator : "<<root->elem._operator<<endl;
}

int main()
{
/*
//test_1
number a,b(1),c(2);
cout<<"Before Expression"<<endl;
a=b+c;
cout<<"After Expression"<<endl;
*/

/*
//test_2
number a,b(1),c(2),d(3);
cout<<"Before Expression"<<endl;
a=b+(c+d);
cout<<"After Expression"<<endl;
*/

/*
//test_2_b 
number a,b(1),c(2),d(3);
cout<<"Before Expression"<<endl;
a=b+c+d;
cout<<"After Expression"<<endl;
*/

/*
//test_3
number a,b(1),c(2),d(3);
cout<<"Before Expression"<<endl;
a=b+c*d;
cout<<"After Expression"<<endl;
*/

/*
//test_4
number a,b(1),c(2),d(3);
cout<<"Before Expression"<<endl;
a=(b+c)*d;
cout<<"After Expression"<<endl;
*/

/*
//test_5
number a,b(1),c(2),d(3),e(4);
cout<<"Before Expression"<<endl;
a=a*b+c*d;
cout<<"After Expression"<<endl;
*/

//test_6
number a,b(1),c(2),d(3),e(4),f(5),g(6),h(7),i(8);
cout<<"Before Expression"<<endl;
a=b+c*(d+e)*(f+g)*h+i;
cout<<"After Expression"<<endl;


return 0;
}