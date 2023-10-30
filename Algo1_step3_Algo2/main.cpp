//如果a不屬於任一個most-inner irreducible conserved
// interval，那麼我們會將它標記為是一個free marker。 for (a = 0; a < m; a++) for
// (b = a+1; b<m; b++)如果a所在的contig Ci以及b所在的contig
// Cj是不同的話，我們才會繼續進行下面的步驟。

// A代表Ci上a以後的markers，B代表Cj上b以前的markers。A={a+1,…} B={…, b-1}
// IF 存在其他的contigs D1, D2, …, Dk，且每一個 D ∈ {C1, …, Cn} \ {Ci, Cj}，使得
// A∪D1∪…∪Dk∪B = {a+1, …, b-1}且每一個在{a+1, …, b-1}中的元素都是一個free
// marker。

// Then 將這些contigs相接，且把在{a, a+1, …, b}之中的每一個free
// marker標記為non-free marker。

#include "dataInput.h"
#include <algorithm> //for find
#include <iostream>
#include <set>
#include <vector>

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

bool IsFreeMarker(vector<int> markers, vector<marker> Fmarkers) {
  bool isMatching = true;
  for (int a : markers) {
    bool found = false;
    for (int i = 0; i < Fmarkers.size(); i++) {
      if (a == Fmarkers[i].value) {
        found = true;
        break;
      }
    }
    if (!found) {
      isMatching = false; // 找不到匹配的情况
      break;
    }
  }

  return isMatching;
}
vector<marker> UpdateFreeMarkers(vector<string> contigs,
                                 vector<marker> Fmarkers) {
  vector<marker> updatedFmarkers;

  for (const marker &m : Fmarkers) {
    // 检查 Fmarkers[i].contigName 是否出现在 contigs 中
    if (find(contigs.begin(), contigs.end(), m.contigName) == contigs.end()) {
      // 如果没有出现，将该元素添加到更新后的 Fmarkers 中
      updatedFmarkers.push_back(m);
    }
  }

  return updatedFmarkers;
}

int main() {
  // 讀取 input 檔案並建立 contig adjacency graph
  unordered_map<string, ContigInfo> genome1 =
      buildContigGraph("ICI_Merged.txt");
  writeToFile(genome1, "Original.txt");
  cout << "Contig Graph builded" << endl;

  vector<marker> G1markerArr = markerArr("ICI_Merged.txt");
  vector<marker> FreemarkerArr = markerArr("FreeMarkers.txt");
  cout << "G1 FreeMarkers array maked" << endl;

  for (size_t a = 0; a < G1markerArr.size(); a++) {
    for (size_t b = a + 1; b < G1markerArr.size(); b++) {
      if (G1markerArr[a].contigName != G1markerArr[b].contigName) {
        string AcontigName = G1markerArr[a].contigName;
        string BcontigName = G1markerArr[b].contigName;
        //抓出a以後同個contigName的markers
        vector<int> Amarkers = genome1[AcontigName].markers;
        int idx = 0;
        while (idx < Amarkers.size() && Amarkers[idx] != G1markerArr[a].value) {
          idx++;
        }
        if (idx < Amarkers.size()) {
          Amarkers.erase(Amarkers.begin(), Amarkers.begin() + idx);
        }
        //抓出b以前同個contigName的markers
        vector<int> Bmarkers = genome1[BcontigName].markers;
        int idxb = 0;
        while (idxb < Bmarkers.size() &&
               Bmarkers[idxb] != G1markerArr[b].value) {
          idxb++;
        }
        if (idxb > 0) {
          vector<int> BeforeB(Bmarkers.begin(), Bmarkers.begin() + idxb);
          Bmarkers = BeforeB;
        }
        //確認a b的markers都是FreeMarkers
        if (!IsFreeMarker(Amarkers, FreemarkerArr) ||
            !IsFreeMarker(Bmarkers, FreemarkerArr))
          continue;
        // cout << "Merge AB A:" << AcontigName << " B:" << BcontigName << endl;
        // // 合併A B 進genome1(合入A刪除B)
        // genome1[AcontigName].markers.insert(
        //     genome1[AcontigName].markers.end(),
        //     genome1[BcontigName].markers.begin(),
        //     genome1[BcontigName].markers.end());
        // genome1[AcontigName].markerCount += genome1[BcontigName].markerCount;
        // genome1.erase(BcontigName);

        // // 從FreeMarkers剔除Acontig Bcontig
        // vector<string>ABcontigs = {AcontigName, BcontigName};
        // FreemarkerArr = UpdateFreeMarkers(ABcontigs, FreemarkerArr);
        //抓出a以後b以前 與a b contigName不同的D contig 先記錄
        vector<string> Dcontigs;
        for (const auto &entry : genome1) {
          if (entry.first != AcontigName && entry.first != BcontigName) {
            vector<int> Dmarkers = entry.second.markers;
            //確認D contig都是freeMarkers，不是的話下面一位
            if (IsFreeMarker(Dmarkers, FreemarkerArr)) {
              //如果D的數量大於1
              if (!Dmarkers.empty()) {
                Dcontigs.push_back(entry.first);
              }
            }
          }
        } // for genome1 end
        if(Dcontigs.size()>0){
          //合併A B 進genome1(合入A刪除B)
          genome1[AcontigName].markers.insert(
              genome1[AcontigName].markers.end(),
              genome1[BcontigName].markers.begin(),
              genome1[BcontigName].markers.end());
          genome1[AcontigName].markerCount += genome1[BcontigName].markerCount;
          genome1.erase(BcontigName);
          //從FreeMarkers剔除Acontig Bcontig
          vector<string>ABcontigs = {AcontigName, BcontigName};
          FreemarkerArr = UpdateFreeMarkers(ABcontigs, FreemarkerArr);
        }
        for (size_t idD = 0; idD < Dcontigs.size(); idD++) {
          string DcontigName = Dcontigs[idD];
          cout << "Merge AD A:" << AcontigName << " D:" << DcontigName << endl;
          //合併這些contigs D合併進A 刪除D
          genome1[AcontigName].markers.insert(
              genome1[AcontigName].markers.end(),
              genome1[DcontigName].markers.begin(),
              genome1[DcontigName].markers.end());
          genome1[AcontigName].markerCount += genome1[DcontigName].markerCount;
          genome1.erase(DcontigName);
          //從FreeMarkers剔除 Dcontig
          FreemarkerArr = UpdateFreeMarkers(Dcontigs, FreemarkerArr);
        }
      } // If contig A!=B end
    }   // for b end
  }     // for a end
  cout << "Merge End" << endl;
  writeToFile(genome1, "Algo2_Merged.txt");
  cout << "owo complete" << endl;

  return 0;
}
