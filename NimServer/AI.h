#pragma once
// AI.h
// The file serves to reference the functions of AI.cpp
#include <iostream>
#include <bitset>
#include <vector>

// prototypes
std::string optimalMove(std::string config);
std::vector<int> convert_from_config(std::string config);
std::string convert_to_config(std::vector<int> sizes);
int calcNinSum(std::string config);
std::string convertBinary(int dec);
int convertDec(std::string bin);
std::string exclusive_or(std::vector<std::string> bstrs);
std::string exclusive_or(int heapSize, int x);