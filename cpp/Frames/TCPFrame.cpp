#include "TCPFrame.h"
struct tcp_pseudo /*the tcp pseudo header*/
{
  __u32 src_addr;
  __u32 dst_addr;
  __u8 zero;
  __u8 proto;
  __u16 length;
} pseudohead;


unsigned short TCPFrame::checksum(unsigned short *ptr, unsigned int nbBytes) {
          /* Compute Internet Checksum for "count" bytes
            *         beginning at location "addr".
            */
    register long sum;
	unsigned short oddbyte;
	register short answer;

	sum = 0;
	while(nbBytes>1){
	//on avance dans le pointeur et on incrémente la valeur de sum avec la valeur du pointeur à sa position
		sum+=*ptr++;
	//On décrémente le nb de Bytes de 2 étant donné que le pointeur est un short(donc tient sur 2 bytes, 16 bits)
		nbBytes-=2;
	}
	

	//Si on tombe sur un header impair alors on lui rajoute la donnée nécessaire pour qu'il soit pair
	if(nbBytes==1){
		oddbyte = 0;
		*((u_char *)&oddbyte) = *(u_char*)ptr;
		sum+=oddbyte;
	}

	//On bouge la somme de 16 bits vers la droite en gros divisé par 65536 (16 bits) et on lui ajoute et on ajoute le reste avec le low_order de 16 bits
	sum = (sum>>16)+(sum & 0xffff);
	//On redécalle le sum de 16bits 
	sum = sum + (sum>>16);
	//On fait le complément de la somme avec l'opérateur ~(bitwise NOT), autrement dit on donne l'inverse au niveau des bits 1=0 et 0=1;
	answer=(short)~sum;

	cout << hex << answer << endl;
	//On retourne le checksum
	return(answer);
}

unsigned short TCPFrame::get_tcp_checksum(struct iphdr * myip, struct tcphdr * mytcp) {

        u16 total_len = ntohs(myip->tot_len);
        int tcpopt_len = mytcp->doff*4 - 20;
        int tcpdatalen = total_len - (mytcp->doff*4) - (myip->ihl*4);

        pseudohead.src_addr=myip->saddr;
     
        pseudohead.dst_addr=myip->daddr;
        pseudohead.zero=0;
        pseudohead.proto=IPPROTO_TCP;
        pseudohead.length=htons(sizeof(struct tcphdr) + tcpopt_len + tcpdatalen);

        int totaltcp_len = sizeof(struct tcp_pseudo) + sizeof(struct tcphdr) + tcpopt_len + tcpdatalen;
        unsigned short * tcp = new unsigned short[totaltcp_len];


        memcpy((unsigned char *)tcp,&pseudohead,sizeof(struct tcp_pseudo));
        memcpy((unsigned char *)tcp+sizeof(struct tcp_pseudo),(unsigned char *)mytcp,sizeof(struct tcphdr));
        memcpy((unsigned char *)tcp+sizeof(struct tcp_pseudo)+sizeof(struct tcphdr), (unsigned char *)myip+(myip->ihl*4)+(sizeof(struct tcphdr)), tcpopt_len);
        memcpy((unsigned char *)tcp+sizeof(struct tcp_pseudo)+sizeof(struct tcphdr)+tcpopt_len, (unsigned char *)mytcp+(mytcp->doff*4), tcpdatalen);

        return checksum(tcp,totaltcp_len);

}

unsigned short TCPFrame::get_ip_checksum(struct iphdr * myip) {
        return checksum((unsigned short *)myip,sizeof(iphdr));
}


TCPFrame::TCPFrame(unsigned char* buffer){
	

	/**
	* On recupere les autres couches 
	*/

	this->eth = *new ETHFrame(buffer);
	this->ip = *new IPFrame(buffer);

	/**
	* On recupere TCP
	*/
	int begin = this->ip.HeaderLength+13;

	this->src = buffer[begin+1]*256 + buffer[begin+2];
	this->dst = buffer[begin+3]*256 + buffer[begin+4];
	this->seq_number = (buffer[begin+5] << 24) | (buffer[begin+6] << 16) | (buffer[begin+7] << 8) | buffer[begin+8];
	this->ack_number = (buffer[begin+9] << 24) | (buffer[begin+10] << 16) | (buffer[begin+11] << 8) | buffer[begin+12];
	this->HeaderLength = (buffer[begin+13] >> 4)*4;
	this->Flags = buffer[begin+14];
	this->Windows = buffer[begin+15]*256 + buffer[begin+16];
	this->Checksum = buffer[begin+17]*256 + buffer[begin+18];
	this->urgentPointer = buffer[begin+19]*256 + buffer[begin+20];
	for(int i = 21+13+this->ip.HeaderLength; i < this->HeaderLength+14+this->ip.HeaderLength; i++){
		this->options.push_back(buffer[i]);
	}
	//On recupere le message TCP
	this->data = "";
	for(int i = this->ip.HeaderLength+this->HeaderLength+14 ; i <= this->ip.TotalLength+13 ; i++){
		this->data += buffer[i];
	}
}

