/*
Parsing a config file
libs=sudo apt-get install libconfig++-dev
compilation = g++ -o test marcelconfig.cpp -lconfig++
*/
#ifndef MARCELCONFIG_H
#define MARCELCONFIG_H

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <libconfig.h++>
#include <string>
#include <vector>

class Myconf{

public:	

	Myconf();
	~Myconf();



	//PUBLIC MEMBERS
	libconfig::Config m_cfg;
	const char* m_filename="/usr/local/etc/marcel.cfg";

	//PUBLIC METHODS
	std::vector<std::string> getBinSettings();
	std::vector<int> getCamSettings();
	std::vector<int> getOpenCVIntSettings();
	std::vector<std::string> getOpenCVStrSettings();
	std::vector<bool> getOpenCVBoolSettings();
	std::vector<std::string> getSphinxStrSettings();

protected:
private:
	//PRIVATE METHODS
	int readConfigFile();


};
#endif 
