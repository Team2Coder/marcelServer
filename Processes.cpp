#include "Processes.h"

//pthread_mutex_t mutex_mjpg = PTHREAD_MUTEX_INITIALIZER; /* Création du mutex */

Processes::Processes()
{  //Cst
}

Processes::~Processes()
{
    //dtor
}
//fork and exec arg table pointer
void Processes::forkExec(char **arguments){

        pid_t pid;
        int status;

        if(!(pid = fork()) < 0) {
                cout << "ERROR: forking child process failed" << endl;
                exit(1);
        }
        else if(pid == 0) {
                if(execvp(*arguments, arguments) < 0) {
                        cout << "ERROR: exec failed" << endl;
                        exit(1);
                }
        }
        else {
                while (wait(&status) != pid);
             }

}



pid_t Processes::create_process(void)
{

    /* On crée une nouvelle valeur de type pid_t */
    pid_t pid;
    /* On fork() tant que l'erreur est EAGAIN */
    do {
    pid = fork();
    } while ((pid == -1) && (errno == EAGAIN));
    /* On retourne le PID du processus ainsi créé */
    return pid;
}

void Processes::execute_process(std::string binexec, std::string fileDirectory){

                if(binexec == "aplay"){
                        std::string command = "aplay ";
                        command += fileDirectory;
                        const char * execcommand = command.c_str();
                        system(execcommand);}
                else if(binexec == "mpg123"){
                        std::string command = "mpg123 ";
                        command += fileDirectory;
                        system(command.c_str());}
                else if(binexec == "mpg321"){
                        std::string command = "mpg321 ";
                        command += fileDirectory;
                        system(command.c_str());}


}

void Processes::father_process(void)

{

    cout << "Pocketsphinx Process launched!" << endl ;

}

void* Processes::mjpgStream(void *arg) {

	//pthread_mutex_unlock (&mutex_mjpg);
        char *arguments[] = {"bin/mjpg_streamer", "-i", "plugins/input_file.so -d 0 -f tmp -n output.jpg", "-o", "plugins/output_http.so" , (char *)0 };
        execvp("bin/mjpg_streamer", arguments);
	//pthread_mutex_unlock (&mutex_mjpg);
}
