#include "tcp.h"
#include "ip.h"
#include "ethernet.h"

int main(int argc, char *argv[])
{	
	printf("IP eth0: %s\n",inet_ntoa(getIp("eth0")));
	//tcp_sniffer();
	getMac_address("eth0");
	int result = 0;
	result = sendPacket();
	if(result == 1)
	{
		printf("error");
	}
	return 0;
}
