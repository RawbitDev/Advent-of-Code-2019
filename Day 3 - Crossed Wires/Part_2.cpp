#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
using namespace std;

#define patternSize 50000

struct wirePart {
    char direction;
    unsigned int distance;
    wirePart(char dir, unsigned int dis) : direction(dir), distance(dis) {}
};

struct intersection {
    unsigned int X;
    unsigned int Y;
    unsigned int steps;
    intersection(unsigned int X_Coordinate, unsigned int Y_Coordinate, unsigned int Steps) : X(X_Coordinate), Y(Y_Coordinate), steps(Steps) {}
};

int main(int argc, char *argv[]) {
    string file_path = "input.txt";
    if(argc > 1) {
        file_path = argv[1];
    }

    cerr << "Reading input file..." << endl;
    ifstream file(file_path);
    if (file) {
        stringstream currentLine;
        string line="", current="";
        vector <vector <wirePart>> wires;
        vector <wirePart> wire;
        while(getline(file, line)) {
            currentLine << line;
            while (getline(currentLine, current, ',')) {
                char direction = current.front();
                current.erase(current.begin());
                unsigned int distance = stoi(current);
                wire.push_back(wirePart(direction, distance));
            }
            wires.push_back(wire);
            currentLine.clear();
            wire.clear();
        }
        cerr << "(Done)" << endl << endl;

        cerr << "Initialising grid pattern..." << endl;
        vector <vector<char>> pattern(patternSize, vector<char>(patternSize));
        for (unsigned int i=0; i<patternSize; ++i) {
            unsigned int percentage = (static_cast<double>(i)/static_cast<double>(patternSize))*100;
            cerr << "\r" << percentage << "%";
            for(unsigned int j=0; j<patternSize; ++j) {
                pattern[i][j] = '.';
            }
        }
        cerr << "\r(Done)          " << endl;

        unsigned int posX=0;
        unsigned int posY=0;

        cerr << endl << "Connecting wires..." << endl;
        for (unsigned int i=0; i<wires.size(); ++i) {
            cerr << "\r-> Generating " << i+1 << ". wire...                                                                                                                                       " << endl;
            vector <vector<char>> savedPattern(pattern);
            posX=patternSize/2;
            posY=patternSize/2;
            pattern[posX][posY] = 'O';
            for (unsigned int j=0; j<wires.at(i).size(); ++j) {
                cerr << "\r   Adding wire at X: " << posX << ", Y: " << posY << " [Direction: " << wires.at(i).at(j).direction << ", Distance: " << wires.at(i).at(j).distance << "]          ";
                switch (wires.at(i).at(j).direction) {
                    case 'D':
                        for (unsigned int k=0; k<wires.at(i).at(j).distance; ++k) {
                            ++posY;
                            if(savedPattern[posX][posY] == '.') {
                                pattern[posX][posY] = '|';
                            } else {
                                pattern[posX][posY] = 'X';
                            }
                        }
                        if(j+1 != wires.at(i).size()) {
                            pattern[posX][posY] = '+';
                        }
                        break;
                    case 'U':
                        for (unsigned int k=0; k<wires.at(i).at(j).distance; ++k) {
                            --posY;
                            if(savedPattern[posX][posY] == '.') {
                                pattern[posX][posY] = '|';
                            } else {
                                pattern[posX][posY] = 'X';
                            }
                        }
                        if(j+1 != wires.at(i).size()) {
                            pattern[posX][posY] = '+';
                        }
                        break;
                    case 'R':
                        for (unsigned int k=0; k<wires.at(i).at(j).distance; ++k) {
                            ++posX;
                            if(savedPattern[posX][posY] == '.') {
                                pattern[posX][posY] = '-';
                            } else {
                                pattern[posX][posY] = 'X';
                            }
                        }
                        if(j+1 != wires.at(i).size()) {
                            pattern[posX][posY] = '+';
                        }
                        break;
                    case 'L':
                        for (unsigned int k=0; k<wires.at(i).at(j).distance; ++k) {
                            --posX;
                            if(savedPattern[posX][posY] == '.') {
                                pattern[posX][posY] = '-';
                            } else {
                                pattern[posX][posY] = 'X';
                            }
                        }
                        if(j+1 != wires.at(i).size()) {
                            pattern[posX][posY] = '+';
                        }
                        break;
                    default:
                        throw runtime_error("ERROR: Encountered invalid direction!");
                }
            }
        }
        cerr << "\r(Done)                                                                                                                                       " << endl;

        posX=0;
        posY=0;

        cerr << endl << "Searching for intersection points..." << endl;
        vector <intersection> intersections;
        for (unsigned int i=0; i<wires.size(); ++i) {
            cerr << "\r-> Scanning " << i+1 << ". wire...                                                                                                                                       " << endl;
            unsigned int steps = 0;
            posX=patternSize/2;
            posY=patternSize/2;
            pattern[posX][posY] = 'O';
            for (unsigned int j=0; j<wires.at(i).size(); ++j) {
                cerr << "\r   Checking wire at X: " << posX << ", Y: " << posY << " [Direction: " << wires.at(i).at(j).direction << ", Distance: " << wires.at(i).at(j).distance << "]          ";
                switch (wires.at(i).at(j).direction) {
                    case 'D':
                        for (unsigned int k=0; k<wires.at(i).at(j).distance; ++k) {
                            ++posY;
                            ++steps;
                            if(pattern[posX][posY] == 'X') {
                                intersections.push_back(intersection(posX, posY, steps));
                            }
                        }
                        break;
                    case 'U':
                        for (unsigned int k=0; k<wires.at(i).at(j).distance; ++k) {
                            --posY;
                            ++steps;
                            if(pattern[posX][posY] == 'X') {
                                intersections.push_back(intersection(posX, posY, steps));
                            }
                        }
                        break;
                    case 'R':
                        for (unsigned int k=0; k<wires.at(i).at(j).distance; ++k) {
                            ++posX;
                            ++steps;
                            if(pattern[posX][posY] == 'X') {
                                intersections.push_back(intersection(posX, posY, steps));
                            }
                        }
                        break;
                    case 'L':
                        for (unsigned int k=0; k<wires.at(i).at(j).distance; ++k) {
                            --posX;
                            ++steps;
                            if(pattern[posX][posY] == 'X') {
                                intersections.push_back(intersection(posX, posY, steps));
                            }
                        }
                        break;
                    default:
                        throw runtime_error("ERROR: Encountered invalid direction!");
                }
            }
        }
        cerr << "\r(Done)                                                                                                                                       " << endl;

        cerr << endl << "Calculating intersection steps..." << endl;
        intersection closestIntersection(0,0,patternSize);
        for (unsigned int i=0; i<intersections.size(); ++i) {
            for (unsigned int j=0; j<intersections.size(); ++j) {
                if((i!=j) && (intersections.at(i).X == intersections.at(j).X) && (intersections.at(i).Y == intersections.at(j).Y)) {
                    if( (intersections.at(i).steps+intersections.at(j).steps) < closestIntersection.steps ) {
                        closestIntersection = intersection( intersections.at(i).X, intersections.at(i).Y, (intersections.at(i).steps+intersections.at(j).steps) );
                    }
                    cerr << "\r -> Intersection detected at X: " << j << ", Y: " << i << " [Steps: " << (intersections.at(i).steps+intersections.at(j).steps) << "]          ";
                }
            }
        }
        cerr << "\r(Done)                                                                                                                                       " << endl;

        cout << endl << "#######################"
             << endl << " Closest Intersection:"
             << endl << " X: " << closestIntersection.X
             << endl << " Y: " << closestIntersection.Y
             << endl << " [Steps: " << closestIntersection.steps << "]"
             << endl << "#######################" << endl;


    } else {
        cerr << "Cannot open given file!" << endl;
    }
    return 0;
}