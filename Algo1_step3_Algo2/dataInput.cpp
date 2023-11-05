// dataInput.cpp
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "dataInput.h" // 包含頭文件

using namespace std;

// 定義結構
// Data structure to store contig adjacency information


// 定義函數，從文件讀取數據並轉換成結構
unordered_map<string, ContigInfo>
buildContigGraph(const string &filename) {
  ifstream input(filename);
  unordered_map<string, ContigInfo> contigAdjacencyGraph;

  if (input.is_open()) {
    string line;
    while (getline(input, line)) {
      string index, marker, contigName;
      int markerCount;
      istringstream lineStream(line); //需要sstream 可拆解字串
      lineStream >> index >> marker >> contigName >> markerCount;
      if (contigAdjacencyGraph.find(contigName) ==
          contigAdjacencyGraph.end()) { //若該contig_x不存在 就加入之
        ContigInfo contigInfo;
        contigInfo.contigName = contigName;
        contigInfo.markerCount = 1;
        contigInfo.markers.push_back(stoi(marker));
        contigAdjacencyGraph[contigName] = contigInfo;
      } else {
        contigAdjacencyGraph[contigName].markers.push_back(stoi(marker));
        contigAdjacencyGraph[contigName].markerCount +=
            markerCount; //若contig_x已存在 則+1 (因為markerCount那格都會是1)
      }
    }
    input.close();
  } else { // file open有error
    cerr << "Unable to open file: " << filename << endl;
  }

  return contigAdjacencyGraph;
}

vector<marker> markerArr(const string &filename){
  ifstream input(filename);
  vector<marker> arr;

  if (input.is_open()) {
    string line;
    while (getline(input, line)) {
      string index, marker, contigName;
      int markerCount;
      istringstream lineStream(line); //需要sstream 可拆解字串
      lineStream >> index >> marker >> contigName >> markerCount;
      struct marker a;
      a.value = stoi(marker);
      a.contigName = contigName;
      arr.push_back(a);
    }
    input.close();
  } else { // file open有error
    cerr << "Unable to open file: " << filename << endl;
  }
  return arr;
}