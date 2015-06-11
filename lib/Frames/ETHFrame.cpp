#include "ETHFrame.h"
/**
 * \file ETHFrame.cpp
 * \brief fichier qui permet de lire les trames ethernets bruts
 * \author LE NOC Reunan, VIAUD Thomas, HIPEAU Kevin, TRICHARD Guillaume
 * \version 0.1
 *
 */
ETHFrame::ETHFrame(){
	this->src = PC::getMAC();
	this->dst = "FF:FF:FF:FF:FF:FF";
	this->Type = 0x0800;
}
/**
 * \fn ETHFrame::ETHFrame(unsigned char* buffer)
 * \brief Fonction de lecture de la trame ethernet
 *
 * \param la fonction prend en parametres le buffer brut de la trame
 * \return la fonction retourne la trame une fois analysé et parsé
 */
ETHFrame::ETHFrame(unsigned char* buffer){
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
		//On converti en hexadecimal
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
		//On converti en hexadecimal
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
