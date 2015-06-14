/**
 * \file SendFrame.cpp
 * \brief Fichier contenant les fonctions liées à l'envoi
 * \author LE NOC Reunan, VIAUD Thomas, HIPEAU Kevin, TRICHARD Guillaume
 * \version 0.1
 *
 */
#include "SendFrame.h"

/**
 * \fn SendFrame::SendFrame(TCPFrame tcp)
 * \brief Constructeur par défaut de la classe SendFrame
 *
 * \param tcp trame TCP à envoyer
 */
SendFrame::SendFrame(TCPFrame tcp){
	this->first = true;
	this->retransmission = -1;
	this->lastSend = time(0);
	this->tcp = tcp;
}

/**
 * \fn bool SendFrame::canRetransmit()
 * \brief Permet de savoir si on doit retransmettre ou non une trame
 *
 *\return Un booléen pour savoir si oui ou non on peut retransmettre
 */
bool SendFrame::canRetransmit(){	
	cout << " ok " << this->lastSend << "  :  " << time(0)  << "  ok " << endl;
	return this->lastSend - time(0) > 3;
}

/**
 * \fn bool SendFrame::retransmissionLimit()
 * \brief Permet de savoir si le nombre de retransmission a été atteint
 *
 *\return Un booléen pour savoir si oui ou non on peut encore retransmettre
 */
bool SendFrame::retransmissionLimit(){
	return this->retransmission <= 5;
}

/**
 * \fn void SendFrame::timeCalc()
 * \brief Permet d'avoir le temps lié à l'envoi de la trame
 */
void SendFrame::timeCalc(){
	this->lastSend = time(0);
}
