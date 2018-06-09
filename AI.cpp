#include "AI.h"

using namespace std;

AI::AI(){
	std::cout << "Creating new AI\n";

	Myconf cfg;
        vector<int> parsecamInt = cfg.getCamSettings();
	speedX=parsecamInt[3];
	speedY=parsecamInt[4];

	clearBuffTemp();
	firstGetServoXY=true;
	
	tcpServer = MyTcpServer::getInstance();

	marcelVision = Vision::getInstance();
	
	marcelEars = Ears::getInstance();

	act = Action::getInstance();
	act->setLeds(100,100,100); //White = wait for connection

	StartInternalThread();
}

AI::~AI(){

}

void AI::InternalThreadEntry(){
	std::cout << "Internal Thread Launched\n";

	for (;;) {      // Run forever
                usleep(100);
                comManagement();
                headTrackFromVision();
	}
	std::cout << "Internal Thread Finished\n";
}

void AI::comManagement(){

                if (tcpServer->connCli) {

                        if(tcpServer->firstConn) {
                                act->saySomething("Bonjour, je suis connecté");
                                act->setLeds(100,0,100); //Green = connection OK
                                tcpServer->firstConn=false;
                        }
			
                       act->selectAction(tcpServer->tcpClient->m_rcvBuffer);
                        //Sending data from Action to TcpClient
                        if(act->readyToSend){
   				strcpy(tcpServer->tcpClient->m_sendBuffer,act->m_return);
                                tcpServer->tcpClient->sendData();
                                act->readyToSend=false;
                        }
			if(tcpServer->tcpClient->m_rcvBuffer[0]==GET_CV_COMMAND 
			&& tcpServer->tcpClient->m_rcvBuffer[1]>=0 
			&& tcpServer->tcpClient->m_rcvBuffer[1]<=6)
				marcelVision->m_ocv_feature=tcpServer->tcpClient->m_rcvBuffer[1];
				
			
			

                }
}

void AI::headTrackFromVision(){

        if(marcelVision->m_ocv_feature==1 || marcelVision->m_ocv_feature==6){

        if(firstGetServoXY){
                servoCount=0;
                buffTemp[2]=127;
                firstGetServoXY=false;
                buffTemp[0]= SERIAL_SERVO_COMMAND;

        }



                if(marcelVision->posTrack[0]<marcelVision->m_width/3){
                        //buffTemp[1]+=1;
                        if(servoCount < speedX){
                        servoCount++;
                        }
                        else{
                        if(buffTemp[2]<200){
                                buffTemp[2]+=1;
                                act->selectAction(buffTemp);
                        }
 			servoCount=0;
                        }
		}
		  if(marcelVision->posTrack[0]>marcelVision->m_width-(marcelVision->m_width/3)){
                //buffTemp[1]-=1;
                        if(servoCount < speedX){
                        servoCount++;
                        }
                        else{
                        if(buffTemp[2]>30){
                                buffTemp[2]-=1;
                                act->selectAction(buffTemp);
                        }
                        servoCount=0;
                        }


                }
                marcelVision->posCamX=(int)buffTemp[2];

        }


}

void AI::clearBuffTemp(){


                buffTemp[0]= SERIAL_SERVO_COMMAND;
                buffTemp[1]= 190;
                buffTemp[2]= 190;
                buffTemp[3]= 190;


}
