#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

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
        intcode.at(1) = 12;
        intcode.at(2) = 2;
        for(unsigned int i=0; i<intcode.size(); i+=4) {
            switch (intcode.at(i))
            {
                case 1:
                    intcode.at(intcode.at(i+3)) = intcode.at(intcode.at(i+1)) + intcode.at(intcode.at(i+2));
                    break;
                case 2:
                    intcode.at(intcode.at(i+3)) = intcode.at(intcode.at(i+1)) * intcode.at(intcode.at(i+2));
                    break;
                case 99:
                    cout << "Value at position 0 is [" << intcode.at(0) << "]" << endl;
                    return 0;
                default:
                    throw runtime_error("ERROR: Encountered invalid opcode!" );
            }
        }
        cerr << "ERROR: Opcode 99 to halt is missing!" << endl;
    } else {
        cerr << "Cannot open given file!" << endl;
    }
    return 0;
}