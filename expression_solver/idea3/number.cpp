#pragma GCC diagnostic error "-Wunused-result"
#include<iostream>
#include<thread>
using namespace std;

class number;
class expression;

enum expression_enums
{
LEFT_IS_NUMBER=1,
RIGHT_IS_NUMBER=2,
LEFT_IS_EXPRESSION=4,
RIGHT_IS_EXPRESSION=8
};

union _operand
{
const number *ptr2number;
const expression *ptr2expression;
};

class expression_vector
{
public:
expression *expressions;
unsigned int head;
unsigned int tail;
unsigned int size;
expression_vector();
~expression_vector();
};

class expression
{
public:
thread_local static expression_vector expr_vector;
_operand left_operand;
_operand right_operand;
number * result;
char oper;
int flags;
expression();
~expression();
__attribute__((warn_unused_result)) expression & operator+(const number &);
__attribute__((warn_unused_result)) expression & operator+(const expression &);
__attribute__((warn_unused_result)) expression & operator*(const number &);
__attribute__((warn_unused_result)) expression & operator*(const expression &);
};

class number
{
int num;
public:
number();
number(int);
int get();
__attribute__((warn_unused_result)) expression & operator+(const number &);
__attribute__((warn_unused_result)) expression & operator+(const expression &);
__attribute__((warn_unused_result)) expression & operator*(const number &);
__attribute__((warn_unused_result)) expression & operator*(const expression &);
number & operator=(expression &);
friend class expression;
};

//definations starts here
//expression_vector class
expression_vector::expression_vector()
{
cout<<"Created 1024 expressions pool"<<endl;
this->expressions=new expression[1024];
this->size=1024;
this->head=0;
this->tail=0;
}
expression_vector::~expression_vector()
{
cout<<"Destroyed "<<this->size<<" expressions pool"<<endl;
delete [] this->expressions;
}

//expression class
thread_local expression_vector expression::expr_vector;

expression::expression()
{
this->left_operand.ptr2number=NULL;
this->right_operand.ptr2number=NULL;
this->result=NULL;
this->oper=(char)0;
this->flags=0;
}
expression::~expression()
{
delete this->result;
}
__attribute__((warn_unused_result)) expression & expression::operator+(const number &right_operand)
{
if(expression::expr_vector.tail==expression::expr_vector.size)
{
//code to resize
}
expression *t;
t=expression::expr_vector.expressions+expression::expr_vector.tail;
t->oper='+';
t->left_operand.ptr2expression=this;
t->right_operand.ptr2number=&right_operand;
t->flags=LEFT_IS_EXPRESSION | RIGHT_IS_NUMBER;
++(expression::expr_vector.tail);
return *t;
}
__attribute__((warn_unused_result)) expression & expression::operator+(const expression &right_operand)
{
if(expression::expr_vector.tail==expression::expr_vector.size)
{
//code to resize
}
expression *t;
t=expression::expr_vector.expressions+expression::expr_vector.tail;
t->oper='+';
t->left_operand.ptr2expression=this;
t->right_operand.ptr2expression=&right_operand;
t->flags=LEFT_IS_EXPRESSION | RIGHT_IS_EXPRESSION;
++(expression::expr_vector.tail);
return *t;
}
__attribute__((warn_unused_result)) expression & expression::operator*(const number &right_operand)
{
if(expression::expr_vector.tail==expression::expr_vector.size)
{
//code to resize
}
expression *t;
t=expression::expr_vector.expressions+expression::expr_vector.tail;
t->oper='*';
t->left_operand.ptr2expression=this;
t->right_operand.ptr2number=&right_operand;
t->flags=LEFT_IS_EXPRESSION | RIGHT_IS_NUMBER;
++(expression::expr_vector.tail);
return *t;
}
__attribute__((warn_unused_result)) expression & expression::operator*(const expression &right_operand)
{
if(expression::expr_vector.tail==expression::expr_vector.size)
{
//code to resize
}
expression *t;
t=expression::expr_vector.expressions+expression::expr_vector.tail;
t->oper='*';
t->left_operand.ptr2expression=this;
t->right_operand.ptr2expression=&right_operand;
t->flags=LEFT_IS_EXPRESSION | RIGHT_IS_EXPRESSION;
++(expression::expr_vector.tail);
return *t;
}

