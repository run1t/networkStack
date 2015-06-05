#include <string>
extern "C" {    // another way
	#include "../../c/Server.h"
   };
using namespace std;
class TCPFrame
{
public:
	int s_port;
	int d_port;
	int seq_number;
	int ack_number;
	int data_offset;
	int flags;
	int windows;
	int checksum;
	int urgentPointer;
	string data;

	TCPFrame(char* frame);
	TCPFrame();
	~TCPFrame();	
};

