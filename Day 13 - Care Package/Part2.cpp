#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

int playerScore = 0;
vector <vector <char>*> surface;
const char tile[] = {' ', '|', ':', '-', 'O'};

void setRows(unsigned int maxRow) {
    while(surface.size() <= maxRow) {
        surface.push_back(new vector <char>());
        for (unsigned int i=0; i<surface.at(0)->size(); ++i) {
            surface.back()->push_back('?');
        }
    }
}

void setColumns(unsigned int maxColumn) {
    while(surface.at(0)->size() <= maxColumn) {
        for (unsigned int i=0; i<surface.size(); ++i) {
            surface.at(i)->push_back('?');
        }
    }
}

void printSurface() {
    cout << endl << "*** SCORE: " << playerScore << " ***";
    for (unsigned int i=0; i<surface.size(); ++i) {
        cout << endl;
        for (unsigned int j=0; j<surface.at(0)->size(); ++j) {
            cout << surface.at(i)->at(j);
        }
    }
    cout << endl;
}

void updateSurface(int X, int Y, unsigned int tileID) {
    if (X == -1 && Y == 0) { // Update score
        playerScore = tileID;
    } else { // Update tiles
        if(static_cast<unsigned int>(Y) >= surface.size()) {
            setRows(Y);
        }
        if(static_cast<unsigned int>(X) >= surface.at(0)->size()) {
            setColumns(X);
        }
        surface.at(Y)->at(X) = tile[tileID];
    }
}

int getTilePosX(unsigned int tileID) {
    for (unsigned int i=0; i<surface.size(); ++i) {
        for (unsigned int j=0; j<surface.at(0)->size(); ++j) {
            if(surface.at(i)->at(j) == tile[tileID]) {
                return j;
            }
        }
    }
    return -1;
}

int nextMove() { // Virtual joystick bot
    if(getTilePosX(3) < getTilePosX(4)) {
        return 1;
    } else if(getTilePosX(3) > getTilePosX(4)) {
        return -1;
    }
    return 0;
}


void memoryPadding(vector <long>* memory, unsigned int size) {
    while(memory->size() <= size) {
        memory->push_back(0);
    }
}

long* accessMemory(vector <long>* memory, int pos) {
    long* value = nullptr;
    if (pos >= 0) {
        memoryPadding(memory, pos);
        value = &(memory->at(pos));
    }
    return value;
}

long* getParamValue(vector <long>* memory, long pos, int paramMode, long relativeBase = 0) {
    long* value = nullptr;
    if(paramMode == 1) {
        value = new long(pos); // Immediate mode
    } else if(paramMode == 2) {
        value = accessMemory(memory, pos + relativeBase); // Relative mode
    } else {
        value = accessMemory(memory, pos); // Position mode
    }
    return value;
}

void execOpcode(vector <long>* intcode) {
    unsigned int i=0; // Intcode position
    long relativeBase = 0;

    vector<long> outputBuffer;
    outputBuffer.clear();

    while( i<intcode->size() ) {

        if(outputBuffer.size() >= 3) {
            updateSurface(outputBuffer.at(0), outputBuffer.at(1), outputBuffer.at(2));
            outputBuffer.clear();
        }

        int opcode = intcode->at(i)%100;
        long* param1 = nullptr;
        long* param2 = nullptr;
        long* param3 = nullptr;

        int paramMode1 = (intcode->at(i)/100)%10;
        int paramMode2 = (intcode->at(i)/1000)%10;
        int paramMode3 = (intcode->at(i)/10000)%10;

        param1 = getParamValue(intcode, intcode->at(i+1), paramMode1, relativeBase); // Single parameter opcodes

        if((opcode <= 2 || opcode >= 5) && opcode < 9) { // Two parameter opcodes
            param2 = getParamValue(intcode, intcode->at(i+2), paramMode2, relativeBase);
        }

        if((opcode <= 2 || opcode >= 7) && opcode < 9) { // Three parameter opcodes
            param3 = getParamValue(intcode, intcode->at(i+3), paramMode3, relativeBase);
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
                printSurface();
                long tmpInput = nextMove();
                cout << "> " << tmpInput << endl;
                //cin >> tmpInput;
                *param1 = tmpInput;
                i+=2;
                break;
            }

            case 4: { // Output
                outputBuffer.push_back(*param1);
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

            case 9: { // Relative base offset
                relativeBase += *param1;
                i+=2;
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
        vector <long> intcode;
        string line="", current="";
        while(getline(file, current, ',')) {
            intcode.push_back(stol(current));
        }
        intcode.at(0) = 2; // Set number of quarters that have been inserted to 2
        execOpcode(&intcode);
        printSurface();
        cout << "*** GAME OVER ***" << endl;
    } else {
        cerr << "Cannot open given file!" << endl;
    }
    return 0;
}