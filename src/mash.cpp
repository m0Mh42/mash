#include <random>
#include <ctime>
#include "../inc/header.hpp"

using namespace std;

class Mash {
    public:
        string mash(string* chunks);
        string* chunkdata(string* input);
        void rand_seed();
        int get_chunk_count();
    private:
        void rand_c_generator(void);
        string xor_chunks(string chunk1, string chunk2);
        string xor_random(string chunk);
        unsigned char rand_c;
        int cspace;
        int chunk_count;
        string* prv_chunks;
};

string* Mash::chunkdata(string* input){
    string* data;
    data = new string;
    if (data == nullptr){
        cout << memerr;
        exit(1);
    }
    *data = *input;

    if (data->length() % 32 > 0){
        int len  = 32 - (data->length() % 32);
        for (int l = 0; l < len; l++){
            data->push_back((unsigned char) '0' + len);
        }
    } // Divisibled to 32.

    cspace = data->length() / 32; // Each byte space
    chunk_count = cspace; // Chunk count
    string* chunks;

    if (cspace % 2 != 0)
        chunk_count++; // Extra chunk

    chunks = new string[chunk_count];
    prv_chunks = new string[chunk_count];
    if (chunks == nullptr){
        cout << memerr;
        exit(1);
    }

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

    delete data;
    return chunks;
}

string Mash::mash(string* chunks){
    for (int i = 0; i < chunk_count; i++){
        *(prv_chunks + i) = *(chunks + i);
    }

    for (int c = 0; c < chunk_count; c++){ // Initial Random Mash
        *(prv_chunks + c) = xor_random(*(prv_chunks + c));
    }

    while (chunk_count != 1){ // Mashing each chunk with other.
        for (int i = 0; i < chunk_count / 2; i++){
            *(prv_chunks + i) = xor_chunks(*(prv_chunks + (i * 2)), *(prv_chunks + ((i * 2) + 1)));
        }
        chunk_count /= 2;
    }

    return *prv_chunks;
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
        chunk1[i] = chunk1[i] ^ chunk2[i];
    }
    return chunk1;
}

void Mash::rand_seed(){
    time_t secs;
    secs = time(0);
    srand((unsigned int)secs);
}

int Mash::get_chunk_count(){
    return chunk_count;
}
