void makeIP_header(struct iphdr *ip,char *data,char datagram[4096],uint32_t destination_ip,int protocol_id);
int sendPacket();
