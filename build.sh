#!/bin/bash

clang++ -o test --std=c++17 -I. main.cc abstractSyntaxTree.cc pythonCoreParser.cc pythonCoreTokenizer.cc
