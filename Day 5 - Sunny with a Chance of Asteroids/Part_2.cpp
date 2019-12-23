#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

void execOpcode(vector <int>* intcode) {
    unsigned int i=0;
    while( i<intcode->size() ) {

        int opcode = intcode->at(i)%100;
        int* param1 = nullptr;
        int* param2 = nullptr;
        int* param3 = nullptr;

        int paramMode1 = (intcode->at(i)/100)%10;
        int paramMode2 = (intcode->at(i)/1000)%100;
        int paramMode3 = (intcode->at(i)/10000)%1000;

        if(paramMode1) { // Single parameter opcodes
            param1 = new int(intcode->at(i+1)); // Immediate mode
        } else {
            param1 = &(intcode->at(intcode->at(i+1))); // Position mode
        }

        if(opcode <= 2 || opcode >= 5) { // Two parameter opcodes
            if(paramMode2) {
                param2 = new int(intcode->at(i+2));
            } else {
                param2 = &(intcode->at(intcode->at(i+2)));
            }
        }

        if(opcode <= 2 || opcode >= 7) { // Three parameter opcodes
            if(paramMode3) {
                param3 = new int(intcode->at(i+3)); // Immediate mode
            } else {
                param3 = &(intcode->at(intcode->at(i+3))); // Position mode
            }
        }

        switch (opcode)
        {
            case 1: { // Addition
                *param3 = *param1 + *param2;
                i+=4;
                break;
            }

            case 2: { // Multiplication
                *param3 = *param1 * (*param2);
                i+=4;
                break;
            }

            case 3: { // Input
                int tmpInput = 0;
                cout << "> ";
                cin >> tmpInput;
                *param1 = tmpInput;
                i+=2;
                break;
            }

            case 4: { // Output
                cout << *param1 << endl;
                i+=2;
                break;
            }

            case 5: { // Jump-if-true
                if( *param1 ) {
                    i = *param2;
                } else {
                    i+=3;
                }
                break;
            }

            case 6: { // Jump-if-false
                if( !(*param1) ) {
                    i = *param2;
                } else {
                    i+=3;
                }
                break;
            }

            case 7: { // Less than
                if(*param1 < *param2) {
                    *param3 = 1;
                } else {
                    *param3 = 0;
                }
                i+=4;
                break;
            }

            case 8: { // Equals
                if(*param1 == *param2) {
                    *param3 = 1;
                } else {
                    *param3 = 0;
                }
                i+=4;
                break;
            }

            case 99: { // Halt
                return;
            }

            default: {
                throw runtime_error("ERROR: Encountered invalid opcode!" );
            }
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
        execOpcode(&intcode);
    } else {
        cerr << "Cannot open given file!" << endl;
    }
    return 0;
}

