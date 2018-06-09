#include "MyTcpClient.h"


MyTcpClient::MyTcpClient(){
	m_clntSock = new TCPSocket();
	clearRcvBuffer();
	clearSendBuffer();
	StartInternalThread();
}
MyTcpClient::MyTcpClient(TCPSocket *sock){
	m_clntSock=sock;
	clearRcvBuffer();
	clearSendBuffer();
	StartInternalThread();
}

MyTcpClient::~MyTcpClient(){
	
	
}

void MyTcpClient::InternalThreadEntry(){
	
	std::cout << "\n MyTcpClient running...\n"<<std::endl;
	 // Guarantees that thread resources are deallocated upon return  
  pthread_detach(pthread_self()); 
  
  // Extract socket file descriptor from argument  
  HandleTCPClient();

  delete m_clntSock;
  return NULL;
}

void MyTcpClient::HandleTCPClient(){
	cout << "Handling client \n";
	cout.flush();
  try {
    cout << m_clntSock->getForeignAddress() << ":";
  } catch (SocketException &e) {
    cerr << "Unable to get foreign address" << endl;
  }
  try {
    cout << m_clntSock->getForeignPort();
  } catch (SocketException &e) {
    cerr << "Unable to get foreign port" << endl;
  }
  cout << " with thread " << pthread_self() << endl;
  cout.flush();
  
  
  
  while(m_rcvBuffer[0]!=0xFE){
	rcvData();
 }
}

void MyTcpClient::clearRcvBuffer(){
	for(int i=0;i<m_RCVBUFSIZE-1;i++) m_rcvBuffer[i]=0x00;
}

void MyTcpClient::clearSendBuffer(){
	
	for(int i=0;i<m_SENDBUFSIZE-1;i++) m_sendBuffer[i]=0x00;
}

void MyTcpClient::rcvData(){
	m_clntSock->recv(m_rcvBuffer, m_RCVBUFSIZE);
	usleep(1000);
	clearRcvBuffer();

}

void MyTcpClient::sendData(){
	m_clntSock->send(m_sendBuffer, m_SENDBUFSIZE);
	usleep(1000);
	clearSendBuffer();
}


