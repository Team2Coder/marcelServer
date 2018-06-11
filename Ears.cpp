//Ears.cpp

#include "Ears.h"
//create a handler that will loop and listen for commands
//passing the recognized words to the logic node for processing

using namespace std;

Ears::Ears()
{

	//this is a very basic langauge model that comes with pocketsphinx
	//use the setLMdir and setDICdir to set your own language models
	Myconf conf;
	vector<string> parse = conf.getSphinxStrSettings();

	m_modelDir=parse[0];
	m_dict = m_modelDir + parse[1];
	m_hmm =  m_modelDir + parse[2];
	m_lm =   m_modelDir + parse[3];
	m_hw =    parse[4];
	
	for(int i=0;i<4;i++)
	cout << parse[i]<<"\n";
	
	StartInternalThread();
	
}

Ears::~Ears()
{
	fclose(rawfd);
	ps_free(ps);
}
//this will give us access to pocket sphinx's cmd_ln.c interpreter
int Ears::init()
{
	
	//create a config object
	cout<<"Ears init...";
	config = cmd_ln_init(NULL,ps_args(), TRUE,
			     "-hmm", m_hmm.c_str(),
			     "-lm",  m_lm.c_str(),
			     "-dict", m_dict.c_str(),
			    // "-ds","2",      //downsampling lower should be better and higher should be less accurate
                            // "-topn","2",
			    // "-lpbeam","1e-40",
			    // "-maxwpf","5",
			    // "-maxhmmpf","3000",
			     "-logfn","/dev/null",
			     "-nfft","2048",
			     NULL);
	if (config == NULL)
		return 1;
		
	//initialize the decoder	
	ps = ps_init(config);
	if (ps == NULL)
		return 1;
	cout<<"OK!"<<endl;
	return 0;
}

//the ability to set the location for the models will allow new models to be composed and replaced with ease
int Ears::setLMdir(std::string _lmDir)
{
	std::string temp = m_modelDir;
	
	lmDir = temp+ _lmDir;
	if(lmDir.c_str()==NULL)
		return -1;
		
	updateModel();//update the LM if config already exists
	return 1;
}

int Ears::setDICdir(std::string _dicDir)
{
	std::string temp = m_modelDir;
	
	dicDir = temp+ _dicDir;
	if(dicDir.c_str() ==NULL)
		return -1;
	
	updateModel(); //update Dictionary if config object already exists 
	return 1;
}


int Ears::updateModel()
{
	//if init has not been called we don't need to restart pocketSphinx
	if(config == NULL)
		return 1;
	
	ps_free(ps); //free the decoder
	
	init(); //call init to load the decoder with the new language model.
	
}
void Ears::print_word_times()
{
    int frame_rate = cmd_ln_int32_r(config, "-frate");
    ps_seg_t *iter = ps_seg_iter(ps);
    while (iter != NULL) {
        int32 sf, ef, pprob;
        float conf;

        ps_seg_frames(iter, &sf, &ef);
        pprob = ps_seg_prob(iter, NULL, NULL, NULL);
        conf = logmath_exp(ps_get_logmath(ps), pprob);
        printf("%s %.3f %.3f %f\n", ps_seg_word(iter), ((float)sf / frame_rate),
               ((float) ef / frame_rate), conf);
        iter = ps_seg_next(iter);
    }
}
 int Ears::check_wav_header(char *header, int expected_sr)
{
    int sr;

    if (header[34] != 0x10) {
        E_ERROR("Input audio file has [%d] bits per sample instead of 16\n", header[34]);
        return 0;
    }
    if (header[20] != 0x1) {
        E_ERROR("Input audio file has compression [%d] and not required PCM\n", header[20]);
        return 0;
    }
    if (header[22] != 0x1) {
        E_ERROR("Input audio file has [%d] channels, expected single channel mono\n", header[22]);
        return 0;
    }
    sr = ((header[24] & 0xFF) | ((header[25] & 0xFF) << 8) | ((header[26] & 0xFF) << 16) | ((header[27] & 0xFF) << 24));
    if (sr != expected_sr) {
        E_ERROR("Input audio file has sample rate [%d], but decoder expects [%d]\n", sr, expected_sr);
        return 0;
    }
    return 1;
}

