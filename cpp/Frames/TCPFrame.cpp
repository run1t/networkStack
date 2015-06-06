#include "TCPFrame.h"

void showFrame(vector<unsigned char> v){
	cout << "On a une trame de " << v.size() << endl;
	for(int i = 0; i < v.size() ; i++){
		std::stringstream stream;
		stream << std::hex << std::uppercase << int (v.at(i));
		std::string result( stream.str() );
		cout << result << " ";
	}
	cout << endl;
}

TCPFrame::TCPFrame(unsigned char* buffer,int size){
	
	/**
	* On recupere les autres couches 
	*/

	this->eth = *new ETHFrame(buffer,size);
	this->ip = *new IPFrame(buffer,size);

	/**
	* On recupere TCP
	*/
	int begin = this->ip.HeaderLength+13;

	this->src = buffer[begin+1]*256 + buffer[begin+2];
	this->dst = buffer[begin+3]*256 + buffer[begin+4];
	this->seq_number = (buffer[begin+5] << 24) | (buffer[begin+6] << 16) | (buffer[begin+7] << 8) | buffer[begin+8];
	this->ack_number = (buffer[begin+9] << 24) | (buffer[begin+10] << 16) | (buffer[begin+11] << 8) | buffer[begin+12];
	this->HeaderLength = (buffer[begin+13] >> 4)*4;
	this->Flags = buffer[begin+14];
	this->Windows = buffer[begin+15]*256 + buffer[begin+16];
	this->Checksum = buffer[begin+17]*256 + buffer[begin+18];
	this->urgentPointer = buffer[begin+19]*256 + buffer[begin+20];

	//On recupere le message TCP
	this->data = "";
	for(int i = this->ip.HeaderLength+this->HeaderLength+14 ; i <= this->ip.TotalLength+13 ; i++){
		this->data += buffer[i];
	}
}

TCPFrame::TCPFrame(){

}

 unsigned char* TCPFrame::toFrame(){
	vector<unsigned char> frame;


	/**
	* Remplissage du Header Ethernet
	*/

	// Addresse Destination
	std::stringstream ss;
	unsigned int buffer;
	int offset = 0;
	for(int i = 0 ; i < this->eth.dst.length() ; i += 3){
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
	for(int i = 0 ; i < this->eth.src.length() ; i += 3){
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
	* Remplissage du header Ip
	*/

	frame.push_back((this->ip.Version << 4) | (this->ip.HeaderLength/4));
	
	//DFS
	frame.push_back(0x00);

	frame.push_back((this->ip.TotalLength >> 8) & 0xFF);
	frame.push_back((this->ip.TotalLength) & 0xFF);

	frame.push_back((this->ip.Id >> 8) & 0xFF);
	frame.push_back((this->ip.Id) & 0xFF);

	
	//fragment offset
	frame.push_back(this->ip.Flags);
	frame.push_back(this->ip.PositionFragment);

	frame.push_back(this->ip.TTL);
	frame.push_back(this->ip.Protocol);
	frame.push_back((this->ip.Checksum >> 8) & 0xFF);
	frame.push_back((this->ip.Checksum) & 0xFF);

	//ip source 
	int byte1, byte2, byte3, byte4;
	char dot;
	
	istringstream s(this->ip.src);  // input stream that now contains the ip address string
	s >> byte1 >> dot >> byte2 >> dot >> byte3 >> dot >> byte4 >> dot;

	frame.push_back(byte1);
	frame.push_back(byte2);
	frame.push_back(byte3);
	frame.push_back(byte4);

	istringstream s1(this->ip.dst);  // input stream that now contains the ip address string
	s1 >> byte1 >> dot >> byte2 >> dot >> byte3 >> dot >> byte4 >> dot;

	frame.push_back(byte1);
	frame.push_back(byte2);
	frame.push_back(byte3);
	frame.push_back(byte4);

	while(frame.size() < this->ip.TotalLength){
		frame.push_back(0x00);
	}
	this->frameLength = frame.size();
	unsigned char* ret = (unsigned char*) malloc(frame.size()*sizeof(unsigned char*));
	for(int i = 0; i < frame.size() ; i++){
		ret[i] = frame.at(i);
	}
	showFrame(frame);
	return ret;
}