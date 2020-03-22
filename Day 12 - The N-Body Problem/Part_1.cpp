#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
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

void calcTotalEnergy() {
    int totalEnergy = 0;
    vector <int> singleTotals;
    cout << "Energy after " << STEPS << " steps:" << endl;
    for (unsigned int i=0; i<moons.size(); ++i) {
        int pot = abs(moons.at(i).posX) + abs(moons.at(i).posY) + abs(moons.at(i).posZ);
        int kin = abs(moons.at(i).velX) + abs(moons.at(i).velY) + abs(moons.at(i).velZ);
        int total = pot * kin;
        singleTotals.push_back(total);
        totalEnergy += total;
        cout << "pot: " << intPadding(abs(moons.at(i).posX)) << " + " << intPadding(abs(moons.at(i).posY)) << " + " << intPadding(abs(moons.at(i).posZ)) << " = " << intPadding(pot)
             << ";   kin: " << intPadding(abs(moons.at(i).velX)) << " + " << intPadding(abs(moons.at(i).velY)) << " + " << intPadding(abs(moons.at(i).velZ)) << " = " << intPadding(kin)
             << ";   total: " << intPadding(pot) << " * " << intPadding(kin) << " = " << intPadding(total) << endl;
    }
    cout << "Sum of total energy: ";
    for (unsigned int i=0; i<singleTotals.size(); ++i) {
        cout << singleTotals.at(i);
        if(i<singleTotals.size()-1) { cout << " +  "; }
    }
    cout << " = " << totalEnergy << endl;
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
        for (unsigned int i=0; i<=STEPS; ++i) {
            if(i>0) {
                applyGravity();
                applyVelocity();
            }
            cout << "After " << i << " steps:" << endl;
            printMoons();
            cout << endl;
        }
        calcTotalEnergy();
    } else {
        cerr << "Cannot open given file!" << endl;
    }
    return 0;
}