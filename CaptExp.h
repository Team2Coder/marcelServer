//////----------------------------------------------------------------------------------------------------
////// Name:                CaptExp.h
////// Programmer:          Sébastien Lorrain
////// Created:             25/09/2015
////// Microcontroller:     Raspberry Pi B+
////// Purpose:             Ultrasound sensor management, sends signal trigger and waits for sensor's return
////// Dependencies:        wiringPi, https://projects.drogon.net/raspberry-pi/wiringpi/download-and-install/
////// Compiled with:       To compile as an object in an entire project
////// Schematic:
//////
////// Notes:		    This file is an object class header
//////----------------------------------------------------------------------------------------------------


#ifndef CaptExp_class
#define CaptExp_class

#include <stdlib.h>
#include <iostream>
#include <wiringPi.h>
 
 
class CaptExp{

public:
	CaptExp();
	~CaptExp();
	void setup();
	int getCM();
	void display();
};


#endif

