#include <fstream>
#include <unistd.h>
#include <signal.h>
#include <cmath>
#include <chrono>
#include "../inc/header.hpp"
#include "mash.cpp"
#include "file.cpp"

using namespace std;
using namespace chrono; // namespace of time measurement

static bool run;
unsigned short int difficulty;

// Ctrl-C
void IntSig(int signum){
    run = false;
}

// Convertion from string to hex
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

// Checking the chunk's first `difficulty` characters
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

    // Filename is a must
    if (argc < 2){
        cout << "Usage: " << endl << argv[0] << " [file] [difficulty]" << endl;
        exit(NOFILE);
    }

    // Checking difficulty declaration
    if (argc > 2){
        try {
            difficulty = stoi(argv[2]);
        } catch(...) {
            // Not Numeric Difficulty
            cout << "Invalid difficulty. Please insert number as difficulty." << endl;
            exit(USAGEERR);
        }
        if (difficulty > 63){
            // Difficulty can't be 64, so we cant find a result with all the characters '0'
            cout << "Invalid difficulty. Difficulty can\'t be more than 63." << endl;
            exit(INVALIDDIFF);
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
        exit(MEMERR);
    }

    // Reading the file data.
    *data = Readfile(filename);

    // Creating the mashing object
    Mash* mash;
    mash = new Mash;
    if (mash == nullptr){
        cout << memerr; 
        exit(MEMERR);
    }

    // Chunking the data.
    string* chunks = mash->chunkdata(data);
    if (chunks == nullptr){
        cout << memerr; 
        exit(MEMERR);
    }

    // We don't need it anymore
    delete data;

    // Initializing the mashing process
    signal(SIGINT, IntSig);
    mash -> rand_seed();
    string output;
    unsigned long long count = 0;
    double microsecs = 0;
    run = true;

    while (run){
        // Measuring the time taken to do mashing
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
