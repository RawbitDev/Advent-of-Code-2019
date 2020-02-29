#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#define WIDE 25
#define TALL 6

unsigned int countCharsInLayer(vector <vector <char>> layer, char givenChar) {
    unsigned int charCount = 0;
    for(unsigned int i=0; i<layer.size(); ++i) {
        for(unsigned int j=0; j<layer.at(i).size(); ++j) {
            if(layer.at(i).at(j) == givenChar) {
                ++charCount;
            }
        }
    }
    return charCount;
}

vector <vector <char>> determineFewestZeros(vector <vector <vector <char>>> *layers) {
    vector <vector <char>> fewestZerosLayer;
    unsigned int zeroCount = 0;
    unsigned int fewestZeros = static_cast<unsigned int>(-1);
    for(unsigned int i=0; i<layers->size(); ++i) {
        zeroCount = countCharsInLayer(layers->at(i), '0');
        if(zeroCount < fewestZeros) {
            fewestZeros = zeroCount;
            fewestZerosLayer = layers->at(i);
        }
    }
    return fewestZerosLayer;
}

void printSingleLayer(vector <vector <char>> layer, bool count = false) {
    for(unsigned int i=0; i<layer.size(); ++i) {
        for(unsigned int j=0; j<layer.at(i).size(); ++j) {
            cout << layer.at(i).at(j);
        }
        cout << endl;
    }
    if(count) {
        cout << "Number of '0' digits -> [" << countCharsInLayer(layer, '0') << "]" << endl;
        cout << "Number of '1' digits -> [" << countCharsInLayer(layer, '1') << "]" << endl;
        cout << "Number of '2' digits -> [" << countCharsInLayer(layer, '2') << "]" << endl;
        cout << endl;
    }
}

int main(int argc, char *argv[]) {
    string file_path = "input.txt";
    if(argc > 1) {
        file_path = argv[1];
    }
    ifstream file(file_path);
    if (file) {
        string rawLine = "";
        vector <vector <vector <char>>> *layers = new vector <vector <vector <char>>> ();
        vector <vector <char>> *currentLayer = new vector <vector <char>> ();
        vector <char> *currentLine = new vector <char> ();
        while (getline(file, rawLine)) {
            for (unsigned int i=0; i<rawLine.size() ; ++i) {
                currentLine->push_back(rawLine.at(i));
                if(i % WIDE == WIDE-1) {
                    currentLayer->push_back(*currentLine);
                    currentLine = new vector <char> ();
                    if(currentLayer->size() >= TALL) {
                        layers->push_back(*currentLayer);
                        currentLayer = new vector <vector <char>> ();
                    }
                }
            }
        }
        cout << "Done!" << endl;
        vector <vector <char>> fewestZerosLayer = determineFewestZeros(layers);
        cout << endl << "Layer with the fewest number of zeros found:" << endl;
        printSingleLayer(fewestZerosLayer);
        cout << endl << "Counting chars..." << endl;
        unsigned int count1Digits = countCharsInLayer(fewestZerosLayer, '1');
        unsigned int count2Digits = countCharsInLayer(fewestZerosLayer, '2');
        cout << "Number of '0' digits -> [" << countCharsInLayer(fewestZerosLayer, '0') << "]" << endl;
        cout << "Number of '1' digits -> [" << count1Digits << "]" << endl;
        cout << "Number of '2' digits -> [" << count2Digits << "]" << endl;
        cout << endl << "FINAL: Number of '1' digits multiplied by the number of '2' digits -> (" << count1Digits*count2Digits << ")" << endl << endl;
    } else {
        cerr << "Cannot open given file!" << endl;
    }
    return 0;
}
