#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

#define minPhaseSetting 5
#define maxPhaseSetting 9

int execOpcode(vector <int>* intcode, vector <int>* input = new vector <int>, vector <int>* output = new vector <int>, bool silentMode = false, unsigned int pc = 0, unsigned int ic = 0) {
    unsigned int i = pc;
    unsigned int inputCounter = ic;

    while( i<intcode->size() ) {

        int opcode = intcode->at(i)%100;
        int* param1 = nullptr;
        int* param2 = nullptr;
        int* param3 = nullptr;

        int paramMode1 = (intcode->at(i)/100)%10;
        int paramMode2 = (intcode->at(i)/1000)%100;
        int paramMode3 = (intcode->at(i)/10000)%1000;

        if(opcode != 99) { // Skip on halt opcode
            if (paramMode1) { // Single parameter opcodes
                param1 = new int(intcode->at(i + 1)); // Immediate mode
            } else {
                param1 = &(intcode->at(intcode->at(i + 1))); // Position mode
            }

            if (opcode <= 2 || opcode >= 5) { // Two parameter opcodes
                if (paramMode2) {
                    param2 = new int(intcode->at(i + 2));
                } else {
                    param2 = &(intcode->at(intcode->at(i + 2)));
                }
            }

            if (opcode <= 2 || opcode >= 7) { // Three parameter opcodes
                if (paramMode3) {
                    param3 = new int(intcode->at(i + 3));
                } else {
                    param3 = &(intcode->at(intcode->at(i + 3)));
                }
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
                if(inputCounter < input->size()) { // If there's still any given input use it
                    tmpInput = input->at(inputCounter);
                    ++inputCounter;
                } else {
                    return i; // Return program counter
                }
                *param1 = tmpInput;
                i+=2;
                break;
            }

            case 4: { // Output
                output->push_back(*param1); // Save the output
                if(!silentMode) cout << *param1 << endl;
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
                return 0;
            }

            default: {
                throw runtime_error("ERROR: Encountered invalid opcode!" );
            }
        }
    }
    cerr << "ERROR: Opcode 99 to halt is missing!" << endl;
    return -1;
}

int runAmplifierSequence (vector <int>* originalAmplifierCode, vector <unsigned int>* amplifierSequence) {
    bool stillRunningFlag = false;
    unsigned int memoryPos = 0;
    vector <int> programmCounter;

    vector <vector <int>*> memory;
    for(unsigned int i=0; i<amplifierSequence->size(); ++i) {
        programmCounter.push_back(0);
        memory.push_back( new vector <int> (*originalAmplifierCode) );
        memory.push_back( new vector <int> ({ static_cast<int>(amplifierSequence->at(i)) }) );
        memory.push_back( new vector <int> );
    }
    memory.at(1)->push_back(0); // Setting first input value

    do {
        vector <int>* currentIntcodeMemory = memory.at(memoryPos%memory.size());
        vector <int>* currentInputMemory = memory.at((memoryPos+1)%memory.size());
        vector <int>* currentOutputMemory = memory.at((memoryPos+2)%memory.size());
        vector <int>* nextInputMemory = memory.at((memoryPos+4)%memory.size());

        programmCounter.at((memoryPos/3)%programmCounter.size()) = execOpcode(currentIntcodeMemory, currentInputMemory, currentOutputMemory, true, static_cast<unsigned int>(programmCounter.at((memoryPos/3)%programmCounter.size())), static_cast<unsigned int>( currentInputMemory->size()==2 ? 0 : currentInputMemory->size()-1 ) );
        nextInputMemory->push_back(currentOutputMemory->back());
        memoryPos+=3;

        stillRunningFlag = false;
        for(unsigned int i=0; i<programmCounter.size(); ++i) {
            if(programmCounter.at(i) > 0) {
                stillRunningFlag = true;
                break;
            }
        }

    } while (stillRunningFlag);
    return memory.back()->back();
}

void determineHighestSignal(vector <int>* amplifierCode) {
    int highestSignalValue = 0;
    string highestSignalSequence = "";

    for (unsigned int i1=minPhaseSetting; i1<=maxPhaseSetting; ++i1) {
        for (unsigned int i2=minPhaseSetting; i2<=maxPhaseSetting; ++i2) {
            for (unsigned int i3=minPhaseSetting; i3<=maxPhaseSetting; ++i3) {
                for (unsigned int i4=minPhaseSetting; i4<=maxPhaseSetting; ++i4) {
                    for (unsigned int i5=minPhaseSetting; i5<=maxPhaseSetting; ++i5) {

                        vector <unsigned int> amplifierSequence = {i1, i2, i3, i4, i5};
                        bool duplicateFlag = false;

                        for (unsigned int j=0; j<amplifierSequence.size(); ++j) {
                            for (unsigned int k=0; k<amplifierSequence.size(); ++k) {
                                if( j!=k && amplifierSequence.at(j) == amplifierSequence.at(k) ) {
                                    duplicateFlag = true;
                                    break;
                                }
                            }
                            if(duplicateFlag) break;
                        }
                        if (!duplicateFlag) {
                            int result = runAmplifierSequence(amplifierCode, &amplifierSequence);
                            if(result > highestSignalValue) {
                                highestSignalSequence = to_string(i1)+to_string(i2)+to_string(i3)+to_string(i4)+to_string(i5);
                                highestSignalValue = result;
                            }
                        }
                    }
                }
            }
        }
    }
    cout << "Max thruster signal: " << highestSignalValue << endl << "Phase setting sequence: " << highestSignalSequence << endl;
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
        determineHighestSignal(&intcode);
    } else {
        cerr << "Cannot open given file!" << endl;
    }
    return 0;
}