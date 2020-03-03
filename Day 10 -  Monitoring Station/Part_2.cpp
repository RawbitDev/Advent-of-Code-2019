#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <cmath>
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
                else if(map.at(i).at(j) == -3) { s = "X"; }
                cout << right << setw(padding) << s;
            } else {
                cout << map.at(i).at(j);
            }
        }
        cout << endl;
    }
}

double getAngle(int X0, int Y0, int X1, int Y1, int X2, int Y2)
{
    // A = (0,0); B = (X2,Y2); C = (X1, Y1)
    // v -> CA; w -> CB;

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

vector <vector <double>> calculateAngleMap(vector <vector <char>> map, unsigned int X, unsigned int Y, int X0, int Y0) {
    vector <vector <double>> angleMap;
    for (unsigned int i=0; i<map.size(); ++i) {
        angleMap.push_back(vector <double>());
        for (unsigned int j=0; j<map.at(i).size(); ++j) {
            angleMap.at(i).push_back(-1);
            if(map.at(i).at(j) == '#' && (X != j || Y != i)) {
                angleMap.at(i).at(j) = getAngle(X0, Y0, X, Y, j, i);
            }
        }
    }
    angleMap.at(Y).at(X) = -2;
    return angleMap;
}

vector <vector <int>> calculateCounterMap(vector <vector <char>> map) {
    const int X0 = -126; // Just some random values as 'center point',
    const int Y0 = -612;  // make sure they're not on the map (-> negative values)
    vector <vector <int>> counterMap;
    for (unsigned int i=0; i<map.size(); ++i) {
        counterMap.push_back(vector <int>());
        for (unsigned int j=0; j<map.at(i).size(); ++j) {
            counterMap.at(i).push_back(-1);
            if(map.at(i).at(j) == '#') {
                vector <vector <double>> angleMap = calculateAngleMap(map, j, i, X0, Y0);
                counterMap.at(i).at(j) = countDetectableAsteroids(angleMap, j, i);
                cout << endl << "Angle map [X=" << j << "; Y=" << i << "]:" << endl;
                printMap(angleMap, 8);
            }
        }
    }
    return counterMap;
}

vector <vector <double>> fixAngleMap(vector <vector <double>> map, unsigned int X, unsigned int Y) {
    for (unsigned int i=0; i<Y; ++i) {
        for (unsigned int j=0; j<X; ++j) {
            double currentAngle = map.at(i).at(j);
            if (currentAngle >= 0) {
                map.at(i).at(j) = 360-currentAngle;
            }

        }
    }
    return map;
}

double getLowestAngle(vector <vector <double>> map, double lastValue) {
    double lowestAngle = 361;
    for (unsigned int i=0; i<map.size(); ++i) {
        for (unsigned int j=0; j<map.at(i).size(); ++j) {
            if(map.at(i).at(j) >= 0 && map.at(i).at(j) > lastValue && map.at(i).at(j) < lowestAngle) {
                lowestAngle = map.at(i).at(j);
            }
        }
    }
    return lowestAngle;
}

bool double_equals(double a, double b, double epsilon = 0.001)
{
    return abs(a - b) < epsilon;
}

static unsigned int vaporizeCount = 1;
static unsigned int X200th = 0;
static unsigned int Y200th = 0;
vector <vector <double>> vaporizeNextAsteroid(vector <vector <double>> map, double currentLowestAngle, int bestX, int bestY) {
    unsigned int closestX = -1;
    unsigned int closestY = -1;
    unsigned int closestDist = -1;
    for (unsigned int i=0; i<map.size(); ++i) {
        for (unsigned int j=0; j<map.at(i).size(); ++j) {
            if(double_equals(map.at(i).at(j), currentLowestAngle)) {
                unsigned int currentDist = abs((bestY)-static_cast<int>(i)) + abs((bestX)-static_cast<int>(j));
                if( currentDist < closestDist )  {
                    closestDist = currentDist;
                    closestX = j;
                    closestY = i;
                }
            }
        }
    }
    map.at(closestY).at(closestX) = -1;
    cout << "The " << vaporizeCount << " asteroid to be vaporized is at " << closestX << "," << closestY << "." << endl;
    if(vaporizeCount == 200) {
        X200th = closestX;
        Y200th = closestY;
    }
    ++vaporizeCount;
    return map;
}

static unsigned int roundCount = 1;
vector <vector <double>> vaporizeNextRound(vector <vector <double>> map, int bestX, int bestY) {
    vector<double> passedAngles;
    double currentLowestAngle = 0;
    passedAngles.push_back(-1);
    cout << endl << "Round " << roundCount << ":" << endl;
    while(true) {
        currentLowestAngle = getLowestAngle(map, passedAngles.back());
        passedAngles.push_back(currentLowestAngle);
        if(currentLowestAngle <= 360) {
            map = vaporizeNextAsteroid(map, currentLowestAngle, bestX, bestY);
        } else { break; }
    }
    ++roundCount;
    return map;
}

bool finishedVaporization(vector <vector <double>> map) {
    for (unsigned int i=0; i<map.size(); ++i) {
        for (unsigned int j=0; j<map.at(i).size(); ++j) {
            if(map.at(i).at(j) >= 0) {
                return true;
            }
        }
    }
    return false;
}

void vaporization(vector <vector <char>> map, int bestX, int bestY) {
    vector <vector <double>> bestAngleMap = calculateAngleMap(map, bestX, bestY, bestX, 0);
    bestAngleMap = fixAngleMap(bestAngleMap, bestX, bestAngleMap.size());
    bestAngleMap.at(bestY).at(bestX) = -3;
    cout << endl << endl << "---------------------------" << endl;
    cout << "Vaporization map [X=" << bestX << "; Y=" << bestY << "]:" << endl;
    printMap(bestAngleMap, 8);
    cout << "---------------------------";
    while(finishedVaporization(bestAngleMap)) {
        bestAngleMap = vaporizeNextRound(bestAngleMap, bestX, bestY);
    }
    cout << "---------------------------" << endl;
}

void determineBestAsteroid(vector <vector <int>> map, vector <vector <char>> rawMap) {
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
    vaporization(rawMap, bestX, bestY);
    cout << endl << "###########################"
         << endl << " 200th vaporization:"
         << endl << " X: " << X200th
         << endl << " Y: " << Y200th
         << endl << " [Final answer: " << X200th*100+Y200th << "]"
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
        determineBestAsteroid(counterMap, rawMap);
    } else {
        cerr << "Cannot open given file!" << endl;
    }
    return 0;
}