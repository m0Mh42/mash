#include <random>
#include <ctime>
#include "../inc/header.hpp"

using namespace std;

class Mash {
    public:
        string mash(string &input);
        void reset_seed();
    private:
        void rand_c_generator(void);
        string xor_chunks(string chunk1, string chunk2);
        string xor_random(string chunk);
        unsigned char rand_c;
};

string Mash::mash(string &input){
    string* data;
    string extrablockchars = "1234567890qwertuiopasdfghjklzxcvb";
    data = new string;
    *data = input;

    if (data->length() % 32 > 0){
        int len  = 32 - (data->length() % 32);
        int c = 0;
        for (int l = 0; l < len; l++){
            data->push_back(extrablockchars[c]);
            c++;
        }
    } // Divisibled to 32.

    int cspace = data->length() / 32;
    int chunk_count = cspace;
    string* chunks;

    if (cspace % 2 != 0)
        chunk_count++;

    chunks = new string[chunk_count];

    for (int a = 0; a < cspace; a++){ // Chunk all data
        for (int r = 0; r < 32; r++){
            (chunks + a)->push_back((*data)[r * cspace + a]);
        }
    }

    if (cspace != chunk_count){ // Last empty chunk
        for (int i = 0; i < 32; i++){
            rand_c_generator();
            (chunks + cspace)->push_back(rand_c);
        }
    }

    for (int c = 0; c < chunk_count; c++){ // Initial Random Mash
        *(chunks + c) = xor_random(*(chunks + c));
    }

    while (chunk_count != 1){ // Mashing each chunk with other.
        for (int i = 0; i < chunk_count / 2; i++){
            *(chunks + i) = xor_chunks(*(chunks + (i * 2)), *(chunks + ((i * 2) + 1)));
        }
        chunk_count /= 2;
    }
    return *chunks;

}

void Mash::rand_c_generator(void){
    rand_c = rand();
}

string Mash::xor_random(string chunk){
    for (int i = 0; i < 32; i++){
        rand_c_generator();
        chunk[i] = chunk[i] ^ rand_c;
    }
    return chunk;
}

string Mash::xor_chunks(string chunk1, string chunk2){
    for (int i = 0; i < 32; i++){
        chunk1[i] = chunk1[i] ^ chunk2[i];
    }
    return chunk1;
}

void Mash::reset_seed(){
    time_t secs;
    secs = time(0);
    srand((unsigned int)secs);
}
