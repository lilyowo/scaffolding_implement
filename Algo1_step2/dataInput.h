// dataInput.h
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

#ifndef INPUTDATA_H
#define INPUTDATA_H

// 在這裡聲明結構和函數
struct ContigInfo {
  vector<int> markers; //存該contig有哪些marker
  string contigName;   //存contig的名稱或編號
  int markerCount;     //紀錄該contig共有幾個marker
};
struct marker{
  int value;
  string contigName;
};
unordered_map<string, ContigInfo>
buildContigGraph(const string &filename);
vector<marker> markerArr(const string &filename);
#endif // INPUTDATA_H