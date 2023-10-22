// Connect ICI.h
/*

*/
#include <iostream>
#include <string>
#include <vector>
#include "findCI.h"
#include "dataInput.h"
using namespace std;

#ifndef CONNECTICI_H
#define CONNECTICI_H
//here your code
unordered_map<string, ContigInfo> ConnectICI(vector<ConservedInterval> ICI, unordered_map<string, ContigInfo> G1);
#endif 