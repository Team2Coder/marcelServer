#include "Myconf.h"

using namespace std; 
using namespace libconfig; // This example reads the configuration file 'example.cfg' and displays // some of its contents. 

Myconf::Myconf(){

	readConfigFile();
}

Myconf::~Myconf(){

}

int Myconf::readConfigFile(){

  std::cout << "config file : " << m_filename << "\n";
 // Read the file. If there is an error, report it and exit.

  try
  {
    m_cfg.readFile(m_filename);
  }
  catch(const FileIOException &fioex)
  {
    std::cerr << "I/O error while reading file." << std::endl;
    return(EXIT_FAILURE);
  }
  catch(const ParseException &pex)
  {
    std::cerr << "Parse error at " << pex.getFile() << ":" <<pex.getLine()
              << " - " << pex.getError() << std::endl;
    return(EXIT_FAILURE);
  }
}

vector<std::string> Myconf::getBinSettings(){
	const Setting& root = m_cfg.getRoot();
	const Setting &m_subroot=root["binaries"]; 
	const int nbElm=2;
	string strTab[nbElm];

	try
	{
        m_subroot.lookupValue("wpigsetupPath",strTab[0]);
        m_subroot.lookupValue("espeakPath",strTab[1]);
 	} catch(const SettingNotFoundException &nfex){
    	cout << "No  settings in configuration file." << endl;
  	}

	vector<string> temp;

        for(int i=0;i<nbElm;i++){
                temp.push_back(strTab[i]);
                cout << temp[i] << "\n";
        }
        return temp;
}

vector<int> Myconf::getCamSettings(){
  // Get the camera settings.
	const Setting& root = m_cfg.getRoot();
	const Setting &m_subroot=root["camera"]; 
	const int nbElm=5;
	int intTab[nbElm];

try
  {
   	m_subroot.lookupValue("width",intTab[0]);
    	m_subroot.lookupValue("height",intTab[1]);
    	m_subroot.lookupValue("fps",intTab[2]);
    	m_subroot.lookupValue("speedX",intTab[3]);
    	m_subroot.lookupValue("speedY",intTab[4]);

  }
  catch(const SettingNotFoundException &nfex)
  {
    cout << "No  settings in configuration file." << endl;
  }
	vector<int> temp;
        for(int i=0;i<nbElm;i++){
                temp.push_back(intTab[i]);
        }
	

	return temp;
}

vector<int> Myconf::getOpenCVIntSettings(){ // Get the opencv settings.
        const Setting& root = m_cfg.getRoot();
        const Setting &m_subroot=root["opencv"];

        int feat=0,devId=0; 
  try
  {
        m_subroot.lookupValue("feature",feat);
        m_subroot.lookupValue("deviceId",devId);
  }
  catch(const SettingNotFoundException &nfex)
  {
    cout << "No settings in configuration file." << endl;
  }
        vector<int> temp;
        temp.push_back(feat);
	temp.push_back(devId);
	return temp;
}
vector<string> Myconf::getOpenCVStrSettings(){ // Get the opencv settings.
        const Setting& root = m_cfg.getRoot();
        const Setting &m_subroot=root["opencv"];

	const int nbElm=8;
	string strTab[nbElm];
  try
  {
        m_subroot.lookupValue("haarcascadePath",strTab[0]);
        m_subroot.lookupValue("saveImagePath",strTab[1]);
        m_subroot.lookupValue("extension",strTab[2]);
        m_subroot.lookupValue("csvPath",strTab[3]);
        m_subroot.lookupValue("csvFile",strTab[4]);
        m_subroot.lookupValue("outputStreamPath",strTab[5]);
        m_subroot.lookupValue("mjpgPath",strTab[6]);
        m_subroot.lookupValue("objTrackParams",strTab[7]);
  }
  catch(const SettingNotFoundException &nfex)
  {
    cout << "No settings in configuration file." << endl;
  }
        vector<string> temp;

	for(int i=0;i<nbElm;i++){
        	temp.push_back(strTab[i]);
	}
	return temp;
}

std::vector<bool> Myconf::getOpenCVBoolSettings(){

	const Setting& root = m_cfg.getRoot();
     	const Setting &m_subroot=root["opencv"];

        bool namedwindow=false;
  	try
  	{
       		m_subroot.lookupValue("displaynamedWindows",namedwindow);
  	}
  	catch(const SettingNotFoundException &nfex)
  	{
    		cout << "No settings in configuration file." << endl;
  	}
        vector<bool> temp;
        temp.push_back(namedwindow);
        return temp;


}



std::vector<std::string> Myconf::getSphinxStrSettings(){
        const Setting& root = m_cfg.getRoot();
        const Setting &m_subroot=root["sphinx"];
	
	const int nbElm=5;
        string strTab[nbElm];
  try
  {
        m_subroot.lookupValue("modelDir",strTab[0]);
        m_subroot.lookupValue("dict",strTab[1]);
        m_subroot.lookupValue("hmm",strTab[2]);
        m_subroot.lookupValue("lm",strTab[3]);
		m_subroot.lookupValue("hw",strTab[4]);

  }
  catch(const SettingNotFoundException &nfex)
  {
    cout << "No settings in configuration file." << endl;
  }
        vector<string> temp;

	for(int i=0;i<nbElm;i++){
        	temp.push_back(strTab[i]);
		cout << temp[i] << "\n";
	}
	return temp;





}

