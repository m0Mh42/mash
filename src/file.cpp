#include <iostream>
#include <fstream>
#include "../inc/errcod.hpp"

using namespace std;


string Readfile(const string filename){
    ifstream file;
    const string _memerr = "Memory Error\n";
    string* buffer = new string;
    if (buffer == nullptr){
        cout << _memerr;
        exit(MEMERR);
    }
    string *line = new string;
    if (line == nullptr){
        cout << _memerr;
        exit(MEMERR);
    }
    file.open(filename, ios::in | ios::binary);

    if (file.is_open()){
        while(getline(file, *line)){
            buffer->append(*line);
            buffer->push_back('\n');
        }
    }

    return *buffer;
    
}

