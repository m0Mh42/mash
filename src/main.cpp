#include <iostream>
#include <fstream>
#include <unistd.h>
#include <signal.h>
#include <cmath>
#include <chrono>
#include "errcod.hpp"
#include "file.h"
#include "mash.h"

#define memerr "Memory Error\n"

using namespace std;
using namespace chrono; // Time measurement

static bool run;
uint8_t difficulty;

// Ctrl-C
void intsig(int sig){
    cout << "Signal " << sig << endl;
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
void check_output(string data){
    if (difficulty > 0){
        for (int i = 0; i < difficulty; i++){
            // If the first characters defined by `difficulty` aren't '0', then we return, as it is false.
            if (data[i] != '0'){
                return;
            }
        }
        // Otherwise, it's the mash we were looking for and we print it, also stop running.
        cout << data << endl;
        run = false;
    } else {
        // There's no difficulty defined, printing the mash and sleeping.
        cout << data << endl;
        usleep(500 * 1000);
    }
}

// Checking difficulty declaration
void difficulty_set(char* difficulty_num){
    try {
        difficulty = stoi(difficulty_num);
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
    // The probability to find the mash we look for.
    probability = pow(16.00, x) / pow(16.00, 64.00);
    cout << "Finding Probability: " << probability << endl;
}

int main(int argc, char* argv[]) {

    // Filename is a must
    if (argc < 2){
        cout << "Usage: " << endl << argv[0] << " [file] [difficulty]" << endl;
        exit(NOFILE);
    }

    // Checking difficulty declaration
    if (argc > 2){
        difficulty_set(argv[2]);
    }

    string filename = argv[1];
    
    // File object
    File file;
    file.open_file(filename);

    // Mashing Object
    Mash mash;

    // Random seed
    mash.rand_seed();

    // Calculating chunk size.
    unsigned long len = file.get_file_length();
    unsigned long chunksize = len / 32;

    // Mashtree Object
    Mashtree mashtree(chunksize);
    string chunk = "";
    file.is_eof = false;

    signal(SIGINT, intsig);

    while (true){
        for (uint l = 0; l < 2; l++){
            if (!file.is_eof){
                chunk = file.read_chunk();
                mashtree.add_node(chunk);
            }
        }
        if (mashtree.tree_size > 1)
            mashtree.mash_k_matching_nodes();
        else
            break;
        if (file.is_eof && mashtree.tree_size > 1){
            mashtree.mash_remaining();
            break;
        }
    }

    // we don't need the file anymore, so we close it.
    file.close_file();

    // We get the resulted mash node, so we can mash it again, without using the nodetree.
    MashNode mashnode = mashtree.return_result();

    // And then we delete the nodetree to free memory.
    mashtree.delete_nodetree();

    // cout << endl << "Size of mashnode: " << sizeof(MashNode) << endl;
    // cout << "Size of mashtree: " << (sizeof(MashNode) * (chunksize)) << endl;
    // cout << "Size of mash class: " << sizeof(Mash) << endl;
    // cout << "Size of Mashtree class: " << sizeof(Mashtree) << endl;

    // Some initializations
    double microsecs = 0;
    run = true;
    string out;
    u_long count = 0;

    while (run) {
        // Measuring time taken to mash
        auto t1 = high_resolution_clock::now();

        mashnode = mash.mash(mashnode);

        auto t2 = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(t2 - t1);
        microsecs += duration.count();

        out = string_to_hex(mashnode.value);
        check_output(out);

        count++;
    }

    cout << endl << "Time taken by Mashing: " << (microsecs / 1000) << "ms" << endl;
    cout << "Count: " << count << endl;
    cout << "Exiting..." << endl;

    return 0;
}
