#include "MyTcpServer.h"


MyTcpServer::MyTcpServer(){
	
	m_echoServPort=4444;
	connCli=false;
	firstConn=false;
	 
}


MyTcpServer::~MyTcpServer(){
	
	
}

void MyTcpServer::InternalThreadEntry(){
	
	  m_servSock = new TCPServerSocket(m_echoServPort);
	cout << "\n Tcp internal Thread Started with port : "<< m_echoServPort <<"!\n";
	

    for (;;) {      // Run forever 
		try{
			// Create separate memory for client argument  
		
			TCPSocket *clntSock = m_servSock->accept();  
						
			// Create TcpClient  
			tcpClient = new MyTcpClient(clntSock);	
			connCli=true;
			firstConn=true;	
			
			
		} catch (SocketException &e) {
			cerr << e.what() << endl;
			connCli=false;
			firstConn=false;
			exit(1);
		}
	
	}
}

void MyTcpServer::setPortAndStart(unsigned short port){

	m_echoServPort=port;
        StartInternalThread();

}
