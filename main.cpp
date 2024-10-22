#include "hash.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[]){
    string result = "";
    if (argc != 4){
        cerr << "Usage ./a.out table_size hash_function(Last7|XOR) collision_method(Linear|Double)" << endl;
        return -1;
    }
    
    Hash* myHash = new Hash;
    result = myHash->Set_Up(atoi(argv[1]), argv[2], argv[3]);
    if (result.length() != 0){
        cerr << result << endl;
        return -1;
    }

    string line = "";
    char command = 'a';
    string key = "";
    string val = "";
    bool quit = false;
    while(getline(cin, line)){
        istringstream sin(line); 
        sin >> command;
        switch (command){
            case 'A':
                sin >> key;
                sin >> val;
                result = myHash->Add(key, val);
                    if (result.length() != 0){
                        cout << result << endl;
                    }
                break;
            case 'F':
                sin >> key;
                result = myHash->Find(key);
                    if (result.length() == 0){
                        cout << "Not Found" << endl;
                    } else {
                        cout << "Found " << result << endl;
                    }
                break;
            case 'P':
                myHash->Print();
                break;
            case 'T':
                cout << setw(5) << myHash->Total_Probes() << endl;
                break;
            case 'Q':
                quit = true;
                break;
            case '?':
                cout << "Commands" << endl;
                cout << "A key val (Add key/value pair to table)" << endl;
                cout << "F key (Find the value of the key in the table)" << endl;
                cout << "P (Print the total probes needed to find all keys)" << endl;
                cout << "Q (Quit)" << endl;
                cout << "? Print commands list" << endl;
                break;
            }
        if (quit){
            break;
        }
    }
}