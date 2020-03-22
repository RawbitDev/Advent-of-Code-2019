#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <boost/math/common_factor.hpp>
using namespace std;

#define STEPS 1000
#define PADDING 3

struct moon {
    int posX;
    int posY;
    int posZ;
    int velX;
    int velY;
    int velZ;
    moon (int X_Coordinate, int Y_Coordinate, int Z_Coordinate) : posX(X_Coordinate), posY(Y_Coordinate), posZ(Z_Coordinate), velX(0), velY(0), velZ(0) {}
};
vector <moon> moons;

int prepareInput(string input) {
    for (int i=input.size()-1; i>=0; --i) {
        if(!isdigit(input.at(i)) && input.at(i) != '-') {
            input.erase(input.begin()+i);
        }
    }
    return stoi(input);
}

string intPadding(int input) {
    stringstream tmp;
    tmp << setfill(' ') << setw(PADDING) << input;
    return tmp.str();
}

void printMoons() {
    for(unsigned int i=0; i<moons.size(); ++i) {
        cout << "pos=<x=" << intPadding(moons.at(i).posX) << ", y=" << intPadding(moons.at(i).posY) << ", z=" << intPadding(moons.at(i).posZ) << ">, vel=<x=" << intPadding(moons.at(i).velX) << ", y=" << intPadding(moons.at(i).velY) << ", z=" << intPadding(moons.at(i).velZ) << ">" << endl;
    }
}

int calcGravity(int posA, int posB) {
    if(posA < posB) {
        return +1;
    } else if(posA > posB) {
        return -1;
    }
    return 0;
}

void applyGravity() {
    for (unsigned int i=0; i<moons.size(); ++i) {
        for (unsigned int j=0; j<moons.size(); ++j) {
            if(i != j) {
                moons.at(i).velX += calcGravity(moons.at(i).posX, moons.at(j).posX);
                moons.at(i).velY += calcGravity(moons.at(i).posY, moons.at(j).posY);
                moons.at(i).velZ += calcGravity(moons.at(i).posZ, moons.at(j).posZ);
            }
        }
    }
}

void applyVelocity() {
    for (unsigned int i=0; i<moons.size(); ++i) {
        moons.at(i).posX += moons.at(i).velX;
        moons.at(i).posY += moons.at(i).velY;
        moons.at(i).posZ += moons.at(i).velZ;
    }
}

bool initialX(vector <moon> initialMoons) {
    for (unsigned int i=0; i<moons.size(); ++i) {
        if (moons.at(i).posX != initialMoons.at(i).posX || moons.at(i).velX != 0) { return false; }
    }
    return true;
}
bool initialY(vector <moon> initialMoons) {
    for (unsigned int i=0; i<moons.size(); ++i) {
        if (moons.at(i).posY != initialMoons.at(i).posY || moons.at(i).velY != 0) { return false; }
    }
    return true;
}
bool initialZ(vector <moon> initialMoons) {
    for (unsigned int i=0; i<moons.size(); ++i) {
        if (moons.at(i).posZ != initialMoons.at(i).posZ || moons.at(i).velZ != 0) { return false; }
    }
    return true;
}

void calcStepsUntilInitialState() {
    long stepsX = 0, stepsY = 0, stepsZ = 0;
    bool doneX = false, doneY = false, doneZ = false;
    vector <moon> initialMoons = moons;
    cout << endl << "Initial State:" << endl;
    printMoons();
    do {
        applyGravity();
        applyVelocity();
        if(!doneX) {
            ++stepsX;
            if(initialX(initialMoons)) { doneX = true; }
        }
        if(!doneY) {
            ++stepsY;
            if(initialY(initialMoons)) { doneY = true; }
        }
        if(!doneZ) {
            ++stepsZ;
            if(initialZ(initialMoons)) { doneZ = true; }
        }
    } while(!doneX || !doneY || !doneZ);
    long totalSteps = boost::math::lcm(boost::math::lcm(stepsX, stepsY), stepsZ);
    cout << endl << "This set of initial positions takes " << totalSteps << " steps before it repeats a previous state!" << endl;
}

int main(int argc, char *argv[]) {
    string file_path = "input.txt";
    if(argc > 1) {
        file_path = argv[1];
    }
    ifstream file(file_path);
    if (file) {
        stringstream currentLine;
        string line="", current="";
        while(getline(file, line)) {
            vector <int> moonPos;
            currentLine << line;
            while (getline(currentLine, current, ',')) {
                moonPos.push_back(prepareInput(current));
            }
            moons.push_back(moon(moonPos.at(0), moonPos.at(1), moonPos.at(2)));
            moonPos.clear();
            currentLine.clear();
        }
        calcStepsUntilInitialState();
    } else {
        cerr << "Cannot open given file!" << endl;
    }
    return 0;
}