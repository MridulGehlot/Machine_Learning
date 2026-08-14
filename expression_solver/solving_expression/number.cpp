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
number *accumulator;
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
operator int()
{
return this->num;
}
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
this->accumulator=NULL;
}
expression_node::~expression_node()
{
delete this->root_node;
delete this->accumulator;
}
expression_node & expression_node::operator+(number &right_operand)
{
//cout<<"expression_node & expression_node::operator+(number &right_operand)"<<endl;
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
//cout<<"expression_node & expression_node::operator+(expression_node &right_operand)"<<endl;
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
//cout<<"expression_node & expression_node::operator*(number &right_operand)"<<endl;
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
//cout<<"expression_node & expression_node::operator*(expression_node &right_operand)"<<endl;
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
//cout<<"expression_node & number::operator+(const number &right_operand)"<<endl;
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
//cout<<"expression_node & number::operator+(expression_node &right_operand)"<<endl;
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
//cout<<"expression_node & number::operator*(const number &right_operand)"<<endl;
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
//cout<<"expression_node & number::operator*(expression_node &right_operand)"<<endl;
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
//cout<<"number & number::operator=(expression_node &tree)"<<endl;
//Evaluation Logic
expression_node *t,*parent,*left_sibling,*right_sibling;
char op;
int res,l,r;
t=&tree;
while(!(tree.left==NULL && tree.right==NULL))
{
if(t->left==NULL && t->right==NULL) //it is a leaf node
{
parent=t->parent;
if(parent->left==t)
{
left_sibling=t;
right_sibling=parent->right;
}
else 
{
left_sibling=parent->left;
right_sibling=t;
}
//Now Check if Both Siblings are Leaf Node
if(left_sibling->left!=NULL) t=left_sibling->left;
else if(left_sibling->right!=NULL) t=left_sibling->right;
else if(right_sibling->left!=NULL) t=right_sibling->left;
else if(right_sibling->right!=NULL) t=right_sibling->right;
else //evaluate the expression
{
op=parent->elem._operator;
l=left_sibling->accumulator?left_sibling->accumulator->num:left_sibling->elem._operand->num;
r=right_sibling->accumulator?right_sibling->accumulator->num:right_sibling->elem._operand->num;
if('+'==op) res=l+r;
else if('-'==op) res=l-r;
else if('*'==op) res=l*r;
else if('/'==op) res=l/r;
else res=0; 		//invalid operator
if(!parent->accumulator) parent->accumulator=new number;
parent->accumulator->num=res;
//cout<<"l = "<<l<<", r = "<<r<<" , res = "<<res<<endl;
parent->left=parent->right=NULL;
t=parent;
}
}
else
{
if(t->left==NULL) t=t->right;
else t=t->left;
}
}
this->num=t->accumulator->num;
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
number a,b(1),c(2),d(3),e(4),f(5),g(6),h(7),i(8);


/*
//test_1
cout<<"Before Expression"<<endl;
a=b+c;
cout<<"After Expression"<<endl;
cout<<"Result : "<<a<<endl;
cout<<"Actual : "<<(1+2)<<endl;
*/

/*
//test_2
cout<<"Before Expression"<<endl;
a=b+(c+d);
cout<<"After Expression"<<endl;
cout<<"Result : "<<a<<endl;
cout<<"Actual : "<<(1+(2+3))<<endl;
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
cout<<"Before Expression"<<endl;
a=b+c*d;
cout<<"After Expression"<<endl;
cout<<"Result : "<<a<<endl;
cout<<"Actual : "<<(1+2*3)<<endl;
*/

/*
//test_4
cout<<"Before Expression"<<endl;
a=(b+c)*d;
cout<<"After Expression"<<endl;
cout<<"Result : "<<a<<endl;
cout<<"Actual : "<<((1+2)*3)<<endl;
*/

/*
//test_5
cout<<"Before Expression"<<endl;
a=a*b+c*d;
cout<<"After Expression"<<endl;
cout<<"Result : "<<a<<endl;
cout<<"Actual : "<<(1*2+3*4)<<endl;
*/


//test_6
cout<<"Before Expression"<<endl;
a=b+c*(d+e)*(f+g)*h+i;
cout<<"After Expression"<<endl;
cout<<"Result : "<<a<<endl;
cout<<"Actual : "<<(1+2*(3+4)*(5+6)*7+8)<<endl;


return 0;
}