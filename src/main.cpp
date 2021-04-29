#include <fstream>
#include <cmath>
#include <unistd.h>
#include <signal.h>
#include "../inc/header.hpp"
#include "mash.cpp"
#include "file.cpp"

using namespace std;

static bool run;
unsigned short int difficulty;

void IntSig(int signum){
    run = false;
}

string string_to_hex(const string& input)
{
    static const char hex_digits[] = "0123456789ABCDEF";

    string output;
    output.reserve(input.length() * 2);
    for (unsigned char c : input)
    {
        output.push_back(hex_digits[c >> 4]);
        output.push_back(hex_digits[c & 15]);
    }
    return output;
}

bool check_output(string data){
    if (difficulty > 0){
        for (int i = 0; i < difficulty; i++){
            if (data[i] != '0'){
                return false;
            }
        }
        return true;
    } else {
        cout << data << endl;
        usleep(500 * 1000);
        return false;
    }
}

int main(int argc, char* argv[]) {

    if (argc > 2){
        difficulty = stoi(argv[2]);
        if (difficulty > 64){
            cout << "Invalid difficulty" << endl;
            exit(254);
        }
        cout << "Difficulty set to " << difficulty << endl;
    }

    string filename = argv[1];
    string* data = new string;
    if (data == nullptr){
        cout << memerr; 
        exit(1);
    }

    *data = Readfile(filename);

    Mash* mash;
    mash = new Mash;
    if (mash == nullptr){
        cout << memerr; 
        exit(1);
    }

    string* chunks = mash->chunkdata(data);
    if (chunks == nullptr){
        cout << memerr; 
        exit(1);
    }

    delete data;

    mash -> rand_seed();

    string output;

    run = true;

    signal(SIGINT, IntSig);

    int count = 0;

    while (run){
        output = mash -> mash(chunks);
        output = string_to_hex(output);
        if(check_output(output)){
            cout << output << endl;
            run = false;
        }
        count++;
    }

    cout << endl << "Count: " << count << endl;
    cout << "Exiting..." << endl;

    delete mash;
    delete[] chunks;
    
    return 0;
}
