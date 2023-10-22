#include "ConnectICI.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
/**
首先 ContigInfo 跟 marker是如何運作的：
  marker是拿來 把所有marker無視contig排出來 用於找CI，因此與本步驟關係不大
  ContigInfo是存取一整個genome的格式 這是我們要用到的

再來 要怎麼透過ICI去挑出contig：
  ICI存取的這個ICI有沒有跨contig
  for(the belongContig vector) if(BC[i]!=BC[i+1])跨contig了
  就從G1找出contig name與之相等的兩個contig
  記住他們在G1的ID
  把他們的marker灌進去一個ContigInfo中，改名，改數量
  出G1迴圈後把那兩個contig殺掉，然後把contigInfo丟給ans

  所有ICI的merged contig都灌進去後，把剩下無法merge的contig也都灌進去ans

最後 怎麼把接起來的contig存起來
*/
unordered_map<string, ContigInfo>
ConnectICI(vector<ConservedInterval> ICI,
           unordered_map<string, ContigInfo> G1) {
  unordered_map<string, ContigInfo> ans;

  int cnt = 0;
  while (true) {
    if (G1.empty())
      break;
    if (cnt > 1450)
      break;
    cnt++;
    const string &contigName = G1.begin()->first;
    const ContigInfo &contigInfo = G1.begin()->second;
    bool merged = false;
    string keyA, keyB;
    keyA = contigName;
    for (size_t i = 0; i < ICI.size(); i++) {
      if(ICI[i].belongContig[i].size()<=1) continue;
      if (ICI[i].belongContig[0] == ICI[i].belongContig[1])
        continue;
      // cout<<"ContigA: "<<ICI[i].belongContig[0]<<"\nContigB: "<<ICI[i].belongContig[1]<<endl;
      if (ICI[i].belongContig[0] == contigName) {
        keyA = contigName;
        for (const auto contig2 : G1) {
          if (contig2.first == ICI[i].belongContig[1]) {
            keyB = contig2.first;
            merged = true;
            ContigInfo otherContigInfo = contig2.second;
            ContigInfo mergedContig;
            //把marker兩個各自丟進去
            mergedContig.markers = contigInfo.markers;
            mergedContig.markers.insert(mergedContig.markers.end(),
                                        otherContigInfo.markers.begin(),
                                        otherContigInfo.markers.end());
            //相加marker總數量
            mergedContig.markerCount =
                contigInfo.markerCount + otherContigInfo.markerCount;
            //修改contig name
            string oc_num = otherContigInfo.contigName;
            //把contig_B的B挖出來
            size_t pos = oc_num.find("contig_");
            if (pos != string::npos) {
              oc_num.replace(pos, 7, ""); // 將 "contig_" 替換為空字符串
            }
            mergedContig.contigName = contigName + "_" + oc_num;
            //把mergedContig丟進ans
            ans[mergedContig.contigName] = mergedContig;
            break;
          }
        } // for contig 2 end

      } // if it can merge end
      if (merged) break;
    } // for ICI end
    if (merged) {
      cout<<"merge contig:"<<keyA<<" "<<keyB<<endl;
      G1.erase(keyA);
      G1.erase(keyB);
    } else {
      
      ans[contigName] = contigInfo;
      G1.erase(keyA);
    }
  } // while end

  return ans;
}