#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "ETHFrame.h"
#if !defined( IPFRAME_H )
#define IPFRAME_H
using namespace std;

class IPFrame
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
	unsigned int Version : 4;
	int HeaderLength;
	int DFS;
	int TotalLength;
	vector<unsigned char> options;
	int Id;
	int Flags;
	bool Flag_Reserved;
	bool Flag_DF;
	bool Flag_MF;
	int PositionFragment;
	int TTL;
	int Protocol;
	int Checksum;
	string src;
	string dst;


	// on analyse la trame et on construit un objet ETHFrame
	IPFrame(unsigned char* bufferr);

	// on construit un template de trame
	IPFrame();
};

#endif // !defined( IPFRAME_H )