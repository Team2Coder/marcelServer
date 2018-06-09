#ifndef AI_H
#define AI_H

#include <iostream> // For cout, cerr
#include <cstdlib>  // For atoi()
#include <string>
#include "Vision.h"
#include "Ears.h"
#include "MyTcpServer.h"
#include "Action.h"
#include "MarcelRaspConf.h"
#include "Mythread.h"
#include "Singleton.h"
#include "Myconf.h"

class AI:public MyThread,public Singleton<AI>
{
        friend class Singleton<AI>;
        private:
                //CONSTRUCTOR(S) AND DESTRUCTOR
                AI();
                ~AI();
                //Member var for Singleton
                int _value;
	public:

		//Members
		Vision *marcelVision;
	        Ears *marcelEars;
        	MyTcpServer *tcpServer;
        	Action *act;
		
		unsigned char buffTemp[MyTcpClient::m_RCVBUFSIZE];
        	bool firstGetServoXY;
	        int servoCount;
        	int speedX,speedY;

                //Public interface for Singleton
                void setValue (int val) { _value = val; }
                int getValue () { return _value; }

		//Public Methods
		void InternalThreadEntry();
	        void comManagement();
        	void headTrackFromVision();
        	void clearBuffTemp();





};

#endif //AI_H