TCPFrame::TCPFrame(){

}

 unsigned char* TCPFrame::toFrame(){
	vector<unsigned char> frame;


	/**
	* Remplissage du Header Ethernet
	*/

	// Addresse Destination
	std::stringstream ss;
	unsigned int buffer;
	int offset = 0;
	for(size_t i = 0 ; i < this->eth.dst.length() ; i += 3){
		if(this->eth.dst[i] != ':'){
			ss.clear();
			ss << std::hex << this->eth.dst.substr(offset, 2);
			ss >> buffer;
			frame.push_back(static_cast<unsigned char>(buffer));
			offset += 3;
		}
		
	}

	// Address Source
	offset = 0;
	for(int i = 0 ; i < this->eth.src.length() ; i += 3){
		if(this->eth.src[i] != ':'){
			ss.clear();
			ss << std::hex << this->eth.src.substr(offset, 2);
			ss >> buffer;
			frame.push_back(static_cast<unsigned char>(buffer));
			offset += 3;
		}
		
	}

	//Ether Type
	frame.push_back((this->eth.Type >> 8) & 0xFF);
	frame.push_back((this->eth.Type) & 0xFF);
	
	/**
	* Remplissage du header Ip
	*/

	frame.push_back((this->ip.Version << 4) | (this->ip.HeaderLength/4));
	
	//DFS
	frame.push_back(0x00);

	//on recalcule la taille total
	int TOTAL = this->ip.HeaderLength + this->HeaderLength + this->data.length();
	frame.push_back((TOTAL >> 8) & 0xFF);
	frame.push_back((TOTAL) & 0xFF);

	frame.push_back((this->ip.Id >> 8) & 0xFF);
	frame.push_back((this->ip.Id) & 0xFF);

	
	//fragment offset
	frame.push_back(this->ip.Flags);
	frame.push_back(this->ip.PositionFragment);

	frame.push_back(this->ip.TTL);
	frame.push_back(this->ip.Protocol);

	frame.push_back(0x00);
	frame.push_back(0x00);

	//ip source 
	int byte1, byte2, byte3, byte4;
	char dot;
	
	istringstream s(this->ip.src);  // input stream that now contains the ip address string
	s >> byte1 >> dot >> byte2 >> dot >> byte3 >> dot >> byte4 >> dot;

	frame.push_back(byte1);
	frame.push_back(byte2);
	frame.push_back(byte3);
	frame.push_back(byte4);

	istringstream s1(this->ip.dst);  // input stream that now contains the ip address string
	s1 >> byte1 >> dot >> byte2 >> dot >> byte3 >> dot >> byte4 >> dot;

	frame.push_back(byte1);
	frame.push_back(byte2);
	frame.push_back(byte3);
	frame.push_back(byte4);

	for(int i = 0 ; i < this->ip.options.size() ; i++){
		frame.push_back(this->ip.options.at(i));
	}

	/*
	* Remplissage de la trame TCP
	**/

	//les ports
	frame.push_back((this->dst >> 8) & 0xFF);
	frame.push_back((this->dst) & 0xFF);

	frame.push_back((this->src >> 8) & 0xFF);
	frame.push_back((this->src) & 0xFF);

	frame.push_back((this->seq_number >> 24) & 0xFF);
	frame.push_back((this->seq_number >> 16) & 0xFF);
	frame.push_back((this->seq_number >> 8) & 0xFF);
	frame.push_back((this->seq_number) & 0xFF);

	frame.push_back((this->ack_number >> 24) & 0xFF);
	frame.push_back((this->ack_number >> 16) & 0xFF);
	frame.push_back((this->ack_number >> 8) & 0xFF);
	frame.push_back((this->ack_number) & 0xFF);

	frame.push_back((this->HeaderLength/4) << 4);
	frame.push_back(this->Flags);
	frame.push_back((this->Windows >> 8) & 0xFF);
	frame.push_back((this->Windows) & 0xFF);

	//on remet le checksum a Zero
	frame.push_back(0x00);
	frame.push_back(0x00);

	frame.push_back((this->urgentPointer >> 8) & 0xFF);
	frame.push_back((this->urgentPointer) & 0xFF);

	for(int i = 0 ; i < this->options.size() ; i++){
		frame.push_back(this->options.at(i));
	}

	for(int i = 0 ; i < this->data.length() ; i++){
		frame.push_back(this->data[i]);
	}

	this->data[4] = this->data[4] + 20;

	this->frameLength = frame.size();

	unsigned char* ret = (unsigned char*) malloc(frame.size()*sizeof(unsigned char*));
	for(int i = 0; i < frame.size() ; i++){
		ret[i] = frame.at(i);
	}
	
	/**
	* On va recupere la position du checksum dans la tram
	*/
	int position = this->ip.HeaderLength + 13 + 17; 
	unsigned short check = this->get_tcp_checksum((struct iphdr*)(ret + sizeof(ethhdr)),(struct tcphdr*)(ret + sizeof(ethhdr) + sizeof(iphdr)));
	u16 checkTCP = htons(check);
	frame[position] = (checkTCP >> 8) & 0xFF;
	frame[position+1] = (checkTCP)  & 0xFF;

	u16 checkIP = htons(this->get_ip_checksum((struct iphdr*)(ret + sizeof(ethhdr))));
	int pos = 13+11;
	frame[pos] = (checkIP >> 8) & 0xFF;
	frame[pos+1] = (checkIP)  & 0xFF;
	cout << "checksum IP" << endl;
	cout << hex << checkIP << endl;
	cout << "checksum IP" << endl;
	for(int i = 0; i < frame.size() ; i++){
		ret[i] = frame.at(i);
	}
	
	return ret;
}