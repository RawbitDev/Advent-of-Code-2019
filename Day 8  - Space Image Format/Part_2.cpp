#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#define WIDE 25
#define TALL 6

vector <vector <char>> renderLayer(vector <vector <vector <char>>> *layers) {
    vector <vector <char>> renderedLayer = layers->at(0);
    for(unsigned int i=1; i<layers->size(); ++i) {
        for (unsigned int j=0; j<layers->at(i).size(); ++j) {
            for (unsigned int k=0; k<layers->at(i).at(j).size(); ++k) {
                if((layers->at(i).at(j).at(k) != '2') && (renderedLayer.at(j).at(k) != '0' && renderedLayer.at(j).at(k) != '1')) {
                    renderedLayer.at(j).at(k) = layers->at(i).at(j).at(k);
                }
            }
        }
    }
    return renderedLayer;
}

void printSingleLayer(vector <vector <char>> layer, bool optimizedView = false) {
    for(unsigned int i=0; i<layer.size(); ++i) {
        for(unsigned int j=0; j<layer.at(i).size(); ++j) {
            if(optimizedView) {
                if(layer.at(i).at(j) == '0') {
                    cout << " ";
                } else {
                    cout << "#";
                }
            } else {
                cout << layer.at(i).at(j);
            }
        }
        cout << endl;
    }
    cout << endl;
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

        for(unsigned int i=0; i<layers->size(); ++i) {
            cout << "Layer " << i+1 << ":" << endl;
            printSingleLayer(layers->at(i));
        }

        cout << "Done!" << endl;
        vector <vector <char>> renderedLayer = renderLayer(layers);
        cout << endl << "Rendered image:" << endl;
        printSingleLayer(renderedLayer);
        cout << endl << "Optimized view:" << endl;
        printSingleLayer(renderedLayer, true);
    } else {
        cerr << "Cannot open given file!" << endl;
    }
    return 0;
}
