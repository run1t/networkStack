/**
 * \file ICMPFrame.cpp
 * \brief fichier qui permet de lire les trames ICMP bruts
 * \author LE NOC Reunan, VIAUD Thomas, HIPEAU Kevin, TRICHARD Guillaume
 * \version 0.1
 *
 */
#include "ICMPFrame.h"

/**
 * \fn unsigned short ICMPFrame::checksum(unsigned short *ptr, unsigned int nbBytes)
 * \brief Fonction de réalisation d'un checksum pour l'intégrité des données 
 *
 * \param unsigned short *ptr la fonction prend en parametres la structure sur laquelle réaliser le checksum
 * \param unsigned int nbBytes la fonction prend en parametres la taille 
 * \return la fonction retourne le checksum
 */
unsigned short ICMPFrame::checksum(unsigned short *ptr, unsigned int nbBytes) {
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

	//On retourne le checksum
	return(answer);
}

/**
 * \fn unsigned short ICMPFrame::get_ip_checksum(struct iphdr * myip)
 * \brief Fonction de réalisation d'un checksum pour l'IP
 *
 * \param struct iphdr * myip la fonction prend en parametres la structure sur laquelle réaliser le checksum
 * \return la fonction retourne le checksum
 */
unsigned short ICMPFrame::get_ip_checksum(struct iphdr * myip) {
        return checksum((unsigned short *)myip,sizeof(iphdr));
}
/**
 * \fn unsigned short ICMPFrame::get_icmp_checksum(struct icmphdr * myicmp,int length)
 * \brief Fonction de réalisation d'un checksum ICMP
 *
 * \param struct icmphdr * myicmp la fonction prend en parametres la structure sur laquelle réaliser le checksum 
  * \param int length la fonction prend en parametres la taille de la structure
 * \return la fonction retourne le checksum
 */
unsigned short ICMPFrame::get_icmp_checksum(struct icmphdr * myicmp,int length) {
        return checksum((unsigned short *)myicmp,length);
}

/**
 * \fn ICMPFrame::ICMPFrame(unsigned char* buffer)
 * \brief Fonction de lecture d'un buffer brut
 *
 * \param unsigned char* buffer la fonction prend en parametres le buffer afin de l'analyser
 * \return la fonction retourne la trame analysé 
 */
ICMPFrame::ICMPFrame(unsigned char* buffer){
	

	/**
	* On recupere les autres couches 
	*/

	this->eth = *new ETHFrame(buffer);
	this->ip = *new IPFrame(buffer);

	/**
	* On recupere TCP
	*/
	int begin = this->ip.HeaderLength+13;

	this->Type     = buffer[begin+1];
	this->Code     = buffer[begin+2];
	this->Checksum = buffer[begin+3]*256 + buffer[begin+4];
	this->Id       = buffer[begin+5]*256 + buffer[begin+6];
	this->sequence = buffer[begin+7]*256 + buffer[begin+8];
	this->data = "";
	for(int i = begin+9 ; i < this->ip.TotalLength+14; i++){
		this->data += buffer[i];
	}

}

/**
 * \fn ICMPFrame::ICMPFrame()
 * \brief Constructeur par défaut d'un paquet ICMP
 *
 * \return la fonction retourne le paquet ICMP par défaut
 */
ICMPFrame::ICMPFrame(){
	/**
	* On recupere les autres couches 
	*/

	this->eth = *new ETHFrame();
	this->ip = *new IPFrame();

	/**
	* On recupere TCP
	*/
	this->Type     = 8;
	this->Code     = 0;
	this->Checksum = 0;
	this->Id       = rand();
	this->sequence = 8448;
	this->data = "123456789AZERTYUIOP";
	
}

/**
 * \fn unsigned char* ICMPFrame::toFrame()
 * \brief Fonction de création de la trame brut a partir des parametres 
 *
 * \return la fonction retourne la trame brut
 */
unsigned char* ICMPFrame::toFrame(){
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
	for(size_t i = 0 ; i < this->eth.src.length() ; i += 3){
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
	int TOTAL = this->ip.HeaderLength  + 8 + this->data.length();
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

	for(size_t i = 0 ; i < this->ip.options.size() ; i++){
		frame.push_back(this->ip.options.at(i));
	}

	/**
	* On rempli le packet ICMP
	*/
	frame.push_back(this->Type);
	frame.push_back(this->Code);
	frame.push_back(0x00);
	frame.push_back(0x00);

	frame.push_back((this->Id >> 8) & 0xFF);
	frame.push_back((this->Id) & 0xFF);

	frame.push_back((this->sequence>> 8) & 0xFF);
	frame.push_back((this->sequence) & 0xFF);
	
	for(size_t i = 0 ; i < this->data.length() ; i++){
		frame.push_back(this->data[i]);
	}

	this->frameLength = frame.size();

	

	unsigned char* ret = (unsigned char*) malloc(frame.size()*sizeof(unsigned char*));
	for(size_t i = 0; i < frame.size() ; i++){
		ret[i] = frame.at(i);
	}
	
	u16 checkIP = htons(this->get_ip_checksum((struct iphdr*)(ret + sizeof(ethhdr))));
	int pos = 13+11;
	frame[pos] = (checkIP >> 8) & 0xFF;
	frame[pos+1] = (checkIP)  & 0xFF;
	
	/**
	* On va recuperer la position du checksum dans la trame
	*/
	int position = this->ip.HeaderLength + 13 + 3; 
	unsigned short check = this->get_icmp_checksum((struct icmphdr*)(ret + sizeof(ethhdr) + sizeof(iphdr)),8 + this->data.length());
	u16 checkTCP = htons(check);
	frame[position] = (checkTCP >> 8) & 0xFF;
	frame[position+1] = (checkTCP)  & 0xFF;


	for(size_t i = 0; i < frame.size() ; i++){
		ret[i] = frame.at(i);
	}

	for(size_t i = 0; i < frame.size() ; i++){
		std::stringstream stream;
		stream << std::hex << std::uppercase << int (frame.at(i));
		std::string result( stream.str() );
	}
	return ret;
}
