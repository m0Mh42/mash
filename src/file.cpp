#include <iostream>
#include <fstream>

using namespace std;

static int i;

string* Readfile(const string filename){
    ifstream file;
    string* buffer;
    buffer = new string[1024 * 1024 * 10];
    string* line;
    line = new string;
    i = 0;
    
    file.open(filename, ios::in | ios::binary);

    if (file.is_open()){
        while(getline(file, *line)){
            *(buffer + i) = *line;
            i++;
        }
    }
    
    return buffer;
}

int GetLineLen() {
    // static int i;
    return i;
}
