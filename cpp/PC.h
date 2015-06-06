#include <string>
#include <sstream>
using namespace std;
extern "C" {    // another way
	#include <net/if.h>
	#include <errno.h>
	#include <sys/ioctl.h>
	#include <string.h>
	#include <unistd.h>
	#include <netinet/in.h>
	#include <sys/socket.h>
	#include <arpa/inet.h>
};
#if !defined( PC_H )
#define PC_H

class PC
{
public:
	static string getIP();
	static string getMAC();
};

#endif // !defined( PC_H )