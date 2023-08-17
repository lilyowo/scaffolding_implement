#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
// Data structure to store contig adjacency information
struct ContigInfo {
  vector<int> markers; //存該contig有哪些marker
  string contigName;   //存contig的名稱或編號
  int markerCount;     //紀錄該contig共有幾個marker
};

// Function to read input data and build contig adjacency graph
//讀入data.txt return 儲存好contig_x跟他的數量的map
unordered_map<string, ContigInfo>
buildContigAdjacencyGraph(const string &filename) {
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

// Function to merge contigs based on adjacency
unordered_map<string, ContigInfo>
mergeContigs(unordered_map<string, ContigInfo> &graphIn) {
  unordered_map<string, ContigInfo> graph = graphIn;
  unordered_map<string, ContigInfo> mergedGraph;
  int cnt = 0;
  while(true){
      if(graph.empty()) break;
      if(cnt==100) break;
      cnt++;
      const string &contigName = graph.begin()->first;
      const ContigInfo &contigInfo = graph.begin()->second;
      bool merged = false;
      string keyA, keyB;
      for(const auto &otherEntry:graph){
      if (otherEntry.first != contigName) { // Avoid self-comparison
          const ContigInfo &otherContigInfo = otherEntry.second;
          keyA = contigName;
          keyB = otherEntry.first;
          for (int markerA : contigInfo.markers) {
              for (int markerB : otherContigInfo.markers) {
                  if (markerA == markerB + 1 || markerB == markerA + 1) {
                      string oc_num = otherContigInfo.contigName;
                      //把contig_B的B挖出來
                      size_t pos = oc_num.find("contig_");
                      if (pos != string::npos) {
                        oc_num.replace(pos, 7, ""); // 將 "contig_" 替換為空字符串
                      }
                      //設計一塊新的merged contig
                      ContigInfo mergedContig;
                      mergedContig.contigName = contigName + "_" + oc_num;//merged_name = contig_A_B
                      mergedContig.markerCount =
                          contigInfo.markerCount + otherContigInfo.markerCount;
                      mergedContig.markers = contigInfo.markers;
                      mergedContig.markers.insert(mergedContig.markers.end(),
                                                  otherContigInfo.markers.begin(),
                                                  otherContigInfo.markers.end());
                      if(oc_num=="49"){
                        for(int x:contigInfo.markers) cout<<x<<" ";
                        cout<<endl;
                        for(int x:mergedContig.markers) cout<<x<<" ";
                        cout<<endl;
                        for(int x:otherContigInfo.markers) cout<<x<<" ";
                      }
                      mergedGraph[mergedContig.contigName] = mergedContig;
                      merged = true;
                      break;
                    } // if end
              }//for B end
              if(merged) break;
          }// for A end
      }//if end
      if(merged) break;
      }//for otherEntry end
      if (!merged){
          mergedGraph[contigName] = contigInfo;
          graph.erase(keyA);
      }else{//remove contig A and contig B
          graph.erase(keyA);
          graph.erase(keyB);
      }
  }//while end
  return mergedGraph;
}


void writeMergedToFile(const unordered_map<string, ContigInfo> &mergedGraph, string outFileName) {
  ofstream output(outFileName); // Open the output file

  if (output.is_open()) {
    int lineNumber = 1;
    for (const auto &entry : mergedGraph) {
      const ContigInfo &contigInfo = entry.second;
      for (int marker : contigInfo.markers) {
        // Write the formatted information to the file
        output << lineNumber << "\t" << marker << "\t" << contigInfo.contigName << "\t1\n";
        lineNumber++;
      }
    }
    output.close(); // Close the output file
  } else {
    cerr << "Unable to open output file." << endl;
  }
}

int main() {
  //deal with query all
  string inputFilename = "query.all"; 
  unordered_map<string, ContigInfo> contigGraph =
      buildContigAdjacencyGraph(inputFilename);
  unordered_map<string, ContigInfo> mergedGraph = mergeContigs(contigGraph);
  //deal with reference all
  string inputFilename2 = "reference.all";
  unordered_map<string, ContigInfo> contigGraph2 =
      buildContigAdjacencyGraph(inputFilename2);
  unordered_map<string, ContigInfo> mergedGraph2 = mergeContigs(contigGraph2);
  
  // Print the merged graph-Query
  int total_markers = 0;
  for (const auto &entry : mergedGraph) {
    const string &contigNumber = entry.first;
    const ContigInfo &contigInfo = entry.second;
    cout << "Contig: " << contigNumber
         << ", Marker Count: " << contigInfo.markerCount << endl;
    total_markers += contigInfo.markerCount;
  }
  cout<<"Total Query Marker Count: "<<total_markers<<endl<<endl;
  writeMergedToFile(mergedGraph, "query_merged.txt");
  // Print the merged graph-Reference
  int total_markers2 = 0;
  for (const auto &entry : mergedGraph) {
    const string &contigNumber2 = entry.first;
    const ContigInfo &contigInfo2 = entry.second;
    cout << "Contig: " << contigNumber2
         << ", Marker Count: " << contigInfo2.markerCount << endl;
    total_markers2 += contigInfo2.markerCount;
  }
  cout<<"Total Reference Marker Count: "<<total_markers2<<endl;
  writeMergedToFile(mergedGraph2, "reference_merged.txt");

  return 0;
}
