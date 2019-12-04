#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

void execOpcode(vector <int>* intcode) {
    for(unsigned int i=0; i<intcode->size(); i+=4) {
        switch (intcode->at(i))
        {
            case 1:
                intcode->at(intcode->at(i+3)) = intcode->at(intcode->at(i+1)) + intcode->at(intcode->at(i+2));
                break;
            case 2:
                intcode->at(intcode->at(i+3)) = intcode->at(intcode->at(i+1)) * intcode->at(intcode->at(i+2));
                break;
            case 99:
                return;
            default:
                throw runtime_error("ERROR: Encountered invalid opcode!" );
        }
    }
    cerr << "ERROR: Opcode 99 to halt is missing!" << endl;
}

int main(int argc, char *argv[]) {
    string file_path = "input.txt";
    if(argc > 1) {
        file_path = argv[1];
    }
    ifstream file(file_path);
    if (file) {
        stringstream linestream;
        vector <int> intcode;
        string line="", current="";
        while(getline(file, current, ',')) {
            intcode.push_back(stoi(current));
        }
        vector <int> defaultIntcode = intcode;
        for(unsigned int i=0; i<99; ++i) {
            for(unsigned int j=0; j<99; ++j) {
                intcode = defaultIntcode;
                intcode.at(1) = i;
                intcode.at(2) = j;
                execOpcode(&intcode);
                if(intcode.at(0) == 19690720) {
                    cout << "noun is [" << i << "]" << endl
                         << "verb is [" << i << "]" << endl
                         << "Result is [" << 100*i+j << "]" << endl;
                    return 0;
                }
            }
        }
        cerr << "ERROR: Couldn't determine the pair you were looking for!" << endl;
    } else {
        cerr << "Cannot open given file!" << endl;
    }
    return 0;
}

