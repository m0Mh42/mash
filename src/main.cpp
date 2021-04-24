#include <fstream>
#include <cmath>
#include <unistd.h>
#include <signal.h>
#include "../inc/header.hpp"
#include "mash.cpp"
#include "memcheck.cpp"
#include "../inc/base64.hpp"
#include "file.cpp"

using namespace std;

static bool run;

void IntSig(int signum){
    run = false;
}

int main(int argc, char* argv[]) {
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

    string* output = new string;
    if (output == nullptr){
        cout << memerr; 
        exit(1);
    }

    mash -> rand_seed();

    macaron::Base64* b64c;
    b64c = new macaron::Base64;
    if (b64c == nullptr){
        cout << memerr; 
        exit(1);
    }

    string* b64 = new string;
    if (b64 == nullptr){
        cout << memerr; 
        exit(1);
    }

    run = true;

    signal(SIGINT, IntSig);

    while (run){
        *output = mash -> mash(*data);
        *b64 = b64c -> Encode(*output);
        cout << *b64 << endl;
        usleep(500 * 1000);
    }

    cout << endl << "Exiting..." << endl;

    delete mash;
    delete data;
    delete output;
    delete b64c;
    delete b64;
    
    return 0;
}
