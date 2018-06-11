//////----------------------------------------------------------------------------------------------------
////// Name:                Motor.h
////// Programmer:          Sébastien Lorrain
////// Created:             11/01/2015
////// Microcontroller:     Raspberry Pi B+
////// Purpose:             DC Motor Management for Keyes L298 Module
////// Dependencies:        wiringPi
////// Compiled with:       To compile as an object in an entire project
////// Schematic:
//////
////// Notes:		    This file is an object class header
//////----------------------------------------------------------------------------------------------------


#ifndef Motor_class
#define Motor_class

#include <stdlib.h>
#include <iostream>
#include <wiringPi.h>
#include <softPwm.h> 
 
class Motor{

public:
//METHODS
	Motor();
	Motor(int pinEN,int pin1,int pin2);
	~Motor();
	void setup(int pinsEN,int pins1,int pins2);
	void setDirection(int dir);
	void forward();
	void backward();
	void stop();
	void setSpeed(int speed);

protected:
//ATTRIBUTES
	int m_pinEN;
	int m_pin1;
	int m_pin2;
	
};


#endif

