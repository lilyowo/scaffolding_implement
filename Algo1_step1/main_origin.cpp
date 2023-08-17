// #include <fstream>
// #include <iostream>
// #include <sstream>
// #include <string>
// #include <unordered_map>
// #include <vector>
// //原本要用for迴圈寫，結果因為erase不可以在迴圈中而砍掉重練merged
// using namespace std;
// // Data structure to store contig adjacency information
// struct ContigInfo {
//   vector<int> markers; //存該contig有哪些marker
//   string contigName;   //存contig的名稱或編號
//   int markerCount;     //紀錄該contig共有幾個marker
// };

// // Function to read input data and build contig adjacency graph
// //讀入data.txt return 儲存好contig_x跟他的數量的map
// unordered_map<string, ContigInfo>
// buildContigAdjacencyGraph(const string &filename) {
//   ifstream input(filename);
//   unordered_map<string, ContigInfo> contigAdjacencyGraph;

//   if (input.is_open()) {
//     string line;
//     while (getline(input, line)) {
//       string index, marker, contigName;
//       int markerCount;
//       istringstream lineStream(line); //需要sstream 可拆解字串
//       lineStream >> index >> marker >> contigName >> markerCount;
//       if (contigAdjacencyGraph.find(contigName) ==
//           contigAdjacencyGraph.end()) { //若該contig_x不存在 就加入之
//         ContigInfo contigInfo;
//         contigInfo.contigName = contigName;
//         contigInfo.markerCount = 1;
//         contigInfo.markers.push_back(stoi(marker));
//         contigAdjacencyGraph[contigName] = contigInfo;
//       } else {
//         contigAdjacencyGraph[contigName].markers.push_back(stoi(marker));
//         contigAdjacencyGraph[contigName].markerCount +=
//             markerCount; //若contig_x已存在 則+1 (因為markerCount那格都會是1)
//       }
//     }
//     input.close();
//   } else { // file open有error
//     cerr << "Unable to open file: " << filename << endl;
//   }

//   return contigAdjacencyGraph;
// }

// // Function to merge contigs based on adjacency
// unordered_map<string, ContigInfo>
// mergeContigs(unordered_map<string, ContigInfo> &graphIn) {
//   unordered_map<string, ContigInfo> graph = graphIn;
//   unordered_map<string, ContigInfo> mergedGraph;

//   for (const auto &entry : graph) {
//     const string &contigName = entry.first;
//     const ContigInfo &contigInfo = entry.second;
//     bool merged = false; // track if contig was merged
//     string remove_keyB, remove_keyA;
//     for (const auto &otherEntry : graph) {
//       if (otherEntry.first != contigName) { // Avoid self-comparison
//         const ContigInfo &otherContigInfo = otherEntry.second;
        
//         for (int markerA : contigInfo.markers) {
//           for (int markerB : otherContigInfo.markers) {
//             if (markerA == markerB + 1 || markerB == markerA + 1) {
//               string oc_num = remove_keyB = otherContigInfo.contigName;
//               remove_keyA = contigName;
//               //把contig_B的B挖出來 B is int
//               size_t pos = oc_num.find("contig_");
//               if (pos != string::npos) {
//                 oc_num.replace(pos, 7, ""); // 將 "contig_" 替換為空字符串
//               }
//               //設計一塊新的merged contig
//               ContigInfo mergedContig;
//               mergedContig.contigName = contigName + "_" + oc_num;//merged_name = contig_A_B, AB both int
//               mergedContig.markerCount =
//                   contigInfo.markerCount + otherContigInfo.markerCount;
//               mergedContig.markers.insert(mergedContig.markers.end(),
//                                           otherContigInfo.markers.begin(),
//                                           otherContigInfo.markers.end());
//               mergedGraph[mergedContig.contigName] = mergedContig;
//               merged = true;
//               break;
//             } // if end
//           }   // for B end
//           if (merged){
//             graph.erase(remove_keyB);//消滅被merged的contig B，原本沒消結果他氾濫!!
//             // graph.erase(remove_keyA);
//             break;
//           }
//         } // for A end
//         // if(merged)graph.erase(remove_keyA);
//       }
//       if (merged)
//         break;
//     }//for other graph end

//     // if not merged, add contig to merged graph
//     if (!merged)
//       mergedGraph[contigName] = contigInfo;
//   }//for graph end

//   return mergedGraph;
// }

// void writeMergedContigsToFile(const unordered_map<string, ContigInfo> &mergedGraph) {
//   ofstream output("merged_contigs.txt"); // Open the output file

//   if (output.is_open()) {
//     int lineNumber = 1;
//     for (const auto &entry : mergedGraph) {
//       const ContigInfo &contigInfo = entry.second;
//       for (int marker : contigInfo.markers) {
//         // Write the formatted information to the file
//         output << lineNumber << "\t" << marker << "\t" << contigInfo.contigName << "\t1\n";
//         lineNumber++;
//       }
//     }
//     output.close(); // Close the output file
//   } else {
//     cerr << "Unable to open output file." << endl;
//   }
// }

// int main() {
//   string inputFilename = "query.all"; // Replace with your input file's name
//   unordered_map<string, ContigInfo> contigGraph =
//       buildContigAdjacencyGraph(inputFilename);
//   unordered_map<string, ContigInfo> mergedGraph = mergeContigs(contigGraph);
//   // unordered_map<string, ContigInfo> mergedGraph2 = mergeContigs(mergedGraph);
//   // unordered_map<string, ContigInfo> mergedGraph3 = mergeContigs(mergedGraph2);
//   // Print the merged graph
//   int total_markers = 0;
//   for (const auto &entry : mergedGraph) {
//     const string &contigNumber = entry.first;
//     const ContigInfo &contigInfo = entry.second;
//     cout << "Contig: " << contigNumber
//          << ", Marker Count: " << contigInfo.markerCount << endl;
//     total_markers += contigInfo.markerCount;
//   }
//   cout<<"Total Marker Count: "<<total_markers<<endl;



//   int total = 0;
//   for (const auto &entry : contigGraph) {
//     const string &contigNumber = entry.first;
//     const ContigInfo &contigInfo = entry.second;
//     cout << "Contig: " << contigNumber
//          << ", Marker Count: " << contigInfo.markerCount << endl;
//     total += contigInfo.markerCount;
//   }
//   cout<<"Total Count: "<<total<<endl;

//   return 0;
// }
