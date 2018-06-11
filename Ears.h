//Ears.h
#ifndef _EARS_H_
#define _EARS_H_

#include <sphinxbase/err.h>
#include <sphinxbase/ad.h>
#include <pocketsphinx/pocketsphinx.h>

#include <cstdlib>
#include <string>
#include <vector>

#if !defined(_WIN32_WCE)
#include <signal.h>
#include <setjmp.h>
#endif
#if defined(WIN32) && !defined(GNUWINCE)
#include <time.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif

#include "Myconf.h"
#include "Mythread.h"
#include "Singleton.h"

#define WNF "wnf" //word not found flag
//using namespace std;

class Ears: public MyThread,public Singleton<Ears>
{
	friend class Singleton<Ears>;
	private:	
		//default constructors and destructors
		Ears();
		~Ears();
		int _value;
        
	public:
		//public interface for Singleton
		void setValue (int val) { _value = val; }
	  	int getValue () { return _value; }
		
		//methods for init
		int init(); 
		int setLMdir(std::string);
		int setDICdir(std::string);
		
		//methods for microphone recognizing
		void prepare_ad();	
		std::string listen();
		void print_word_times();
		//std::string transcode();
		void close_ad();
		
		//methods for raw audio file recognizing
		void record(std::string,std::string);
		std::string rawToString(const char*);
		int check_wav_header(char *header, int expected_sr);
		
		//Inherited virtual Method from Mythread object
		void InternalThreadEntry();	
	
	private:
		//METHODS
		int updateModel();
		void sleep_msec(int32);
				
		//MEMBERS
		
		//init members
		std::string lmDir;
		std::string dicDir;
		ps_decoder_t *ps;
		std::string m_modelDir= "";
		std::string m_dict="sphinxlm/marcel.dic";
		std::string m_hmm= "sphinxlm/";
		std::string m_lm= "sphinxlm/french3g62K.DMP";
		std::string m_hw= "plughw:1,0";
		
		//decode from file members
		cmd_ln_t *config;
		FILE *rawfd;
		char const *hyp;
		std::string hyp2;
		char const *uttid;
		int16 buf[512];
		int rv;
		int32 score;

		//decode from mic members
		ad_rec_t *ad;
		int16 adbuf[2048];
		int32 k;
		int32 ts;
		int32 rem;
		uint8 in_speech;
		uint8 utt_started;	
		char word[256];
		
};

#endif
