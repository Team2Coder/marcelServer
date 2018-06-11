#include "CaptExp.h"
#include "MarcelRaspConf.h" 

CaptExp::CaptExp(){


}
CaptExp::~CaptExp(){


}


void CaptExp::setup() {
        pinMode(PIN_USTRIG, OUTPUT);
        pinMode(PIN_USECHO, INPUT);
 
        //TRIG pin must start LOW
        digitalWrite(PIN_USTRIG, LOW);
        delay(30);
}
 
int CaptExp::getCM() {
        //Send trig pulse
        digitalWrite(PIN_USTRIG, HIGH);
        delayMicroseconds(20);
        digitalWrite(PIN_USTRIG, LOW);
 
        //Wait for echo start
        while(digitalRead(PIN_USECHO) == LOW);
 
        //Wait for echo end
        long startTime = micros();
        while(digitalRead(PIN_USECHO) == HIGH);
        long travelTime = micros() - startTime;
 
        //Get distance in cm
        int distance = travelTime / 58;
 
        return distance;
}
void CaptExp::display(){
        std::cout << "Distance: " << getCM()<<std::endl;
}
