#include <iostream>

using namespace std;

template <typename T>
bool memcheck(T var){
    if (var == NULL){
        return false;
    }
    return true;
}
