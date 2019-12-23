#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
    unsigned int rangeRuleMin = 152085;
    unsigned int rangeRuleMax = 670283;
    if(argc > 2) {
        try {
            rangeRuleMin = stoi(argv[1]);
            rangeRuleMax = stoi(argv[2]);
        } catch (...) {
            cerr << "Cannot interpret given range! Using default instead..." << endl;
        }
    }

    unsigned int possiblePasswordCount = 0;
    for(unsigned int i=rangeRuleMin; i<=rangeRuleMax; ++i) {
        cerr << "\rChecking password key facts for [" << i << "]     ";
        string current = to_string(i);
        bool adjacentDigits = false;
        bool neverDecrease = true;
        for(unsigned int j=0; j<current.size()-1; ++j) {
            if( (current.at(j) > current.at(j+1)) ) {
                neverDecrease = false;
                break;
            }
            if( (current.at(j) == current.at(j+1)) ) {
                adjacentDigits = true;
            }
        }
        if(adjacentDigits && neverDecrease) {
            ++possiblePasswordCount;
            cerr << "\rPossible password found: [" << i << "]          " << endl;
        }
    }

    cout << "\r" << possiblePasswordCount << " passwords found that meet the given criteria." << endl;

    return 0;
}