#ifndef MYTCPCLIENT_H
#define MYTCPCLIENT_H

#include "Mythread.h"
#include "PracticalSocket.h"
#include <unistd.h>

class MyTcpClient : public MyThread
{
	public:
	MyTcpClient();
	MyTcpClient(TCPSocket *sock);
	~MyTcpClient();
	
	//PUBLIC MEMBERS
	TCPSocket* m_clntSock;
		
	int m_status;
	static const int m_RCVBUFSIZE =4;
	unsigned char m_rcvBuffer[m_RCVBUFSIZE];
	static const int m_SENDBUFSIZE = 4;
	unsigned char m_sendBuffer[m_SENDBUFSIZE];
	
	
	//PUBLIC METHODS
		
		void InternalThreadEntry();	
		void HandleTCPClient();
		void clearRcvBuffer();
		void clearSendBuffer();
		void rcvData();
		void sendData();

	protected:
	
	private:
	
};
#endif // MYTCPCLIENT_H
