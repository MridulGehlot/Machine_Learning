rem g++  %1   -fopenmp  -O3  -I  ..\include   -L ..\lib   -l _ml-framework  -o  %2
g++  test_graph.cpp   -fopenmp  -O3  -I  ..\include   -L ..\lib   -l _ml-framework  -o  test_graph.exe