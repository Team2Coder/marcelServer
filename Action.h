#ifndef ACTION_H
#define ACTION_H
#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>
#include "Processes.h"
#include "MarcelRaspConf.h"
#include "CaptExp.h"
#include "Motor.h"
#include "Serial.h"
#include "MyTcpClient.h"
#include "Singleton.h"
#include "Myconf.h"

class Action:public Singleton<Action>
{
	friend class Singleton<Action>;
private:
	Action();
        ~Action();
	 //Member var for Singleton
  	int _value;
public:
	 //Public interface for Singleton
  	void setValue (int val) { _value = val; }
  	int getValue () { return _value; }

	Motor motorLeft;
        Motor motorRight;
	Serial arduino;
	CaptExp ultrasound;
	int m_serialState;

	unsigned char m_return[MyTcpClient::m_SENDBUFSIZE];
	bool readyToSend;
	string wpiPath;
	string espkPath;


	void selectAction(unsigned char*);
	void saySomething(std::string);
	void setLeds(int r,int g, int b);
	void setServo(int pin,int value);
	void InitRaspiConf();
	void InitMotors();
	int InitSerialCom();
	void WriteSerialCom(unsigned char*);
	std::vector<char> ReadSerialCom(unsigned char* buf);
	void InitUltrasound();
	void ClearReturnBuffer();
protected:

private:







};

#endif // ACTION_H
