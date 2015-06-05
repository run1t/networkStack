#define SYNC_WAIT 0
#define SYNC_ACK_WAIT 1



class stackClient{
	public:
		int OPEN;
		int state;
		stackClient();
		void handleStack();

};