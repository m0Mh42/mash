#include "../inc/header.hpp"

using namespace std;

class Mash {
    public:
        string mash(string input);
        void reset_seed(void);
    private:
        int s1, s2;
};

string Mash::mash(string input) {
    long len = input.length();
    string* ret;
    ret = new string;
    *ret = "";
    for (int i = 0; i < len; i++){

        if (debug){
            bitset<8> x(input[i]);
            bitset<8> y(input[len - (i + 1)]);
            cout << i << " " << x << " | " << y << endl;
        }

        unsigned char ored = input[i] | input[len - (i + 1)];
        ored ^= s1;
        s2 <<= 1;
        s1 ^= s2;
        *ret += ored;
        s1++;
    }

    return *ret;
}

void Mash::reset_seed(void){
    s1 = 0;
    s2 = 1;
}
