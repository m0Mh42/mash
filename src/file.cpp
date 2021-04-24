#include <iostream>
#include <fstream>

using namespace std;


string Readfile(const string filename){
    ifstream file;
    string* buffer = new string;
    string *line = new string;
    file.open(filename, ios::in | ios::binary);

    if (file.is_open()){
        while(getline(file, *line)){
            buffer->append(*line);
            buffer->push_back('\n');
        }
    }

    return *buffer;
    
}

