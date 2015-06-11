#include <string>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <functional>
#include "lib/Stack.h"


using namespace std;

void onSyn(){
    cout << "on a eu un Syn" << endl;
}

void onData(string data){
    cout << "on a de la data " << endl;
    cout << "Data : " << data << endl;
}

void onFin(){
    cout << "on a eu un Fin" << endl;
}

int main()
{

    Stack stack = * new Stack("192.168.1.27",80);
    stack.addSynEvent (onSyn);
    stack.addDataEvent(onData);
    stack.addFinEvent (onFin);
    stack.receiver();

    cout << " ok ok " << endl;
    
}

