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
	this->src = "";
	for(int i = 0; i < 6 ; i++){
		std::stringstream stream;
		stream << std::hex << std::uppercase << int (buffer[i]);
		std::string result( stream.str() );
		//on rajoute un 0 si une seul lettre
		if(result.length() == 1){
			result = "0" + result;
		}
		if(i < 5){
			this->src += result + ":";
		}else{
			this->src += result ;
		}

	}

	//Mac dest
	this->dst = "";
	for(int i = 6; i < 12 ; i++){
		std::stringstream stream;
		stream << std::hex << std::uppercase << int (buffer[i]);
		std::string result( stream.str() );
		//on rajoute un 0 si une seul lettre
		if(result.length() == 1){
			result = "0" + result;
		}
		if(i < 11){
			this->dst += result + ":";
		}else{
			this->dst += result ;
		}

	}

	//Type
	this->Type = (buffer[12]<<8)+buffer[13];

}