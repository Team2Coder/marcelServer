
/********** EXTERNAL LIBRARY IMPORT ***************/
#include <Servo.h> // librairie pour servomoteur
#include <SoftwareSerial.h>

/*********** DEFINES LIST *************************/
#define SERVO_COMMAND 0x66 //lettre f pour debug
#define ULTRASOUND_COMMAND 0x68  //lettre h pour debug
#define LEFT_MOTOR_COMMAND 0x69  //lettre i pour debug
#define RIGHT_MOTOR_COMMAND 0x6A //lettre j pour debug
#define LAMP_COMMAND 0x6B //lettre k pour debug
#define GET_SERVO_VAL_CMD 0x6C //lettre l pour debug
#define MIN_SPEED 50
#define MIN_DIST 20
                
/********** SERIAL PORT READ/WRITE ****************/                                                
byte ReceptionOctet=0;
int ReceptionNombre=0;
int ReceptionNbOctet=0; 

/********** BUFFER ******************************/
const int lgt=4;
byte buff[lgt];
int compt=0;

//BLUETOOTH ATTRIBUT
String strBT; //string that stores the incoming message

SoftwareSerial serialBT(13,12); //Bluetooth soft objet to pin RX-13 TX-12

/********** SERVO MOTOR CONFIGURATION *************/
const int SERVO_X=2;  //declaration constante de broche axe X
const int SERVO_Y=4;  //declaration constante de broche axe Y

int impulsion=1500; // variable pour impulsion - valeur médiane initiale

const int POS_MIN_X=910; // largeur impulsion pour position 0° servomoteur
//const int POS_MIN_X=910;// POS_MIN=550 pour TOWER PRO SG90 

const int POS_MAX_X=2200; // largeur impulsion pour position 180° servomoteur
//const int POS_MAX_X=2200; // POS_MAS=2330 pour TOWER PRO SG90

const int POS_MIN_Y=1300; // largeur impulsion pour position 0° servomoteur
//const int POS_MIN_Y=1300; // POS_MIN=550 pour TOWER PRO SG90 

const int POS_MAX_Y=2900; // largeur impulsion pour position 180° servomoteur
//const int POS_MAX_Y=2900; // POS_MAS=2330 pour TOWER PRO SG90

//OBJECT DECLARATION
Servo servo_X,servo_Y;

//Members
int servoValueX=127;
int servoValueY=127;

/********** ULTRASENSOR CONFIGURATION *************/

//POWER CONNECTED TO 5VDC
const int trigPin = A0;
const int echoPin = A1;

/********* MOTORS CONFIGURATION *********************/

// connect motor controller pins to Arduino digital pins
// left motor
int enA = 11;
int in1 = 9;
int in2 = 8;
// right motor
int enB = 5;
int in3 = 7;
int in4 = 6;

/********* LAMP CONFIGURATION *********************/
int lampPin = 3;
int lampState = LOW;

/************** SETUP OF ALL DEVICES ******************/ 
void setup()
{
  
  pinMode(SERVO_X, OUTPUT); 
  pinMode(SERVO_Y, OUTPUT); 
  servo_X.attach(SERVO_X); 
  servo_Y.attach(SERVO_Y);  
  moveServoMotorY(servoValueY);
  

  
  // set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  //USound in-out
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);

  //Lamp out
  pinMode(lampPin,OUTPUT);
  analogWrite(lampPin,0);
    
  //Initialisation des PORT COM
    Serial.begin(57600);
    while(!Serial){;}
    
    serialBT.begin(9600);

}

/*************** RUNNING RELEASE PROGRAMME ***************/
void loop()
{    
  
  for(char i=0;i<lgt;i++) buff[i]=0;
  ReceptionNbOctet=Serial.available();
  ReceptionOctet=0;
  compt=0;
    
  if(ReceptionNbOctet>0){
 
    while(ReceptionNbOctet>0){ 
    
      ReceptionOctet=Serial.read();
      if(compt<lgt) buff[compt]=ReceptionOctet;
      delay(1);
      ReceptionNbOctet= Serial.available();
      delay(1);    
      compt++;
      
    }

    switch(buff[0]){
       
    case SERVO_COMMAND:
      moveServoMotorX(buff[2]); 
      moveServoMotorY(buff[1]);
      break;
    
    case ULTRASOUND_COMMAND:
      UltrasonCheck();
      break;

    case LEFT_MOTOR_COMMAND:
      leftMotorSpeed(buff[1]);
      leftMotorDir(buff[2]);
      break;

    case RIGHT_MOTOR_COMMAND:
      rightMotorSpeed(buff[1]);
      rightMotorDir(buff[2]);
      break;

    case LAMP_COMMAND:
       if(buff[1]==107) lampState = HIGH;
       else lampState=LOW;
       analogWrite(lampPin,buff[1]);
       break;

     case GET_SERVO_VAL_CMD:
          Serial.print(servoValueX);
          Serial.print(servoValueY);
       break;          
    default:
      Serial.println("Rien!");
    
      break;    
    }
  }    
}

