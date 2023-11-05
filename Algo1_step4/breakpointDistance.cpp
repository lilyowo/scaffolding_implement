#include "breakpointDistance.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Function to calculate breakpoint distance between two genomes
int BreakpointDistance(const vector<int>& genome1, const vector<int>& genome2) {
    int numBreakpoints = 0;

    // Iterate through the genomes
    for (int i = 0; i < genome1.size() - 1; i++) {
        // Check if the current adjacency is conserved
        if (genome1[i] + 1 != genome1[i + 1] && genome2[i] + 1 != genome2[i + 1]) {
            numBreakpoints++;
        }
    }

    // Check the last adjacency for circular genomes
    if (genome1.back() + 1 != genome1.front() && genome2.back() + 1 != genome2.front()) {
        numBreakpoints++;
    }

    return numBreakpoints;
}
