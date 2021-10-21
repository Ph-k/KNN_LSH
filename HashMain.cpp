#include <iostream>
#include "HashTable.h"

using namespace std;

int hashfun(int fun){
    return fun;
}

void print(int *num){
    cout << *num << ' ';
}

int main(){
    HashTable ht(10,&hashfun);

    for(int i=5; i<16; i++){
        ht.Insert(i);
    }

    ht.Traverse(print);
}