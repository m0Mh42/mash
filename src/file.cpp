#include <iostream>
#include <string>
#include "file.h"

using namespace std;

void File::open_file(string filename){
    if(file.is_open()){
        file.close();
    }
    if (file.is_open() == false){
        file.open(filename, ios::binary);
    }
}

void File::close_file(){
    if(file.is_open()){
        file.close();
        is_eof = false;
    }
}

// Chunks are 32 bytes.
string File::read_chunk() {
    i = 0;
    chunk = "";
    char chr;
    if (file.is_open()){
        while (i < 32){
            if(!is_eof && file.get(chr)){
                chunk.push_back(chr);
                i++;
            } else {
                is_eof = true;
                break;
            }
        }
    }
    if (chunk.length() < 32){
        u_char chr;
        uint8_t len = 32 - chunk.length();
        for (uint8_t i = 0; i < len; i++){
            chr = '0' + len;
            chunk.push_back(chr);
        }
    }
    return chunk;
}

u_long File::get_file_length(){
    file.seekg(0, ios::end);
    len = file.tellg();
    file.seekg(0, ios::beg);
    return len;
}