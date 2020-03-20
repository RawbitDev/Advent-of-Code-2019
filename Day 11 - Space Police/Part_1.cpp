#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

#define SURFACE_Y 200
#define SURFACE_X 200
vector <vector <char>*> surface;

enum direction {NORTH, EAST, SOUTH, WEST};

struct position {
    int Y;
    int X;
    position (int Y_Coordinate, int X_Coordinate) : Y(Y_Coordinate), X(X_Coordinate) {}
};
vector<position> painted;

struct positionRobot {
    int Y;
    int X;
    direction dir;
    positionRobot (int Y_Coordinate, int X_Coordinate) : Y(Y_Coordinate), X(X_Coordinate), dir(NORTH) {}
} posRobot(SURFACE_Y/2, SURFACE_X/2);

void initSurface() {
    for (unsigned int i=0; i<SURFACE_Y; ++i) {
        surface.push_back(new vector <char>());
        for (unsigned int j=0; j<SURFACE_X; ++j) {
            surface.at(i)->push_back('.');
        }
    }
}

void printRobot() {
    if(posRobot.dir == NORTH) {
        cout << '^';
    } else if (posRobot.dir == EAST) {
        cout << '>';
    } else if (posRobot.dir == SOUTH) {
        cout << 'v';
    } else if (posRobot.dir == WEST) {
        cout << '<';
    } else {
        cout << 'X';
    }
}

void printSurface() {
    for (unsigned int i=0; i<surface.size(); ++i) {
        cout << endl;
        for (unsigned int j=0; j<surface.at(i)->size(); ++j) {
            if(i == static_cast<unsigned int>(posRobot.Y) && j == static_cast<unsigned int>(posRobot.X)) {
                printRobot();
            } else {
                cout << surface.at(i)->at(j);
            }
        }
    }
    cout << endl;
}

void move() {
    try {
        if(posRobot.dir == NORTH) {
            if(posRobot.Y-1 >= 0) { posRobot.Y-=1; } else { throw runtime_error("ERROR: Encountered invalid NORTH movement!");  }
        } else if (posRobot.dir == EAST) {
            if(posRobot.X+1 < SURFACE_X) { posRobot.X+=1; } else { throw runtime_error("ERROR: Encountered invalid EAST movement!");  }
        } else if (posRobot.dir == SOUTH) {
            if(posRobot.Y+1 < SURFACE_Y) { posRobot.Y+=1; } else { throw runtime_error("ERROR: Encountered invalid SOUTH movement!");  }
        } else if (posRobot.dir == WEST) {
            if(posRobot.X-1 >= 0) { posRobot.X-=1; } else { throw runtime_error("ERROR: Encountered invalid WEST movement!");  }
        }
    } catch (const exception& e) {
        cout << e.what();
        exit(-1);
    }
}

void turn(int dir) {
    if(dir == 0) { // Turn left
        posRobot.dir = direction( (posRobot.dir+3)%4 );
    } else if(dir == 1) { // Turn right
        posRobot.dir = direction( (posRobot.dir+1)%4 );
    } else {
        cerr << "Error: Invalid direction input!" << endl;
    }
}

void logPainting(int Y, int X) {
    for (unsigned int i=0; i<painted.size(); ++i) {
        if(painted.at(i).Y == Y && painted.at(i).X == X) {
            return;
        }
    }
    painted.push_back(position(Y, X));
}

void paint(int color) {
    if(color == 0) { // Paint black
        surface.at(posRobot.Y)->at(posRobot.X) = '.';
    } else if(color == 1) { // Paint white
        surface.at(posRobot.Y)->at(posRobot.X) = '#';
    } else {
        cerr << "Error: Invalid color input!" << endl;
        return;
    }
    logPainting(posRobot.Y, posRobot.X);
}

int accessCamera() {
    if(surface.at(posRobot.Y)->at(posRobot.X) == '.') { // Color black
        return 0;
    } else if(surface.at(posRobot.Y)->at(posRobot.X) == '#') { // Color white
        return 1;
    } else {
        cerr << "Error: Invalid color detected!" << endl;
        return -1;
    }
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

    initSurface();
    vector<long> robotInput;
    robotInput.clear();

    while( i<intcode->size() ) {

        if(robotInput.size() >= 2) {
            paint(robotInput.at(0));
            turn(robotInput.at(1));
            move();
            robotInput.clear();
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
                long tmpInput = accessCamera();
                //cout << "> " << tmpInput << endl;
                //cin >> tmpInput;
                *param1 = tmpInput;
                i+=2;
                break;
            }

            case 4: { // Output
                //cout << *param1 << endl;
                robotInput.push_back(*param1);
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
        execOpcode(&intcode);
        cout << "Panels painted at least once: " << painted.size() << endl;
    } else {
        cerr << "Cannot open given file!" << endl;
    }
    return 0;
}