//class number
number::number()
{
this->num=0;
}
number::number(int num)
{
this->num=num;
}
int number::get()
{
return this->num;
}
__attribute__((warn_unused_result)) expression & number::operator+(const number &right_operand)
{
if(expression::expr_vector.tail==expression::expr_vector.size)
{
//code to resize
}
expression *t;
t=expression::expr_vector.expressions+expression::expr_vector.tail;
t->oper='+';
t->left_operand.ptr2number=this;
t->right_operand.ptr2number=&right_operand;
t->flags=LEFT_IS_NUMBER | RIGHT_IS_NUMBER;
++(expression::expr_vector.tail);
return *t;
}
__attribute__((warn_unused_result)) expression & number::operator+(const expression &right_operand)
{
if(expression::expr_vector.tail==expression::expr_vector.size)
{
//code to resize
}
expression *t;
t=expression::expr_vector.expressions+expression::expr_vector.tail;
t->oper='+';
t->left_operand.ptr2number=this;
t->right_operand.ptr2expression=&right_operand;
t->flags=LEFT_IS_NUMBER | RIGHT_IS_EXPRESSION;
++(expression::expr_vector.tail);
return *t;
}
__attribute__((warn_unused_result)) expression & number::operator*(const number &right_operand)
{
if(expression::expr_vector.tail==expression::expr_vector.size)
{
//code to resize
}
expression *t;
t=expression::expr_vector.expressions+expression::expr_vector.tail;
t->oper='*';
t->left_operand.ptr2number=this;
t->right_operand.ptr2number=&right_operand;
t->flags=LEFT_IS_NUMBER | RIGHT_IS_NUMBER;
++(expression::expr_vector.tail);
return *t;
}
__attribute__((warn_unused_result)) expression & number::operator*(const expression &right_operand)
{
if(expression::expr_vector.tail==expression::expr_vector.size)
{
//code to resize
}
expression *t;
t=expression::expr_vector.expressions+expression::expr_vector.tail;
t->oper='*';
t->left_operand.ptr2number=this;
t->right_operand.ptr2expression=&right_operand;
t->flags=LEFT_IS_NUMBER | RIGHT_IS_EXPRESSION;
++(expression::expr_vector.tail);
return *t;
}
number & number::operator=(expression &list)
{
expression *t;
int left_num,right_num;
for(int idx=0;idx<expression::expr_vector.tail;++idx)
{
t=expression::expr_vector.expressions+idx;
if((t->flags & LEFT_IS_NUMBER) && (t->flags & RIGHT_IS_NUMBER))
{
left_num=t->left_operand.ptr2number->num;
right_num=t->right_operand.ptr2number->num;
}
else if((t->flags & LEFT_IS_EXPRESSION) && (t->flags & RIGHT_IS_NUMBER))
{
left_num=t->left_operand.ptr2expression->result->num;
right_num=t->right_operand.ptr2number->num;
}
else if((t->flags & LEFT_IS_NUMBER) && (t->flags & RIGHT_IS_EXPRESSION))
{
left_num=t->left_operand.ptr2number->num;
right_num=t->right_operand.ptr2expression->result->num;
}
else if((t->flags & LEFT_IS_EXPRESSION) && (t->flags & RIGHT_IS_EXPRESSION))
{
left_num=t->left_operand.ptr2expression->result->num;
right_num=t->right_operand.ptr2expression->result->num;
}
//evaluate
if(t->result==NULL) t->result=new number;
if('+'==t->oper) t->result->num=left_num+right_num;
else if('-'==t->oper) t->result->num=left_num-right_num;
else if('*'==t->oper) t->result->num=left_num*right_num;
else if('/'==t->oper) t->result->num=left_num/right_num;
else t->result->num=0;  //operator not supported
t->left_operand.ptr2number=NULL;
t->right_operand.ptr2number=NULL;
}
this->num=t->result->num;
expression::expr_vector.head=0;
expression::expr_vector.tail=0;
return *this;
}
//definations ends here

void test()
{
number a,b(1),c(2),d(3),e(4),f(5),g(6),h(7),i(8);


//test_1
cout<<"Before Expression"<<endl;
a=b+c;
cout<<"After Expression"<<endl;
cout<<"Result : "<<a.get()<<endl;
cout<<"Actual : "<<(1+2)<<endl;



//test_2
cout<<"Before Expression"<<endl;
a=b+(c+d);
cout<<"After Expression"<<endl;
cout<<"Result : "<<a.get()<<endl;
cout<<"Actual : "<<(1+(2+3))<<endl;



//test_2_b 
cout<<"Before Expression"<<endl;
a=b+c+d;
cout<<"After Expression"<<endl;




//test_3
cout<<"Before Expression"<<endl;
a=b+c*d;
cout<<"After Expression"<<endl;
cout<<"Result : "<<a.get()<<endl;
cout<<"Actual : "<<(1+2*3)<<endl;



//test_4
cout<<"Before Expression"<<endl;
a=(b+c)*d;
cout<<"After Expression"<<endl;
cout<<"Result : "<<a.get()<<endl;
cout<<"Actual : "<<((1+2)*3)<<endl;



//test_5
cout<<"Before Expression"<<endl;
a=b*c+d*e;
cout<<"After Expression"<<endl;
cout<<"Result : "<<a.get()<<endl;
cout<<"Actual : "<<(1*2+3*4)<<endl;



//test_6
cout<<"Before Expression"<<endl;
a=b+c*(d+e)*(f+g)*h+i;
cout<<"After Expression"<<endl;
cout<<"Result : "<<a.get()<<endl;
cout<<"Actual : "<<(1+2*(3+4)*(5+6)*7+8)<<endl;

}
int main()
{
test();
return 0;
}