void Ears::prepare_ad(){
	
	cout << "preparing audio device...";
	if ((ad = ad_open_dev(m_hw.c_str(),
                          (int)cmd_ln_float32_r(config, "-samprate"))) == NULL)
        E_FATAL("Failed to open audio device\n");

    /* Initialize continuous listening module */
   
    if (ad_start_rec(ad) < 0)
        E_FATAL("Failed to start recording\n");
 if (ps_start_utt(ps) < 0)
        E_FATAL("Failed to start utterance\n");
    utt_started = FALSE;
	cout << "OK!\n";

    cout << "Ready....\n";
   
	
	
}

void Ears::InternalThreadEntry(){
		
		if(init()>0)
        {
                cerr<<"error on init"<<endl;
                exit(EXIT_FAILURE);
        }

       prepare_ad();
       
       while(1){
		   listen();
		   
	   }
	   close_ad();
}


std::string Ears::listen(){
	hyp=" ";hyp2=" ";
         if ((k = ad_read(ad, adbuf, 2048)) < 0)
            E_FATAL("Failed to read audio\n");
        ps_process_raw(ps, adbuf, k, FALSE, FALSE);
        in_speech = ps_get_in_speech(ps);
        if (in_speech && !utt_started) {
            utt_started = TRUE;
            cout<<"Listening...\n";
        }
        if (!in_speech && utt_started) {
            /* speech -> silence transition, time to start new utterance  */
            ps_end_utt(ps);
            hyp = ps_get_hyp(ps, NULL );
            if (hyp != NULL) {
		printf("%s\n", hyp);
		hyp2=std::string(hyp);

            }
            if (ps_start_utt(ps) < 0)
                E_FATAL("Failed to start utterance\n");

          utt_started = FALSE;
            cout<<"Ready....\n";
        }
        sleep_msec(100);

	return hyp2; 
	
}


void Ears::close_ad(){
	
    ad_close(ad);
}


void Ears::sleep_msec(int32 ms)
{
#if (defined(WIN32) && !defined(GNUWINCE)) || defined(_WIN32_WCE)
    Sleep(ms);
#else
    /* ------------------- Unix ------------------ */
    struct timeval tmo;

    tmo.tv_sec = 0;
    tmo.tv_usec = ms * 1000;

    select(0, NULL, NULL, NULL, &tmo);
#endif
}

//enter the location of the file to be interpreted. 
//returns the string that was recognized by the decoder
std::string Ears::rawToString(const char * pathToraw)
{
	int16 adbuf[2048];
    const char *fname;
    const char *hyp;
    int32 k;
    uint8 utt_started, in_speech;
    int32 print_times = cmd_ln_boolean_r(config, "-time");

    fname = cmd_ln_str_r(config, "-infile");
    if ((rawfd = fopen(fname, "rb")) == NULL) {
        E_FATAL_SYSTEM("Failed to open file '%s' for reading",
                       fname);
    }
    
    if (strlen(fname) > 4 && strcmp(fname + strlen(fname) - 4, ".wav") == 0) {
        char waveheader[44];
	fread(waveheader, 1, 44, rawfd);
	if (!check_wav_header(waveheader, (int)cmd_ln_float32_r(config, "-samprate")))
    	    E_FATAL("Failed to process file '%s' due to format mismatch.\n", fname);
    }

    if (strlen(fname) > 4 && strcmp(fname + strlen(fname) - 4, ".mp3") == 0) {
	E_FATAL("Can not decode mp3 files, convert input file to WAV 16kHz 16-bit mono before decoding.\n");
    }
    
    ps_start_utt(ps);
    utt_started = FALSE;

    while ((k = fread(adbuf, sizeof(int16), 2048, rawfd)) > 0) {
        ps_process_raw(ps, adbuf, k, FALSE, FALSE);
        in_speech = ps_get_in_speech(ps);
        if (in_speech && !utt_started) {
            utt_started = TRUE;
        } 
        if (!in_speech && utt_started) {
            ps_end_utt(ps);
            hyp = ps_get_hyp(ps, NULL);
            if (hyp != NULL)
        	printf("%s\n", hyp);
            if (print_times)
        	print_word_times();
            fflush(stdout);

            ps_start_utt(ps);
            utt_started = FALSE;
        }
    }
    ps_end_utt(ps);
    if (utt_started) {
        hyp = ps_get_hyp(ps, NULL);
        if (hyp != NULL) {
    	    printf("%s\n", hyp);
    	    if (print_times) {
    		print_word_times();
	    }
	}
    }
    
    fclose(rawfd);	
    return hyp;
}


//Method to record a raw audio file to outputAudioFile path and duration
void Ears::record(string outputAudioFile,string duration){

	system(("arecord -d "+duration+" -c 1 -f S16_LE -r 16000 "+ outputAudioFile).c_str());

}
