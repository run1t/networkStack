#include <netinet/ip_icmp.h>
#include <errno.h>

void make_ICMPheader(struct icmphdr *icmp,u_int8_t typeICMP);
void sendICMP_request(struct icmphdr *ICMP_received,int type_ICMP,uint8_t buf[],int numbytes,struct iphdr *IP_header);
void icmpHandler(struct icmphdr *icmpHeader,uint8_t buf[],int numbytes,struct iphdr *IP_header);
