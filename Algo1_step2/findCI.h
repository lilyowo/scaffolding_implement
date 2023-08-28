// findCI.h
/*

*/
#include "dataInput.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

#ifndef FINDCI_H
#define FINDCI_H
struct ConservedInterval {
  int ida1, idb1;
  vector<string> belongContig;
};
vector<ConservedInterval> findConservedInterval(vector<marker> G1,
                                                vector<marker> G2);

#endif // FINDCI_H