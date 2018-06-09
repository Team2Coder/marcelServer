#include <unistd.h> // for execlp
#include <signal.h> // For kill() to work
#include <iostream> // For cout, cerr
#include <cstdlib>  // For atoi()
#include <pthread.h>// For POSIX threads
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "Mp3player.h"

#ifndef Processes_class
#define Processes_class

using namespace std;

class Processes
{

	public : 
		Processes();
		virtual ~Processes();

	void forkExec(char **arguments);
	pid_t create_process(void);
	void execute_process(std::string binexec, std::string fileDirectory);
	void father_process(void);
	void* mjpgStream(void *arg);
};

#endif // PROCESSES_H
