/*#include <iostream>
#include "SimpleList.h"

using namespace std;

void print(int *num){
    cout << *num << ' ';
}

int main(){
    int i,j;

    SimpleList list;

    for(i=0; i<10; i++){
        if( list.Push(i) !=0 )
            cout << "push error" << endl;
    }

    while( list.Pop(j) !=1 )
        cout << j << ' ';

    cout << "\nTest 1 passed!" << endl;

    for(i=0; i<10; i++){
        if( list.Push(i) !=0 )
            cout << "push error" << endl;
        if( list.Pop(j) !=0 )
            cout << "[pop] error" << endl;
        cout << j << ' ';
    }

    cout << "\nTest 2 passed!" << endl;


    for(i=0; i<2; i++){
        for(j=0; j<5; j++){
            if( list.Push(j) !=0 )
                cout << "push error" << endl;
        }
        if( list.Pop() !=0 )
            cout << "[pop] error" << endl;
    }

    cout << (list.Find(5)? "5 found" : "5 not found /") << ' '
         << (list.Find(2)? "2 found" : "2 not found") << endl;

    list.Traverse(&print);

    while( list.Pop() !=1 ) {}

    list.Traverse(&print);

    for(i=0; i<100; i++){
        if( list.Push(i*69420) !=0 )
            cout << "push error" << endl;
    }

    for(i=0; i<10; i++){
        if( list.Pop() !=0 )
            cout << "pop error" << endl;
    }

    cout << "\nTest 3 passed!" << endl;

    return 0;
}*/