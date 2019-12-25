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

static unsigned int orbitDistance=0;
void distanceOrbits(orbit* currentOrbit, orbit* destinationOrbit) {
    if(currentOrbit != destinationOrbit && currentOrbit->orbitsPtr != nullptr) {
        ++orbitDistance;
        distanceOrbits(currentOrbit->orbitsPtr, destinationOrbit);
    }
}

orbit* searchFirstMeetingOrbit(orbit* currentOrbit, orbit* destinationOrbit) {
    orbit* current = currentOrbit;
    while(current != nullptr) {
        orbit* destination = destinationOrbit;
        while(destination != nullptr) {
            if(current == destination) {
                return current;
            }
            destination = destination->orbitsPtr;
        }
        current = current->orbitsPtr;
    }
    cerr << "ERROR: Problem while searching first meeting orbit!" << endl;
    exit(1);
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

        orbit* youOrbit = searchOrbit("YOU");
        orbit* sanOrbit = searchOrbit("SAN");
        orbit* firstMeetingOrbit = searchFirstMeetingOrbit( youOrbit, sanOrbit );

        distanceOrbits(youOrbit->orbitsPtr, firstMeetingOrbit);
        distanceOrbits(sanOrbit->orbitsPtr, firstMeetingOrbit);
        cout << orbitDistance << " orbital transfers are required to get to santa!" << endl;

    } else {
        cerr << "Cannot open given file!" << endl;
    }
    return 0;
}