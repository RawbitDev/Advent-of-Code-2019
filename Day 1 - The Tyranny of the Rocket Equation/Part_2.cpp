#include <iostream>
#include <fstream>
using namespace std;

int fuelCalc(int mass) {
    int fuel = mass/3-2;
    if(fuel > 0) {
        fuel += fuelCalc(fuel);
    } else {
        fuel = 0;
    }
    return fuel;
}

int main(int argc, char *argv[]) {
    if(argc > 1) {
        string file_path = argv[1];
        ifstream file(file_path);
        if (file) {
            int fuel = 0;
            string line = "";
            while (getline(file, line)) {
                int mass = stoi(line);
                fuel += fuelCalc(mass);
            }
            cout << "Required fuel: " << fuel << endl;
        } else {
            cerr << "Cannot open given file!" << endl;
        }
    } else {
        cerr << "Missing file parameter!" << endl;
    }
    return 0;
}