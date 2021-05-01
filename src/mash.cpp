#include <iostream>
#include "mash.h"
#include "errcod.hpp"
#include "version.h"

#define memerr "Memory Error.\n"

// Chunking data
string* Mash::chunkdata(string* input){
    string* data;
    data = new string;
    if (data == nullptr){
        cout << memerr;
        exit(MEMERR);
    }
    *data = *input;

    if (data->length() % 32 > 0){
        int len  = 32 - (data->length() % 32);
        for (int l = 0; l < len; l++){
            data->push_back((unsigned char) '0' + len);
        }
    } // Divisible to 32.

    cspace = data->length() / 32; // Each byte space
    chunk_count = cspace; // Chunk count
    string* chunks;

    if (cspace % 2 != 0)
        chunk_count++; // Extra chunk

    chunks = new string[chunk_count];
    prv_chunks = new string[chunk_count];
    if (chunks == nullptr){
        cout << memerr;
        exit(MEMERR);
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

// Mashing
MashNode Mash::mash(MashNode node){
    // Random Mash
    xor_random(node);
    
    // Mashing each character round
    for (uint i = 0; i < 16; i++){
        node.value[i] = node.value[i] ^ node.value[32 - i - 1];
    }
    return node;
}

// Random character generator.
void Mash::rand_c_generator(void){
    rand_c = rand();
}

// Mashing Random
MashNode Mash::xor_random(MashNode& node){
    for (int i = 0; i < 32; i++){
        rand_c_generator();
        node.value[i] ^= VERSION;
        node.value[i] ^= rand_c;
    }
    return node;
}

// Mashing Chunks
MashNode Mash::xor_chunks(MashNode& node1, MashNode& node2){
    for (int i = 0; i < 32; i++){
        node1.value[i] ^= VERSION;
        node1.value[i] ^= node2.value[i];
    }
    return node1;
}

// Random Number seeder.
void Mash::rand_seed(){
    time_t secs;
    secs = time(0);
    srand((unsigned int)secs);
}

// Returns chunk count.
int Mash::get_chunk_count(){
    return chunk_count;
}

Mashtree::Mashtree(uint chunksize){
    init_mashtree(chunksize);
}

void Mashtree::init_mashtree(uint chunksize){
    nodetree = new MashNode[chunksize];
    tree_size = 0;
    k = 0;
}

bool Mashtree::add_node(const string& value){
    MashNode node;
    node.key = 0;
    node.value = value;
    node.occupied = true;
    for (uint i = 0; i <= tree_size; i++){
        if ((nodetree + i)->occupied == false){
            *(nodetree + i) = node;
            tree_size++;
            return true;
        }
    }
    return false;
}

void Mashtree::mash_k_matching_nodes(){
    for (uint i = 0; i < tree_size; i++){
        for (uint n = 0; n < tree_size; n++){
            if (i == n)
                continue;
            if ((nodetree + i)->occupied && (nodetree + n)->occupied){
                if ((nodetree + i)->key == (nodetree + n)->key){
                    mash.xor_random(*(nodetree + i));
                    mash.xor_random(*(nodetree + n));
                    mash.xor_chunks(*(nodetree + i), *(nodetree + n));
                    (nodetree + i)->key += 1;
                    (nodetree + n)->occupied = false;
                    tree_size--;
                    i = 0;
                    n = 0;
                }
            }
        }
    }
}

void Mashtree::mash_remaining() {
    for (uint i = 0; i < tree_size; i++){
        for (uint n = 0; n < tree_size; n++){
            if (i == n)
                continue;
            if ((nodetree + i)->occupied && (nodetree + n)->occupied){
                mash.xor_random(*(nodetree + i));
                mash.xor_random(*(nodetree + n));
                mash.xor_chunks(*(nodetree + i), *(nodetree + n));
                (nodetree + i)->key += 1;
                (nodetree + n)->occupied = false;
                i = 0;
                n = 0;
            }
        }
    }
}

MashNode Mashtree::return_result(){
    return *(nodetree);
}