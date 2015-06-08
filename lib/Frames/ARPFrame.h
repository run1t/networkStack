/**
 * \file ARPFrame.h
 * \brief fichier qui permet de lire les trames ARP bruts ainsi que de les créer  
 * \author LE NOC Reunan, VIAUD Thomas, HIPEAU Kevin
 * \version 0.1
 *
 */
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "ETHFrame.h"
#if !defined( ARPFrame_H )
#define ARPFrame_H
using namespace std;

class ARPFrame
{
	/**
	* Trame IP
	* 4 bits Version
	* 4 bits HeaderLength
	* 1 bytes DFS
	* 2 bytes TotalLength
	* 2 bytes ID
	* 3 bits Flags
	*	-> 1 Reserved
	* 	-> 1 DF
	*   -> 1 MF
	* 13 bits Fragments Offset
	* 1 byte TTL
	* 1 byte Protocol
	* 2 bytes Checksum
	* 4 bytes src
	* 4 bytes dst
	*/
public:
	
	ETHFrame eth;
	int HardwareType;
	int ProtocolIp;
	int HardwareSize;
	int ProtocolSize;
	int opCode;
	string senderMac;
	string senderIp;
	string targetMac;
	string targetIp;
	int frameLength;
	// on analyse la trame et on construit un objet ETHFrame
	ARPFrame(unsigned char* bufferr);
	unsigned char* toFrame();
	// on construit un template de trame
	ARPFrame();
};

#endif // !defined( ARPFrame_H )