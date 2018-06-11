//Ears.h
#ifndef _INTERNETSEARCHER_H_
#define _INTERNETSEARCHER_H_

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


class Ears: public MyThread
{
	private:	
		//default constructors and destructors
		InternetSearcher();
		InternetSearcher(std::string search);
		~InternetSearcher();
		int _value;
		std::string mSearch;
        
	public:
		//Inherited virtual Method from Mythread object
		void InternalThreadEntry();	
	
	private:
		//METHODS
		
};

#endif
