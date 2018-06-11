//////----------------------------------------------------------------------------------------------------
////// Name:                mainMarcel.cpp
////// Programmer:          Cedric Leguay/Sebastien Lorrain
////// Created:             25/10/2015
////// Microcontroller:     Raspberry Pi2 v1
////// Purpose:             Controle Marcel Bot via Android 
////// Dependencies:        wiringPi, https://projects.drogon.net/raspberry-pi/wiringpi/download-and-install/
////// Compiled with:       sudo make 
////// Schematic:
//////
////// Notes:
//////----------------------------------------------------------------------------------------------------

#include "mainMarcel.h"

   using namespace std;


int main(int argc, char *argv[]) {
  	if (argc != 2) {                 // Test for correct number of arguments  
    		cerr << "Usage: " << argv[0] << " <Server Port> " << endl;
    		exit(1);
  	}
	cout << "Serveur Running...\n";
	cout.flush();

	ai = AI::getInstance();

	ai->tcpServer->setPortAndStart(atoi(argv[1]));

	for (;;) {      // Run forever  
	}
  // NOT REACHED
  return 0;
}
