/**
 * \file ETHFrame.h
 * \brief fichier qui permet de lire les trames ethernets bruts
 * \author LE NOC Reunan, VIAUD Thomas, HIPEAU Kevin, TRICHARD Guillaume
 * \version 0.1
 *
 */
#include <string>
#include <iostream>
#include <sstream>
#include "../PC.h"
#if !defined( ETHFRAME_H )
#define ETHFRAME_H
using namespace std;

class ETHFrame
{
public:
	//Attributs de ETHFrame
	string src;
	string dst;
	int Type;

	// on analyse la trame et on construit un objet ETHFrame
	ETHFrame(unsigned char* bufferr);

	// on construit un template de trame
	ETHFrame();
};
#endif // !defined( ETHFRAME_H )