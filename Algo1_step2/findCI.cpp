//findCI.cpp
/*
conserved interval要記錄：開頭 結尾 屬於哪些contigs
for(a=0 a++)遍歷整個genome所有markers，不管contigs，作為開頭
在裡面for(b=a+1 b++)遍歷整個genome所有markers，不管contigs，作為開頭
如果此開頭此結尾在    G1中的順序也有在G2中
且                 內容(無視方向性)的集合相同
則建成一個conserved interval
*/
#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // 用于绝对值函数 abs
#include "dataInput.h"
#include "findCI.h"
using namespace std;

int checkOrder(int ida, int idb, vector<marker>G1, vector<marker>G2);
int checkSetContent(int idx_a1, int idx_b1, int idx_a2, int idx_b2, vector<marker>G1, vector<marker>G2);
//int ida1, idb1, ida2, idb2;
vector<ConservedInterval> 
findConservedInterval(vector<marker>G1, vector<marker>G2){
  vector<ConservedInterval> arr;
  for (size_t i = 0; i < G1.size(); ++i){
    for(size_t j=i+1; j<G1.size();++j){
      if(checkOrder(i, j, G1, G2)){
        ConservedInterval newCI;
        newCI.ida1 = i;
        newCI.idb1 = j;
        if(G1[i].contigName==G1[j].contigName)
        newCI.belongContig.push_back(G1[i].contigName);
        arr.push_back(newCI);
      } 
    }//for G1 b end
  }//for G1 a end
  return arr;
}

int checkOrder(int ida1, int idb1, vector<marker>G1, vector<marker>ref){
  int ida2, idb2;
  for(size_t i=0;i<ref.size();i++){
  if(ref[i].value==G1[ida1].value){
    ida2=i;
    for(size_t j=i+1; j<ref.size(); j++){
    if(ref[j].value==G1[idb1].value){
      idb2 = j;
      return checkSetContent(ida1, idb1, ida2, idb2, G1, ref);
    }//if b1=b2 end
    }//for b2 end
  }//if a1=a2 end
  }//for a2 end
  return 0;
}
int checkSetContent(int idx_a1, int idx_b1, int idx_a2, int idx_b2, vector<marker>G1, vector<marker>G2){
  // 检查输入的索引范围是否有效
  if (idx_a1 < 0 || idx_b1 >= G1.size() || idx_a2 < 0 || idx_b2 >= G2.size()) {
    cerr << "Invalid index range." << endl;
    return false;
  }
  // 提取 G1 和 G2 的子集合（使用绝对值）
  vector<int> subsetG1, subsetG2;
  for (int i = idx_a1; i <= idx_b1; ++i) {
    subsetG1.push_back(abs(G1[i].value));
  }
  for (int i = idx_a2; i <= idx_b2; ++i) {
    subsetG2.push_back(abs(G2[i].value));
  }

  // 对子集合排序以便比较
  sort(subsetG1.begin(), subsetG1.end());
  sort(subsetG2.begin(), subsetG2.end());

  // 比较子集合是否相等
  return subsetG1 == subsetG2;
}