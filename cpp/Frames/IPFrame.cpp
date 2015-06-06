
#include "IPFrame.h"

IPFrame::IPFrame(){

}

IPFrame::IPFrame(unsigned char* buffer,int size){

	this->eth = *new ETHFrame(buffer,size);
	/**
	* On commence le remplissage de la trame IP juste après 
	* le header Ethernet au byte 14
	*/

	// Recuperation de la version IP
	this->Version = buffer[14] >> 4;
	this->HeaderLength = (buffer[14] & 0x0F)*32/8;


	this->DFS = 0;
	this->TotalLength = (buffer[16]*256+buffer[17]);
	this->Id = (buffer[18]*256+buffer[19]);

	// Informations sur la fragmentation
	this->Flag_Reserved = buffer[20] >> 7 & 1;
	this->Flag_DF = buffer[20] >> 6 & 1;
	this->Flag_MF = buffer[20] >> 5 & 1;
	this->Flags = buffer[20];
	this->PositionFragment = buffer[21];
	this->TTL = buffer[22];
	this->Protocol = buffer[23];
	this->Checksum = (buffer[24]*256+buffer[25]);
	//IP src
	this->src = "";
	for(int i = 26; i < 30 ; i++){
		std::stringstream stream;
		stream << std::dec << int (buffer[i]);
		std::string result( stream.str() );
		
		if(i < 29){
			this->src += result + ".";
		}else{
			this->src += result ;
		}

	}

	//IP dst
	this->dst = "";
	for(int i = 30; i < 34 ; i++){
		std::stringstream stream;
		stream << std::dec << int (buffer[i]);
		std::string result( stream.str() );
		
		if(i < 33){
			this->dst += result + ".";
		}else{
			this->dst += result ;
		}

	}

}