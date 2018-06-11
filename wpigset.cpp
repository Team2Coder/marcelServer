#include <wiringPi.h>
#include <iostream>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int main(void){
	string res;
	if(wiringPiSetupGpio()==0) cout << "wiringPi set with Broadcom GPIO pin numbers\n";
	else perror("wiringPiGpioSetup :");
		
return EXIT_SUCCESS;
}
