#include <string>
#include <sstream>
#include <iostream>
using namespace std;
extern "C" {    // another way
	#include <net/if.h>
	#include <errno.h>
	#include <sys/ioctl.h>
	#include <string.h>
	#include <unistd.h>
	#include <stdio.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <fcntl.h>
	#include <netinet/in.h>
	#include <sys/socket.h>
	#include <arpa/inet.h>
};
#if !defined( PC_H )
#define PC_H

class PC
{
public:
	PC();
	static string getIP();
	static string getMAC();
	static string getDefaultInterface();
};

#endif // !defined( PC_H )
