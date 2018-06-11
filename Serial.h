//////----------------------------------------------------------------------------------------------------
////// Name:                Serial.h
////// Programmer:          Sébastien Lorrain
////// Created:             21/11/2015
////// Microcontroller:     Raspberry Pi B+
////// Purpose:             Handle Serial Arduino Communication
////// Dependencies:        wiringPi
////// Compiled with:       To compile as an object in an entire project
////// Schematic:
//////
////// Notes:		    This file is an object class header
//////----------------------------------------------------------------------------------------------------


#ifndef Serial_class
#define Serial_class

#include <stdlib.h>
#include <iostream>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <vector>
#include <unistd.h>
class Serial{

public:
//METHODS
	Serial();
	Serial(const char* name,int baudRate);
	~Serial();
	int setup(const char* name,int baudRate);
	std::vector<char> read();
	void write(char* data);
//ATTRIBUTES
	int m_baudRate;
	int m_handle;
	const char* m_name;
};


#endif


