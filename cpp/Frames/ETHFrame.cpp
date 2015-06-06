#include "ETHFrame.h"

ETHFrame::ETHFrame(){

}

ETHFrame::ETHFrame(unsigned char* buffer,int size){
	/**
	* Trame ethernet
	* 6 bytes mac source
	* 6 bytes mac destination
	* 1 byte Type
	*/

	//Mac source
	this->dst = "";
	for(int i = 0; i < 6 ; i++){
		std::stringstream stream;
		stream << std::hex << std::uppercase << int (buffer[i]);
		std::string result( stream.str() );
		//on rajoute un 0 si une seul lettre
		if(result.length() == 1){
			result = "0" + result;
		}
		if(i < 5){
			this->dst += result + ":";
		}else{
			this->dst += result ;
		}

	}

	//Mac dest
	this->src = "";
	for(int i = 6; i < 12 ; i++){
		std::stringstream stream;
		stream << std::hex << std::uppercase << int (buffer[i]);
		std::string result( stream.str() );
		//on rajoute un 0 si une seul lettre
		if(result.length() == 1){
			result = "0" + result;
		}
		if(i < 11){
			this->src += result + ":";
		}else{
			this->src += result ;
		}

	}

	//Type
	this->Type = (buffer[12]<<8)+buffer[13];

}