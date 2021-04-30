#include <fstream>
#include <unistd.h>
#include <signal.h>
#include <cmath>
#include <chrono>
#include "../inc/header.hpp"
#include "mash.cpp"
#include "file.cpp"

using namespace std;
using namespace chrono;

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
        if (difficulty > 63){
            cout << "Invalid difficulty" << endl;
            exit(254);
        }
        cout << "Difficulty set to " << difficulty << endl;
        double probability;
        double x = 64.00 - difficulty;
        probability = pow(16.00, x) / pow(16.00, 64.00);
        cout << "Finding Probability: " << probability  << " out of 1 try"<< endl;
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

    unsigned long long count = 0;

    double microsecs = 0;

    while (run){
        auto t1 = high_resolution_clock::now();
        output = mash -> mash(chunks);
        auto t2 = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(t2 - t1);
        microsecs += duration.count();
        output = string_to_hex(output);
        if(check_output(output)){
            cout << endl << output << endl;
            run = false;
        }
        count++;
    }

    cout << endl << "Duration: " << (microsecs / 1000) << "ms" << endl;
    cout << "Count: " << count << endl;
    cout << "Exiting..." << endl;

    delete mash;
    delete[] chunks;
    
    return 0;
}
