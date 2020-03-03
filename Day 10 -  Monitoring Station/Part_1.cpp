#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <math.h>
using namespace std;

#define DEBUG 0
const double PI = 3.14159265;

template <typename T>
void printMap(vector <vector <T>> map, unsigned int padding = 0) {
    for (unsigned int i=0; i<map.size(); ++i) {
        for (unsigned int j=0; j<map.at(i).size(); ++j) {
            if(padding) {
                stringstream tmp;
                tmp << setprecision(3) << fixed << map.at(i).at(j);
                string s = tmp.str();
                tmp.str(string());
                if(map.at(i).at(j) == -1) { s = "."; } // Just to optimize the output
                else if(map.at(i).at(j) == -2) { s = "#"; }
                cout << right << setw(padding) << s;
            } else {
                cout << map.at(i).at(j);
            }
        }
        cout << endl;
    }
}

double getAngle(int X1, int Y1, int X2, int Y2)
{
    // A = (0,0); B = (X2,Y2); C = (X1, Y1)
    // v -> CA; w -> CB;
    const int X0 = -126; // Just some random values as 'center point',
    const int Y0 = -612;  // make sure they're not on the map (-> negative values)

    int v_X = X0-X1;
    int v_Y = Y0-Y1;
    int w_X = X2-X1;
    int w_Y = Y2-Y1;

    double vw_scalar = (v_X*w_X) + (v_Y*w_Y);

    double v_length = sqrt(pow(v_X, 2) + pow(v_Y, 2));
    double w_length = sqrt(pow(w_X, 2) + pow(w_Y, 2));

    double vw_angle = acos(vw_scalar / (v_length * w_length));
    double deg = (vw_angle * 180) / PI;

    if(DEBUG) {
        cout << "[ANGLE] For v=(" << v_X << ", " << v_Y << ") & w=(" << w_X << ", " << w_Y << ")" << endl;
        cout << "[ANGLE] Scalar -> " << vw_scalar << endl;
        cout << "[ANGLE] Length of v -> " << v_length << endl;
        cout << "[ANGLE] Length of w -> " << w_length << endl;
        cout << "[ANGLE] Angle -> " << vw_angle << endl;
        cout << "[ANGLE] Deg -> " << deg << endl << endl;
    }

    stringstream tmp; // Rounding the angle value
    tmp << setprecision(3) << fixed << deg;
    deg = stod(tmp.str());
    tmp.str(string());

    return deg;
}

unsigned int countDetectableAsteroids(vector <vector <double>> map, unsigned int X, unsigned int Y) {
    unsigned int count = 0;
    vector <vector <char>> detectableMap;
    vector <double> passedAngles;
    for (unsigned int i=0; i<map.size(); ++i) {
        detectableMap.push_back(vector <char>());
        for (unsigned int j=0; j<map.at(i).size(); ++j) {
            detectableMap.at(i).push_back('.');
            if(map.at(i).at(j) != -1 && (X != j || Y != i)) {
                for(unsigned int k=0; k<passedAngles.size(); ++k) {
                    if(passedAngles.at(k) == map.at(i).at(j)) {
                        detectableMap.at(i).at(j) = 'X';
                    }
                }
                if(detectableMap.at(i).at(j) != 'X') {
                    passedAngles.push_back(map.at(i).at(j));
                    detectableMap.at(i).at(j) = '+';
                    ++count;
                };
            }
        }
    }
    detectableMap.at(Y).at(X) = '#';
    cout << endl << "Detectable map [X=" << X << "; Y=" << Y << "]:" << endl;
    printMap(detectableMap);
    cout << "Count: (" << count << ")" << endl;
    return count;
}

vector <vector <double>> calculateAngleMap(vector <vector <char>> map, unsigned int X, unsigned int Y) {
    vector <vector <double>> angleMap;
    for (unsigned int i=0; i<map.size(); ++i) {
        angleMap.push_back(vector <double>());
        for (unsigned int j=0; j<map.at(i).size(); ++j) {
            angleMap.at(i).push_back(-1);
            if(map.at(i).at(j) == '#' && (X != j || Y != i)) {
                angleMap.at(i).at(j) = getAngle(X, Y, j, i);
            }
        }
    }
    angleMap.at(Y).at(X) = -2;
    cout << endl << "Angle map [X=" << X << "; Y=" << Y << "]:" << endl;
    printMap(angleMap, 8);
    return angleMap;
}

vector <vector <int>> calculateCounterMap(vector <vector <char>> map) {
    vector <vector <int>> counterMap;
    for (unsigned int i=0; i<map.size(); ++i) {
        counterMap.push_back(vector <int>());
        for (unsigned int j=0; j<map.at(i).size(); ++j) {
            counterMap.at(i).push_back(-1);
            if(map.at(i).at(j) == '#') {
                counterMap.at(i).at(j) = countDetectableAsteroids(calculateAngleMap(map, j, i), j, i);
            }
        }
    }
    return counterMap;
}

void determineBestAsteroid(vector <vector <int>> map) {
    int bestCount = 0;
    unsigned int bestX = 0;
    unsigned int bestY = 0;
    for (unsigned int i=0; i<map.size(); ++i) {
        for (unsigned int j=0; j<map.at(i).size(); ++j) {
            if(map.at(i).at(j) > bestCount) {
                bestCount = map.at(i).at(j);
                bestX = j;
                bestY = i;
            }
        }
    }
    cout << endl << "###########################"
         << endl << " Best location:"
         << endl << " X: " << bestX
         << endl << " Y: " << bestY
         << endl << " [Detectable asteroids: " << bestCount << "]"
         << endl << "###########################" << endl;
}

int main(int argc, char *argv[]) {
    string file_path = "input.txt";
    if(argc > 1) {
        file_path = argv[1];
    }
    ifstream file(file_path);
    if (file) {
        string line = "";
        vector <vector <char>> rawMap;
        while (getline(file, line)) {
            vector <char> singleLine;
            for (unsigned int i=0; i<line.size(); ++i) {
                if(line.at(i) == '.' || line.at(i) == '#') {
                    singleLine.push_back(line.at(i));
                }
            }
            rawMap.push_back(singleLine);
        }
        vector <vector <int>> counterMap = calculateCounterMap(rawMap);
        cout << endl << endl << endl << endl << "##########" << endl;
        cout << "# RESULT #" << endl;
        cout << "##########" << endl;
        cout << endl << "---------------------------" << endl;
        cout << "Raw map:" << endl;
        printMap(rawMap);
        cout << "---------------------------" << endl;
        cout << endl << "---------------------------" << endl;
        cout << "Counter map:" << endl;
        printMap(counterMap, 5);
        cout << "---------------------------" << endl;
        determineBestAsteroid(counterMap);
    } else {
        cerr << "Cannot open given file!" << endl;
    }
    return 0;
}