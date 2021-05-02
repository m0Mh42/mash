#include <random>
#include <ctime>

using namespace std;

typedef struct {
    uint16_t key;
    string value;
    bool occupied = false;
} MashNode;

class Mash {
    public:
        MashNode mash(MashNode node);
        string* chunkdata(string* input);
        void rand_seed();
        int get_chunk_count();
        MashNode xor_random(MashNode& node);
        MashNode xor_chunks(MashNode& node1, MashNode& node2);
    private:
        void rand_c_generator(void);
        u_char rand_c;
        int cspace;
        int chunk_count;
        string* prv_chunks; // Private chunks to work at
};

class Mashtree {
    public:
        Mashtree(uint chunksize);
        void init_mashtree(uint chunksize);
        bool add_node(const string& value);
        void mash_k_matching_nodes();
        void mash_remaining();
        MashNode return_result();
        void delete_nodetree();
        uint tree_size;
    private:
        uint k;
        Mash mash;
        MashNode* nodetree; // An array of MashNodes
        void mash_nodes(const int& key1, const int& key2);
};