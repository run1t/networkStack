#include "Stack.h"

/**
* Gestiond de la reception 
* des données
*/

void stack(){

	uint8_t buf[BUF_SIZ];
	int numbytes;
	struct ethhdr *etherHeader;
	struct iphdr *ip_hdr;
	struct tcphdr *tcp_hdr;
	struct icmphdr *icmp_hdr;
	struct responseStack Stack;
	
	while(1){

		numbytes = recvfrom(sock, buf, BUF_SIZ, 0, NULL, NULL);
			
		//On verifie que l'on a bien des données
		if(numbytes > 0){

		//On recupère le header
		etherHeader = (struct ethhdr *)buf;

		//Quelle est le type de ethernet ?
				
		if(etherHeader->h_proto == ETH_P_ARP){
			printf(" Arp\n");
		}else if(etherHeader->h_proto == htons(0x0800)){
			//On check si on la bonne address mac
			if(MacIsForMe(etherHeader)){
				//Recuperation du header ip
				ip_hdr = (struct iphdr *)(buf + sizeof(struct ethhdr));
				
				//On ne traite que l'ip V4

				if(ip_hdr->version == 4){
					//On check si c'est la bonne address IP
					if(IpIsForMe(ip_hdr)){
						switch(ip_hdr->protocol){
							//#ICMP
							case ICMP_PROTO:

								icmp_hdr = (struct icmphdr *)(buf +sizeof(struct ethhdr) + sizeof(struct iphdr));
								icmpHandler(icmp_hdr,buf,numbytes,ip_hdr);
							break;

							//#TCP
							case TCP_PROTO:
								printf("TCP\n");
								tcp_hdr = (struct tcphdr *)(buf + sizeof(struct ethhdr) + sizeof(struct iphdr));
								Stack.Type = 1;
								Stack.Tcp = tcpHandler(buf,tcp_hdr,sock,numbytes,ip_hdr);
								return Stack;
							break;

							}
						}
					}
			    }
		    }
	}
}

Stack::Stack(){
	
};

void Stack::receiver(){
    thread t1(stack,this);
    t1.join();
}
