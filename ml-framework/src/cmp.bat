del *.o
del ..\lib\libml_framework.a
g++ -I ..\include -c *.cpp
ar rcs ..\lib\libml_framework.a *.o