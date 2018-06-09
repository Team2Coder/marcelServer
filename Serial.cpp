#include "Serial.h"
#include "MarcelRaspConf.h" 
#include <vector>



Serial::Serial(){
}
Serial::Serial(const char* name,int baudRate){
setup(name,baudRate);
}
Serial::~Serial(){


}

int Serial::setup(const char* name,int baudRate) {

	m_name=name;
	m_baudRate=baudRate;
        m_handle = serialOpen (m_name, m_baudRate);
        std::cout << "Serial State = "<<m_handle<<"\n";

	return m_handle;

}

std::vector<char> Serial::read(){

	std::cout << "Serial port read...";
	std::vector<char> readTab;


	while(serialDataAvail(m_handle)>0){ 
		char lirecar = (char) serialGetchar (m_handle);
		readTab.push_back(lirecar);
		usleep(3000);
	}
	serialFlush(m_handle);

	std::cout  <<"OK\n";
	std::cout.flush();
        //std::cout <<"\n";
	return readTab;
}

void Serial::write(char *data){

	std::cout << "Serial port writing = " <<(int) data[0] <<" "<<(int) data[1] <<" "<<(int) data[2] <<" "<<(int) data[3] << " \n";
	std::cout.flush();
	serialPuts(m_handle,data);
	serialFlush(m_handle);

}