/*************** GESTION SERVO-MOTEURS ******************/
void moveServoMotorX(int value){
    servoValueX=value;
    //----- positionnement du servomoteur en fonction de la largeur d'impulsion demandée
    int impulse=(int)(value*((POS_MAX_X-POS_MIN_X)/255)+POS_MIN_X);
    if (impulse>POS_MAX_X)impulse=POS_MAX_X; // pour éviter valeur supérieure à 2600 = position maxi possible servomoteur
    if (impulse<POS_MIN_X)impulse=POS_MIN_X; // pour éviter valeur inférieure à 300 = position maxi possible servomoteur

    servo_X.writeMicroseconds(impulse); // génère l'impulsion de la durée voulue en µs pour le servomoteur
}

void moveServoMotorY(int value){
    servoValueY=value;
    //----- positionnement du servomoteur en fonction de la largeur d'impulsion demandée
    int impulse=(int)(value*((POS_MAX_Y-POS_MIN_Y)/255)+POS_MIN_Y);
    if (impulse>POS_MAX_Y)impulse=POS_MAX_Y; // pour éviter valeur supérieure à 2600 = position maxi possible servomoteur
    if (impulse<POS_MIN_Y)impulse=POS_MIN_Y; // pour éviter valeur inférieure à 300 = position maxi possible servomoteur

    servo_Y.writeMicroseconds(impulse); // génère l'impulsion de la durée voulue en µs pour le servomoteur
}

/************** GESTION ULTRASON *********************/
long microsecondsToCentimeters(long microseconds)
{
// The speed of sound is 340 m/s or 29 microseconds per centimeter.
// The ping travels out and back, so to find the distance of the
// object we take half of the distance travelled.
return microseconds / 29 / 2;
}

long UltrasonCheck()
{
  // establish variables for duration of the ping,
// and the distance result in inches and centimeters:
long duration, cm;
// The sensor is triggered by a HIGH pulse of 10 or more microseconds.
// Give a short LOW pulse beforehand to ensure a clean HIGH pulse:

digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Read the signal from the sensor: a HIGH pulse whose
// duration is the time (in microseconds) from the sending
// of the ping to the reception of its echo off of an object.
duration = pulseIn(echoPin, HIGH);
// convert the time into a distance

cm = microsecondsToCentimeters(duration);
Serial.print(cm);
delay(1);
return cm;
}

/***************** GESTION MOTEUR ********************/

void TurnOffMoteur(){
    // now turn off motors
    
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}

void rightMotorDir(byte dir){
    
  switch(dir){
  case 0x1:
    if(UltrasonCheck()>MIN_DIST){
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);}
    else TurnOffMoteur();
  break;  
  case 0x2:
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  break;  
  default:
   TurnOffMoteur();
  break; 
  }
}
void rightMotorSpeed(int i){
  if(i>MIN_SPEED) analogWrite(enB, i);
  else TurnOffMoteur();
}

void leftMotorDir(byte dir){
  switch(dir){

  case 0x1:
  if(UltrasonCheck()>MIN_DIST){
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);}
  else TurnOffMoteur();
  break;
  
  case 0x2:
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  break;
  
  default:
   TurnOffMoteur();
  break; 
  }
}

void leftMotorSpeed(int i){
 if(i>MIN_SPEED) analogWrite(enA, i);
  else TurnOffMoteur();
 //  Serial.println("vitesse gauche = ");
 //  Serial.println((int)i);
}


/************ FRONT/BACK MOTOR SAMPLE ***************/
void FrontBackTestMotor()
{

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  // set speed to 200 out of possible range 0~255
  analogWrite(enA, 255);
  // turn on motor B
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  // set speed to 200 out of possible range 0~255
  analogWrite(enB, 255);
  delay(2000);
  // now change motor directions
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH); 
  delay(2000);
  // now turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void SpeedMotorSample()
{
 
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH); 
  // accelerate from zero to maximum speed
  for (int i = 0; i < 256; i++)
  {
    analogWrite(enA, i);
    analogWrite(enB, i);
    delay(20);
  } 
  // decelerate from maximum speed to zero
  for (int i = 255; i >= 0; --i)
  {
    analogWrite(enA, i);
    analogWrite(enB, i);
    delay(20);
  } 
  // now turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);  
}

/********** GESTION BLUETOOTH ********************/


// READ INCOMING DATA FONCTION
void BluetoothReadData(){

  
  if (serialBT.available()>0) {
  Serial.println ("Serial BT available!");
  //---- lecture du nombre reçu
  while (serialBT.available()>0) {
    Serial.println ("Boucle BT");
    ReceptionOctet= serialBT.read(); 
    ReceptionOctet=ReceptionOctet-48; 

    if ((ReceptionOctet>=0)&&(ReceptionOctet<=9))     ReceptionNombre = (ReceptionNombre*10)+ReceptionOctet;
     Serial.print ("Nombre BT recu= ");
     Serial.println(ReceptionNombre);

    delay(1);

    }
  }
}
