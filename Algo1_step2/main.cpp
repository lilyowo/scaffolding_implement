#include "dataInput.h"
#include "findCI.h"
#include "findICI.h"
#include "ConnectICI.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
/**
1. 找出所有的conserved interval V
2. 利用conserved interval找出irredusable conserved interval
3. 找irredusable conserved interval有誰是nested的然後抓出最內層的most inner ICI
4. 把會形成most inner ICI的contigs連起來
*/
using namespace std;

//將merge好的contig寫入output file中
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


void writeCIToFile(const vector<ConservedInterval> &CI, string outFileName) {
  ofstream output(outFileName); // Open the output file

  if (output.is_open()) {
    int lineNumber = 1;
    for(size_t i=0; i<CI.size(); i++) {
      //cout<<"ICI[ "<<i<<" ]: a1= "<<ICI[i].ida1<<", b1= "<<ICI[i].idb1 <<endl;
      output <<"CI[ "<<i<<" ]: a1= "<<CI[i].ida1<<", b1= "<<CI[i].idb1 <<"\n";
      
      
    }
    output.close(); // Close the output file
  } else {
    cerr << "Unable to open output file." << endl;
  }
}



int main() {
  // 讀取 input 檔案並建立 contig adjacency graph
  unordered_map<string, ContigInfo> genome1 =
      buildContigGraph("query_merged.txt");
  writeToFile(genome1, "Original.txt");
  cout<<"Contig Graph builded"<<endl;
  //writeToFile(genome1, "try.txt");
  vector<marker> G1markerArr = markerArr("query_merged.txt");
  vector<marker> G2markerArr = markerArr("reference.all");
  cout<<"G1 G2 array maked"<<endl;
  // for (size_t i = 0; i < G1markerArr.size(); ++i)
  //   cout << G1markerArr[i].value <<endl;
  vector<ConservedInterval> 
 CI = findConservedInterval(G1markerArr, G2markerArr);
  cout<<"Conserved Interval found"<<endl;
  writeCIToFile(CI, "myCI.txt");
  // for(size_t i=0; i<CI.size(); i++) cout<<"CI[ "<<i<<" ]: a1= "<<CI[i].ida1<<", b1= "<<CI[i].idb1 <<endl;

  // cout<<"Irriducible conserved interval found"<<endl;
  vector<ConservedInterval> ICI = findICI(CI);
  writeCIToFile(ICI, "myICI.txt");
  cout<<"Irriducible conserved interval found"<<endl;

  unordered_map<string, ContigInfo>mergedGenome = ConnectICI(ICI,genome1);
  writeToFile(mergedGenome, "ICI_Merged.txt");
  cout<<"ICI be merged!"<<endl;
  
  cout << "owo complete!";

  return 0;
}