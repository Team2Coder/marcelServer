/**************MARCEL HARDWARE RASPI CONFIG BASE******************************/


#include <wiringPi.h> // Include WiringPi library!
#include <softPwm.h> //Include SoftPwm library
#include <wiringSerial.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define SERIAL_NAME "/dev/ttyACM0"
#define BAUD_RATE 57600

#define PIN_PWM_B 2 //5 Regular LED - Broadcom pin 23, P1 pin 16
#define PIN_PWM_G 3 //6
#define PIN_PWM_R 4 //13

#define DELAY 1
#define SERVO_RIGHT 500
#define SERVO_LEFT 2300
#define SERVO_CENTER 1400

//Config Seb
#define PIN_MOTOR_LEFT_ENA 2 
#define PIN_MOTOR_LEFT_IN1 3 
#define PIN_MOTOR_LEFT_IN2 4 
//Config Cedric
//#define PIN_MOTOR_LEFT_ENA 5 
//#define PIN_MOTOR_LEFT_IN1 6 
//#define PIN_MOTOR_LEFT_IN2 13
#define PIN_MOTOR_RIGHT_ENB 25
#define PIN_MOTOR_RIGHT_IN3 14
#define PIN_MOTOR_RIGHT_IN4 15

#define PIN_SERVOAXIS_X 18
#define PIN_SERVOAXIS_Y 12
//#define PIN_USTRIG 6
//#define PIN_USECHO 5
#define PIN_USTRIG 23
#define PIN_USECHO 24
#define USND_IS_PLUGGED false

#define SERIAL_SERVO_COMMAND 0x66 //lettre f pour debug
#define SERIAL_ULTRASOUND_COMMAND 0x68  //lettre h pour debug
#define SERIAL_LEFT_MOTOR_COMMAND 0x69  //lettre i pour debug
#define SERIAL_RIGHT_MOTOR_COMMAND 0x6A //lettre j pour debug
#define SERIAL_LAMP_COMMAND 0x6B //lettre k pour debug
#define GET_SERVO_VAL_CMD 0x6C //lettre l pour debug
#define GET_CV_COMMAND 0x6D //lettre m pour debug
