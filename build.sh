#!/bin/bash

clang++ -O3 -o test --std=c++17 -I. main.cc abstractSyntaxTree.cc pythonCoreParser.cc pythonCoreTokenizer.cc
clang++ -O3 -o libPythonCore.so -shared -fPIC --std=c++17 -I. main.cc abstractSyntaxTree.cc pythonCoreParser.cc pythonCoreTokenizer.cc
strip test
strip libPythonCore.so
