#include <string>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <functional>
#include "cpp/Stack.h"


using namespace std;


int main()
{
    Stack stack = * new Stack("192.168.1.27",80);
    stack.receiver();
    //trt
}

