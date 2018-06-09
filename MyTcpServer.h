#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include "Mythread.h"
#include "PracticalSocket.h"
#include "MyTcpClient.h"
#include "Singleton.h"



class MyTcpServer : public MyThread,public Singleton<MyTcpServer>
{
	friend class Singleton<MyTcpServer>;
	private:
		//CONSTRUCTOR(S) AND DESTRUCTOR
		MyTcpServer();
		~MyTcpServer();
		//Member var for Singleton
		int _value;

	public:
		//Public interface for Singleton
	 	void setValue (int val) { _value = val; }
  		int getValue () { return _value; }

		
		//PUBLIC MEMBERS
		unsigned short m_echoServPort;	
		TCPServerSocket *m_servSock;
		MyTcpClient *tcpClient;	
		bool connCli;
		bool firstConn;	

		//PUBLIC METHODS
		
		void InternalThreadEntry();
		void setPortAndStart(unsigned short port);

	protected:
	
	
	private:
	
	
	
};
#endif // MYTCPSERVER_H
