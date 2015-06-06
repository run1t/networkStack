#include <string>
#include <iostream>
#include <sstream>

using namespace std;

class ETHFrame
{
public:
	//Attributs de ETHFrame
	string src;
	string dst;
	int Type;

	// on analyse la trame et on construit un objet ETHFrame
	ETHFrame(unsigned char* bufferr,int size);

	// on construit un template de trame
	ETHFrame();
};