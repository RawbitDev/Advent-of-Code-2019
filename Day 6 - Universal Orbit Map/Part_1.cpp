#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

struct orbit {
    string id;
    string orbits;
    orbit* orbitsPtr;
    orbit(string ID, string Orbits) : id(ID), orbits(Orbits), orbitsPtr(nullptr) {}
};
static vector <orbit*> orbitMap;

orbit* searchOrbit(string id) {
    for(unsigned int i=0; i<orbitMap.size(); ++i) {
        if(orbitMap.at(i)->id == id) {
            return orbitMap.at(i);
        }
    }
    cerr << "ERROR: Problem while searching for orbit " << id << "!" << endl;
    exit(1);
}

void linkOrbits() { // Linking the orbits to an actual object
    for(unsigned int i=0; i<orbitMap.size(); ++i) {
        if (orbitMap.at(i)->id != "COM") {
            orbitMap.at(i)->orbitsPtr = searchOrbit(orbitMap.at(i)->orbits);
        }
    }
}

static unsigned int orbitCounter=0;
void countOrbits(orbit* currentOrbit) {
    if(currentOrbit->orbitsPtr != nullptr) {
        ++orbitCounter;
        countOrbits(currentOrbit->orbitsPtr);
    }
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
        vector <string> tmp;
        orbitMap.push_back(new orbit("COM", "COM")); // COM orbits nothing
        while(getline(file, line)) {
            tmp.clear();
            currentLine.clear();
            currentLine << line;
            while (getline(currentLine, current, ')')) {
                tmp.push_back(current);
            }
            orbitMap.push_back(new orbit( tmp.at(1), tmp.at(0) )); // Create all orbit objects before linking them to avoid any sorting
        }
        linkOrbits();

        for(unsigned int i=0; i<orbitMap.size(); ++i) { // Count total number of orbits
            countOrbits(orbitMap.at(i));
        }
        cout << "Total number of orbits: " << orbitCounter << endl;

    } else {
        cerr << "Cannot open given file!" << endl;
    }
    return 0;
}