#pragma once
#include<vector>
#include<iostream>
using namespace std;

typedef struct _shape
{
char type;
}shape;
typedef struct _shape_point
{
char type;
double x,y;
}shape_point;
typedef struct _shape_line
{
char type;
shape_point *point_1,*point_2;
}shape_line;

shape_point * point(double x,double y);
shape_line * line(shape_point *point_1,shape_point *point_2);

class plotter
{
private:
vector<shape *> shapes;
pair<int,int> x_range;
pair<int,int> y_range;
pair<int,int> size;
pair<int,int> location;
string x_label;
string y_label;
FILE *gnuplot;
string title;
public:
plotter();
void add(shape_point *point);
void add(shape_line *line);
void remove(char type);
void clear();
void plot();
void set_x_range(pair<int,int> x_range);
void set_y_range(pair<int,int> y_range);
void set_title(string title);
void set_size(pair<int,int> size);
void set_location(pair<int,int> location);
void set_x_label(string x_label);
void set_y_label(string y_label);
};

/*
//Sample Main Function
int main()
{
plotter graph;
point p(10,20);
graph.add(p);
graph.add(point(100,200));
graph.add(point(100,200,RED,2)); //size is 2
graph.add(line(point a,point b,GREEN,4)); //4 is thickness of line
graph.setXLabel("Some String");
graph.setYLabel("Some String");
graph.setTitle("Some String");
graph.setX(10,100,5); start,stop,step
graph.setY(10,100,5); start,stop,step
graph.plot();
graph.remove(point(10,20));
graph.plot();
graph.clear();
return 0;
}
*/