del *.o
del ..\lib\libml_framework.a
g++  -O3 -fopenmp -I ..\include -c *.cpp
ar rcs ..\lib\libml_framework.a *.o