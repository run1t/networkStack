#include <netinet/ip_icmp.h>
void make_ICMPheader(struct icmphdr *icmp,u_int8_t typeICMP);
void sendICMP_request();
void icmpHandler(struct icmphdr *icmpHeader);
