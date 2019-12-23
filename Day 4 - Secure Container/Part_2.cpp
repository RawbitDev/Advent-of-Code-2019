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
        bool neverDecrease = true;
        bool adjacentDigits = false;

        for(unsigned int j=0; j<current.size()-1; ++j) {
            if( current.at(j) > current.at(j+1) ) {
                neverDecrease = false;
                break;
            }
        }

        if(neverDecrease) {
            char last = current.at(0);
            unsigned int occurrence = 1;

            for (unsigned int j=1; j<current.size(); ++j) {
                if (current.at(j) == last) {
                    ++occurrence;
                } else {
                    last = current.at(j);
                    if (occurrence == 2) {
                        adjacentDigits = true;
                        break;
                    } else {
                        occurrence = 1;
                    }
                }
            }
            if (occurrence == 2) {
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