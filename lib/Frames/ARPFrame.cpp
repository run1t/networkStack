/**
 * \file ARPFrame.cpp
 * \brief fichier qui permet de lire les trames ARP bruts ainsi que de les créer  
 * \author LE NOC Reunan, VIAUD Thomas, HIPEAU Kevin, TRICHARD Guillaume
 * \version 0.1
 *
 */
#include "ARPFrame.h"

ARPFrame::ARPFrame(){

}
/**
 * \fn ARPFrame::ARPFrame(unsigned char* buffer)
 * \brief Fonction de lecture de la trame ARP
 *
 * \param unsigned char* buffer la fonction prend en parametres le buffer brut de la trame
 * \return la fonction retourne la trame une fois analysé et parsé
 */
ARPFrame::ARPFrame(unsigned char* buffer){

	this->eth = *new ETHFrame(buffer);
	/**
	* On commence le remplissage de la trame IP juste après 
	* le header Ethernet au byte 14
	*/

	this->HardwareType = buffer[14]*256+buffer[15];
	this->ProtocolIp = buffer[16]*256+buffer[17];
	this->HardwareSize = buffer[18];
	this->ProtocolSize = buffer[19];
	this->opCode = buffer[20]*256+buffer[21];
	this->senderMac = "";
	for(int i = 22; i < 28 ; i++){
		std::stringstream stream;
		//On converti en hexadecimal
		stream << std::hex << std::uppercase << int (buffer[i]);
		std::string result( stream.str() );
		//on rajoute un 0 si une seul lettre
		if(result.length() == 1){
			result = "0" + result;
		}
		if(i < 27){
			this->senderMac += result + ":";
		}else{
			this->senderMac += result ;
		}

	}
	//string senderIp;
	//IP src
	this->senderIp = "";
	for(int i = 28; i < 32 ; i++){
		std::stringstream stream;
		stream << std::dec << int (buffer[i]);
		std::string result( stream.str() );
		
		if(i < 31){
			this->senderIp += result + ".";
		}else{
			this->senderIp += result ;
		}

	}

	this->targetMac = "";
	for(int i = 33; i < 39 ; i++){
		std::stringstream stream;
		//On converti en hexadecimal
		stream << std::hex << std::uppercase << int (buffer[i]);
		std::string result( stream.str() );
		//on rajoute un 0 si une seul lettre
		if(result.length() == 1){
			result = "0" + result;
		}
		if(i < 38){
			this->targetMac += result + ":";
		}else{
			this->targetMac += result ;
		}

	}
	//string senderIp;
	//IP src
	this->targetIp = "";
	for(int i = 38; i < 42; i++){
		std::stringstream stream;
		stream << std::dec << int (buffer[i]);
		std::string result( stream.str() );
		
		if(i < 41){
			this->targetIp += result + ".";
		}else{
			this->targetIp += result ;
		}

	}
}
/**
 * \fn unsigned char* ARPFrame::toFrame()
 * \brief Fonction création de la trame ARP pour l'envoie 
 *
 * \return la fonction retourne la trame sous forme de buffer
 */
unsigned char* ARPFrame::toFrame(){
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
	* Remplissage de ARP
	*/

	frame.push_back((this->HardwareType >> 8) & 0xFF);
	frame.push_back((this->HardwareType) & 0xFF);

	frame.push_back((this->ProtocolIp >> 8) & 0xFF);
	frame.push_back((this->ProtocolIp) & 0xFF);

	frame.push_back(this->HardwareSize);

	frame.push_back(this->ProtocolSize);

	frame.push_back((this->opCode >> 8) & 0xFF);
	frame.push_back((this->opCode) & 0xFF);

	offset = 0;
	for(size_t i = 0 ; i < this->senderMac.length() ; i += 3){
		if(this->eth.src[i] != ':'){
			ss.clear();
			ss << std::hex << this->senderMac.substr(offset, 2);
			ss >> buffer;
			frame.push_back(static_cast<unsigned char>(buffer));
			offset += 3;
		}
	}

	//ip source 
	int byte1, byte2, byte3, byte4;
	char dot;
	
	istringstream s(this->senderIp);  // input stream that now contains the ip address string
	s >> byte1 >> dot >> byte2 >> dot >> byte3 >> dot >> byte4 >> dot;
	frame.push_back(byte1);
	frame.push_back(byte2);
	frame.push_back(byte3);
	frame.push_back(byte4);

	offset = 0;
	for(size_t i = 0 ; i < this->targetMac.length() ; i += 3){
		if(this->eth.src[i] != ':'){
			ss.clear();
			ss << std::hex << this->targetMac.substr(offset, 2);
			ss >> buffer;
			frame.push_back(static_cast<unsigned char>(buffer));
			offset += 3;
		}
	}

	
	
	istringstream s1(this->targetIp);  // input stream that now contains the ip address string
	s1 >> byte1 >> dot >> byte2 >> dot >> byte3 >> dot >> byte4 >> dot;
	frame.push_back(byte1);
	frame.push_back(byte2);
	frame.push_back(byte3);
	frame.push_back(byte4);

	this->frameLength = frame.size();

	unsigned char* ret = (unsigned char*) malloc(frame.size()*sizeof(unsigned char*));
	for(size_t i = 0; i < frame.size() ; i++){
		ret[i] = frame.at(i);
	}
	
	return ret;
}