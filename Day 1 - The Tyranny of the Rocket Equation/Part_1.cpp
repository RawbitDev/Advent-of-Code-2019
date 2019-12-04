#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char *argv[]) {
    string file_path = "input.txt";
    if(argc > 1) {
        file_path = argv[1];
    }
    ifstream file(file_path);
    if (file) {
        int fuel = 0;
        string line = "";
        while (getline(file, line)) {
            int mass = stoi(line);
            fuel += mass/3-2;
        }
        cout << "Required fuel: " << fuel << endl;
    } else {
        cerr << "Cannot open given file!" << endl;
    }
    return 0;
}