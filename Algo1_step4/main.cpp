#include "dataInput.h"
#include "breakpointDistance.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
/*
讀取contig檔案進來
全部marker照順序丟進list/vector
然後兩個genome計算break poing distance
*/
void writeToFile(const unordered_map<string, ContigInfo> &mergedGraph,
                       string outFileName) {
  ofstream output(outFileName); // Open the output file

  if (output.is_open()) {
    int lineNumber = 1;
    for (const auto &entry : mergedGraph) {
      const ContigInfo &contigInfo = entry.second;
      for (int marker : contigInfo.markers) {
        // Write the formatted information to the file
        output << lineNumber << "\t" << marker << "\t" << contigInfo.contigName
               << "\t1\n";
        lineNumber++;
      }
    }
    output.close(); // Close the output file
  } else {
    cerr << "Unable to open output file." << endl;
  }
}




int main() {
  unordered_map<string, ContigInfo> genome1 =
      buildContigGraph("ICI_Merged.txt");
  writeToFile(genome1, "step2_output.txt");
  cout<<"Contig Graph builded"<<endl;
  //writeToFile(genome1, "try.txt");
  vector<marker> G1markerArr = markerArr("ICI_Merged.txt");
  vector<marker> G2markerArr = markerArr("reference.all");
  vector<marker> OriginmarkerArr = markerArr("query.all");
  cout<<"G1 G2 array maked"<<endl;
  vector<int> G1, G2, Origin;
  for(size_t i=0;i<G1markerArr.size();i++){
    G1.push_back(G1markerArr[i].value);
  }
  for(size_t i=0;i<G2markerArr.size();i++){
    G2.push_back(G2markerArr[i].value);
  }
  for(size_t i=0;i<OriginmarkerArr.size();i++){
    Origin.push_back(OriginmarkerArr[i].value);
  }

  // Calculate the breakpoint distance
  int breakpointDistance = BreakpointDistance(G1, G2);
  cout << "Breakpoint distance: " << breakpointDistance << endl;
  int originbp = BreakpointDistance(Origin, G2);
  cout << "Origin bp distance: " << originbp << endl;

  cout << "owo complete!";
}