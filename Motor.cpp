#include "Motor.h"
#include "MarcelRaspConf.h" 



Motor::Motor(){
}
Motor::Motor(int pinEN,int pin1,int pin2){
setup(pinEN,pin1,pin2);

}
Motor::~Motor(){


}

void Motor::setup(int pinsEN,int pins1,int pins2) {

	m_pinEN=pinsEN;
	m_pin1=pins1;
	m_pin2=pins2;

	pinMode(m_pinEN, OUTPUT);
	pinMode(m_pin1, OUTPUT);
	pinMode(m_pin2, OUTPUT);

	softPwmCreate(m_pinEN,0,255);
}

void Motor::setDirection(int dir)
{
	std::cout << "dir = " << dir << "\n"; 
	switch(dir) {

		case 1 : 
			forward();
	    	break;       

		case 2 :
			backward();
		break;
		case 0 :
			stop();
		break;
		default :
			stop();
		break;
		}
}
void Motor::forward(){
	digitalWrite(m_pin1,HIGH);
	digitalWrite(m_pin2,LOW);
}

void Motor::backward(){
 	digitalWrite(m_pin1,LOW);
 	digitalWrite(m_pin2,HIGH);
}

void Motor::stop(){
 	softPwmWrite(m_pinEN,0);
 	digitalWrite(m_pin1,LOW);
 	digitalWrite(m_pin2,LOW);
}

void Motor::setSpeed(int speed){
        std::cout << "speed = " << speed << "\n"; 

 	softPwmWrite(m_pinEN,speed);
}
