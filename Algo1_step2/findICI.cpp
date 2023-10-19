// findICI.cpp
/*
irreducible CI要記錄：

從Conserved interval的id區間來判斷他們有沒有被重疊
ax<bx
CI id區間分類:
[a1=a2<b1<b2] [a1=a2<b2<b1] [a1=a2<b2=b1]
[a1<a2<b1<b2] [a1<a2<b2<b1] [a1<b1<a2<b2] [a1<a2<b1=b2] [a1<b1=a2<b2]
[a2<a1<b1<b2] [a2<b2<a1<b1] [a2<a1<b2<b1] [a2<a1<b1=b2] [a2<b2=a1<b1]

1要拿掉 2要拿掉   重複二取一
都丟掉  Nested   Disjoint 2要拿掉 Chained
Nested Disjoint 都丟掉    1要拿掉 Chained

論 交錯的CI：
conserved interval如果呈現交錯 是不是一定會有一個更大的conserved
interval存在包含他們所以他們應該被消滅？ conserved interval的定義是
開頭跟結尾順序相同+內容物的集合一樣 有可能ref：a1 b1 xxxxx a2 b2這樣a1
b2之間的就不會內容一樣了 所以交錯的CI

論 被包住的CI：
頭相黏 或 尾相黏 ？不符合Nested原則
頭尾不相黏？符合Nested原則


irreducible有三種型式 Disjoint、Nested、Chained
Disjoint：a1<b1 <or> a2<b2
Nested：a1< a2 <b2 <b1
Chained：a1< b1=a2 <b2


我們先抓ICI就好 只要該CI可以由其他CI組成 我們就消滅它
*/
#include "findICI.h"
#include "dataInput.h"
#include "findCI.h"
#include <iostream>
#include <list>
#include <string>
#include <vector>
using namespace std;

bool isNotIrriducible(int ID, int s, int e, vector<ConservedInterval> CI);

vector<ConservedInterval> findICI(vector<ConservedInterval> CI) {
  list<int> IDToRemove;
  for (size_t i = 0; i < CI.size(); i++) {
    if (isNotIrriducible(i, CI[i].ida1, CI[i].idb1, CI))
      IDToRemove.push_back(i);
  }
  vector<ConservedInterval> ICI = CI;
  // cout<<"removeIDs:"<<endl;
  for (int index : IDToRemove) {
    // cout<<index<<" ";
    ICI.erase(ICI.begin() + index);
  }
  return ICI;
}

bool isNotIrriducible(int ID, int s, int e, vector<ConservedInterval> CI) {
  //他們都是ID照理來說不會有正負問題 :)
  if (s > e) {
    int temp = e;
    e = s;
    s = temp;
  }
  bool ans = false;

  for (size_t i = 0; i < CI.size(); i++) {
    if (i == ID)
      continue;
    int sID2, eID2;
    sID2 = CI[i].ida1;
    eID2 = CI[i].idb1;
    if (sID2 > eID2) {
      int temp = eID2;
      eID2 = sID2;
      sID2 = temp;
    }
    // if(s<40 && s>30 && sID2<40 && sID2>30) cout<<"s: "<<s<<" e:
    // "<<e<<"\nsID2: "<<sID2<<" eID2: "<<eID2<<"\n";

    if (sID2 == s && eID2 == e)
      return true;
    else if (sID2 == s && eID2 < e) {
      // if(s<40 && s>30 && sID2<40 && sID2>30)cout<<s<<" "<<e<<" turn to
      // "<<eID2<<" "<<e<<"\n";
      return isNotIrriducible(ID, eID2, e, CI);
    } else if (s < sID2 && eID2 == e) {
      // if(s<40 && s>30 && sID2<40 && sID2>30)cout<<s<<" "<<e<<" turn to
      // "<<s<<" "<<sID2<<"\n";
      return isNotIrriducible(ID, s, sID2, CI);
    }
  }
  // if(s<40 && s>30)cout<<"Fail to replace QQ"<<endl;
  return false;
}

// 1            10
// 1 - 2 2 -   9
// 1   - 5 5 - 10