#include "tcp.h"

int main(int argc, char *argv[])
{	
	printf("IP eth0: %s\n",inet_ntoa(getIp("eth0")));
}
