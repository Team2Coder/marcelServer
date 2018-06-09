#include "Action.h"
using namespace std;

Action::Action(){
	
	Myconf cfg;

	std::vector<std::string> parse = cfg.getBinSettings();
	wpiPath=parse[0];
	espkPath=parse[1];

	InitRaspiConf();
	InitMotors();
	m_serialState=InitSerialCom();
	ClearReturnBuffer();
	readyToSend=false;

}

Action::~Action(){
	
}

void Action::selectAction(unsigned char* buf){
	
	
	
	if(buf != nullptr && strlen(buf)>0){
		cout << "\nCommand = ";

		switch(buf[0]){

			case 0x01:
			{
				cout << "set Leds";
				setLeds((int)buf[1],(int)buf[2],(int)buf[3]);
			}
			break;

			case 0x02:
			{
				cout << "set Servo";
				int pwmValueX = (int)buf[2];
                		int pwmValueY = (int)buf[1];
		                if(pwmValueX>0 && pwmValueX<255)  setServo(PIN_SERVOAXIS_X,pwmValueX) ;
		                if(pwmValueY>0 && pwmValueY<255)  setServo(PIN_SERVOAXIS_Y,pwmValueY) ;
			}
			break;

			case 0x03:
			{
				cout << "Switch off leds";
				setLeds(0,0,0);
			}
			break;

			case 0x04:
			{
				cout << "UltraSound receive";
				/*pwmBuffer[1]=ultrason.getCM();
					pwmBuffer[2]=0;
				ultrason.display();
				*/
				//marcelVision->selectVisionFeature();
			}
			break;

			case 0x05:
			{
				cout << "Left motor";
				motorLeft.setSpeed(buf[1]);
				motorLeft.setDirection(buf[2]);
			}
			break;

			case 0x06:
			{
				cout << "Right motor";
				motorRight.setSpeed(buf[1]);
				motorRight.setDirection(buf[2]);
			}
			break;

			case SERIAL_ULTRASOUND_COMMAND:
			{
					ReadSerialCom(buf);
					std::cout << " cm \n" ;
					std::cout.flush();
				
			}
			break;

			case GET_SERVO_VAL_CMD:
			{
					ReadSerialCom(buf);
					std::cout << " <-- X Y Servo values \n" ;
					std::cout.flush();
			}
			break;
			
			case GET_CV_COMMAND:
			{
				cout << "OCV command =" << (int)buf[1] << "\n";

			}

			default:
			{
				if(buf[0] >= 0x64){
					cout << "Arduino command =" << (int)buf[0] << "\n";
					if(m_serialState>0){WriteSerialCom(buf);}
					else cout << "Arduino unreachable!\n";

				}
				else  cout << " unknown command buffer!\n";
			
			}
			break;
		}
	}

	return buf;
}

void Action::saySomething(std::string say){
		
		Processes *proc = new Processes();
		char *arguments[] = {espkPath.c_str(), "-v", "mb-fr1", "-s" , "200", say.c_str(), (char *)0 };
		proc->forkExec(arguments);
		delete proc;
	
}

void Action::setLeds(int r,int g,int b){

        softPwmWrite(PIN_PWM_R, r); // PWM LED at dim setting
        softPwmWrite(PIN_PWM_G, g); // PWM LED at dim setting
        softPwmWrite(PIN_PWM_B, b); // PWM LED at dim setting	

}

void Action::setServo(int pin,int value){
	int valueMicros=(int)(value*((SERVO_LEFT-SERVO_RIGHT)/255)+SERVO_RIGHT);
        cout << "Servo value: " << valueMicros << endl;
        digitalWrite(pin,LOW);
        delayMicroseconds(valueMicros);
        digitalWrite(pin,HIGH);
        delayMicroseconds(6000-valueMicros);

} 

void Action::InitRaspiConf(){
	

        Processes *proc = new Processes();
        char *arguments[] = {"sudo",wpiPath.c_str() ,(char *)0 };
        proc->forkExec(arguments); // Initialize wiringPi -- using Broadcom pin numbers
        delete proc;


	pwmSetMode(PWM_MODE_MS);
        pwmSetClock(1000);
        //Initialisation de 3 PWM pour RGB
        softPwmCreate(PIN_PWM_R,0,255); // Setup PWM using Pin, Initial Value and Range parameters
        softPwmCreate(PIN_PWM_G,0,255); // Setup PWM using Pin, Initial Value and Range parameters
        softPwmCreate(PIN_PWM_B,0,255); // Setup PWM using Pin, Initial Value and Range parameters

        //Initialisation des pins des servo moteurs en sorties
	pinMode(PIN_SERVOAXIS_X, OUTPUT);
        pinMode(PIN_SERVOAXIS_Y,OUTPUT);
}

void Action::InitMotors(){
	
	motorLeft.setup(PIN_MOTOR_LEFT_ENA,PIN_MOTOR_LEFT_IN1,PIN_MOTOR_LEFT_IN2);
	motorRight.setup(PIN_MOTOR_RIGHT_ENB,PIN_MOTOR_RIGHT_IN3,PIN_MOTOR_RIGHT_IN4);
}

int Action::InitSerialCom()
{

 	int serialState=arduino.setup(SERIAL_NAME,BAUD_RATE);
	return serialState;
}

void Action::WriteSerialCom(unsigned char *buff)
{
	arduino.write((char*)buff);
	delay(1);
}

std::vector<char> Action::ReadSerialCom(unsigned char* buf){

	 std::vector<char> tab;

	if(m_serialState>0){
		WriteSerialCom(buf);
        	ClearReturnBuffer();
                usleep(20000);
                tab=arduino.read();
                usleep(20000);

                for(int i=0; i<tab.size(); i++)
                {
                	std::cout << tab[i];
                        m_return[i]=tab[i];
                }
                        readyToSend=true;
         }
         else {
         	cout << "Arduino unreachable!\n";
                readyToSend=true;
                ClearReturnBuffer();
                m_return[0]=0x58;
                m_return[1]=0x58;
                m_return[2]=0x58;
         }

	return tab;

}



void Action::InitUltrasound(){
        ultrasound.setup();
	ultrasound.display();
}

void Action::ClearReturnBuffer(){

	for(int i=0;i<MyTcpClient::m_SENDBUFSIZE-1;i++) m_return[i]=0x00;
}
