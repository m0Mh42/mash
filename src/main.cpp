#include <fstream>
#include "header.hpp"
#include "hash.cpp"
#include "memcheck.cpp"
#include "base64.hpp"
#include "file.cpp"
#include <cmath>

using namespace std;

int main(int argc, char* argv[]) {
    string filename = argv[1];
    string* data = new string[1024];
    if (data == nullptr){
        exit(1);
    }

    data = Readfile(filename);

    Mash* mash;
    mash = new Mash;
    if (mash == nullptr){
        exit(1);
    }

    int line_c = GetLineLen();
    cout << line_c << endl;

    string* output = new string[line_c + 1];
    if (output == nullptr){
        exit(1);
    }

    mash -> reset_seed();

    if (testing){
        string test = "";
        for (int i = 0; i < pow(256.00, 4.00); i++){
            test += "1";
            *(output + i) = mash -> mash(*(data + i));
            cout << *(output + i) << endl;
        }
    }

    for (int i = 0; i < line_c; i++){
        *(output + i) = mash -> mash(*(data + i));
    }

    macaron::Base64* b64c;
    b64c = new macaron::Base64;
    
    string* b64 = new string[line_c + 1];
    for (int i = 0; i < line_c; i++){
        *(b64 + i) = b64c -> Encode(*(output + i));
        cout << *(b64 + i) << endl;
    }

    delete mash;
    delete[] data;
    delete[] output;
    delete b64c;
    delete[] b64;
    
    return 0;
}
