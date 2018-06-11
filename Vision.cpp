#include "Vision.h"

Vision::Vision()
{
	goVision();

	numPhotos=10;
	delay=100;
	setResolutionOutput(200,200);
	posCamX=127,posCamY=127;
	m_faceRecogInitOK=false;
	debugInImage=false;
	TrackerCompte = 0;
	
	
	 
	 compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
            compression_params.push_back(95);
            
            StartInternalThread();

}

Vision::~Vision()
{
    //dtor
}

void Vision::goVision(){

	//Configure Vision from marcel.cfg
	Myconf conf;

	//Cam config
	vector<int> parsecamInt = conf.getCamSettings();
	m_width=parsecamInt[0];
	m_height=parsecamInt[1];
	m_fps=parsecamInt[2];

	//Ocv int config
	vector<int> parseocvInt = conf.getOpenCVIntSettings();
	m_ocv_feature=parseocvInt[0];
	m_deviceId=parseocvInt[1];

	//Ocv string config
	vector<string> parseocvStr = conf.getOpenCVStrSettings(); 

	//Ocv bool config
	vector<bool> parseocvBool = conf.getOpenCVBoolSettings();
	displaynamedWindows=parseocvBool[0];

	cap.open(m_deviceId); 

	setParameters();
	setPaths(parseocvStr[0],parseocvStr[1],parseocvStr[2]
		,parseocvStr[3],parseocvStr[4],parseocvStr[5]);
	mjpgstreamerPath=parseocvStr[6];
	m_otPath=parseocvStr[7];
	setDetectionBasedTracker();
	setResolutionCapture(m_width,m_height);
	posTrack[0]=m_width/2;
	posTrack[1]=m_height/2;
	setFps(m_fps);
	setNamedWindows(displaynamedWindows);



}
void Vision::setParameters(){
       	// Declare the parameters that are needed for the tracker to run;
    	// All the parameters are mostly the same that you use for haar cascades.
	cout<<"init Parameters... ";

    	m_param.maxObjectSize = 400;    //Default = 400
    	m_param.maxTrackLifetime = 20;  //Default = 20
    	m_param.minDetectionPeriod = 7; //Default = 7
    	m_param.minNeighbors = 3;	      //Default = 3
    	m_param.minObjectSize = 20;     //Default = 20
    	m_param.scaleFactor = 1.1;      //Default = 1.1

	cout<<"OK!\n";

}

void Vision::setDetectionBasedTracker(){
     	cout<<"init Parameters... ";
     	obj = new DetectionBasedTracker(m_haarcascadePath,m_param);
	cout<<"OK!\n";

}

void Vision::setResolutionCapture(int width,int height){
	
	cout<<"init ResolutionCapture... ";

	m_width=width;
	m_height=height;

	cap.set(CV_CAP_PROP_FRAME_WIDTH,(double)m_width);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT,(double)m_height);

	double w,h;
	w=cap.get(CV_CAP_PROP_FRAME_WIDTH); 
	h=cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	cout <<"get cam width = "<<w <<"\n";
	cout <<"get cam height = "<<h <<"\n";
	cout<<"OK!\n";



}
void Vision::setFps(int fps){
	cout<<"init Fps... ";

	m_fps=fps;
	cout << "parameter m_fps =" << m_fps << "\n";

	cap.set(CV_CAP_PROP_FPS,(double)m_fps);

	cout<<"OK!\n";

}

void Vision::setNamedWindows(bool onOff){

    displaynamedWindows=onOff;
    if(displaynamedWindows){
        cv::namedWindow("Camera",cv::WINDOW_NORMAL);
        cv::moveWindow("Camera", 500,0);
   	}
}
void Vision::displayNamedWindows(Mat im){

 if(displaynamedWindows) cv::imshow("Camera",im);      // Show the results.
                if(cv::waitKey(33) == 27){
		obj->stop();
		exit(EXIT_SUCCESS);
 		}


}

void Vision::InternalThreadEntry(){
	sleep (2);  // Temps pour initialisation WebCam ATTENTION laisser a 2!!!

	 //Mjpg-stream fork depuis script command
        char *args[] = {mjpgstreamerPath.c_str(), (char *)0 };
        proc = new Processes();
        proc->forkExec(args);


	while(1){
		selectVisionFeature();		
	}
	
}

void Vision::selectVisionFeature(){

   switch(m_ocv_feature){
	
        case 0:           
			runCamStream();
        break;

        case 1:           
			runTrack();
        break;

        case 2:
            runTracknSave();
        break;

        case 3:
	    if(!m_faceRecogInitOK){
			initFaceRecognition();
           }
            runFaceRecognition();
		break;

        case 4:
            rgbContourEffet(0, 0);
		break;

        case 5:
             grayCannyEdge(0, 0);
        break;
        
        case 6:
           if(!m_objTrackInitOK){
			initObjectTrack();
           }
             runObjectTrack();
        break;
 
        default:
            cout << "bad argument \n";
			cout << "Can't launch Vision! \n";
        break;
	
    }
}

void Vision::runTracknSave(){

	if(TrackerCompte<1){
        TrackerCompte=2;
	n = scandir(m_csvPath.c_str(), &namelist, 0, alphasort);
        if (n < 0) perror("scandir");

    	folderCount=1;
   	for(int i=2;i<n;i++){

       		if(namelist[i]->d_type == DT_DIR) folderCount++;
	}

    	//char foldCnt[128];
            sprintf(foldCnt,"s%d",folderCount);
   	//char command[128];
            sprintf(command,"mkdir %s/%s",m_csvPath.c_str(),foldCnt);
            system(command);

        }

	bool isTracked=false;

   if(displaynamedWindows){
    cv::namedWindow("Result",cv::WINDOW_NORMAL);
	cv::moveWindow("Result", 100,0);}
	    obj->run();
        for(int counter=0;counter<numPhotos;counter++){
            while(!isTracked) {
                cap>>img;
                cv::cvtColor(img,gray,CV_RGB2GRAY);
             // The class object is run on a gray image.
                obj->process(gray);
               // The results are a vector of Rect that enclose the object being tracked
                obj->getObjects(faces);
                usleep(delay);
                
		// if(faces.size() == 0) obj.resetTracking();
                for (int i = 0; i < faces.size(); i++) {
                    face_i = faces[i];
                    // Make a rectangle around the detected object
                    //rectangle(img, face_i, CV_RGB(0, 255,0), 3);
                    string box_text = format("photo!");
                    //int pos_x = std::max(face_i.tl().x - 10, 0);
                    //int pos_y = std::max(face_i.tl().y - 10, 0);
                    // And now put it into the image:
                    putText(img, box_text, Point(20, 20), FONT_HERSHEY_SIMPLEX, 1.0, CV_RGB(0,255,0), 2.0);
                isTracked=true;
           }
		 if(!img.empty() && img.data) cv::imwrite(m_outputStreamFile, img, compression_params);
			displayNamedWindows(img);
            
            }
                imgCrop =gray(face_i).clone();

                resize(imgCrop,imgResize,imgResolutionOutput);

            if(displaynamedWindows)cv::imshow("Result",imgResize);
			char fileCnt[128];
            sprintf(fileCnt,"%d",counter);

         if (!imwrite((m_csvPath+"/"+foldCnt+"/"+fileCnt+m_extension),imgResize)){
               perror("imwrite error : ");
         }
        else {
            cout << "\n Image saved in : " << m_saveImagePath << "\n";
            isTracked=false;
        }
         usleep(delay);
      }
			writeCsvFaceRec(m_csvPath,m_csvFile);
            obj->stop();
	    m_ocv_feature=0;

}

void Vision::runTrack(){

     if(TrackerCompte<1) obj->run();
	TrackerCompte++;
                cap>>img;

		cv::cvtColor(img,gray,CV_RGB2GRAY);
             // The class object is run on a gray image.
                obj->process(gray);
               // The results are a vector of Rect that enclose the object being tracked
                obj->getObjects(faces);
                usleep(delay);
                // if(faces.size() == 0) obj.resetTracking();
             	posTrack[0]=m_width/2;
		posTrack[1]=m_height/2;
		 
		for (int i = 0; i < faces.size(); i++) {
                    face_i = faces[i];
                    // Make a rectangle around the detected object

                  rectangle(img, face_i, CV_RGB(0, 255,0), 3);
	          posTrack[0] = std::max((face_i.tl().x+ face_i.br().x)/2, 0);
        	  posTrack[1] = std::max((face_i.tl().y+ face_i.br().y)/2, 0);



		   }
    	         if(debugInImage){ 
			string box_text = format("X = %d Y=%d cX=%d cY=%d" ,posTrack[0],posTrack[1],posCamX,posCamY);
                  	putText(img, box_text, Point(m_width/2,m_height/4), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
		}
               displayNamedWindows(img);
		if(!img.empty() && img.data) cv::imwrite(m_outputStreamFile, img, compression_params);
                if(TrackerCompte>500){
                         //obj->stop();
                        TrackerCompte = 3;
                }
}

void Vision::setResolutionOutput(int w,int h){

    imgResolutionOutput = Size(w,h);

}

void Vision::setPaths(string haarcascadePath,string saveImagePath,string extension,string csvPath, string csvFile,string outputStreamFile){

   	cout<<"settingPaths... ";

    	m_haarcascadePath=haarcascadePath;
    	m_saveImagePath=saveImagePath;
    	m_extension=extension;
    	m_csvPath=csvPath;
    	m_csvFile=csvFile;
    	m_outputStreamFile=outputStreamFile;

	cout<<"OK!\n";
 
}

void Vision::readCsvFaceRec(string filename) {
    char separator=';';
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    string line, path, classlabel;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
    if(!path.empty() && !classlabel.empty()) {
            m_images.push_back(imread(path, 0));
            m_labels.push_back(atoi(classlabel.c_str()));
        }
    }
}
void Vision::writeCsvFaceRec(string folderPath,string filename){

    char separator=';';
    string subfolder;
    ofstream myfile;
    myfile.open ((folderPath+"/"+filename).c_str());

    int i,j;

    n = scandir(folderPath.c_str(), &namelist, 0, alphasort);
        if (n < 0) perror("scandir");

        for (i = 2; i < n; i++) {

            subfolder = folderPath+"/"+(string)namelist[i]->d_name;

            if(namelist[i]->d_type == DT_DIR){
                nsub = scandir(subfolder.c_str(), &subnamelist, 0, alphasort);
                for (j = 2; j < nsub; j++) myfile << subfolder+"/"+(string) subnamelist[j]->d_name << separator << i-3 << "\n" ;
            }
        }
    

 myfile.close();

}
void Vision::initFaceRecognition(){

    // Read in the data (fails if no valid input filename is given, but you'll get an error message):
    try {
        readCsvFaceRec(m_csvPath+"/"+m_csvFile);
    } catch (cv::Exception& e) {
        cout << "Error opening file \"" << m_csvPath << "\". Reason: " << e.msg << endl;
        // nothing more we can do
        exit(-1);
    }

    // Create a FaceRecognizer and train it on the given images:
    model = createFisherFaceRecognizer();
    model->train(m_images, m_labels);
    haar_cascade.load(m_haarcascadePath);
    // Check if we can use this device at all:
    if(!cap.isOpened()) {
        cerr << "Capture Device ID " << m_deviceId << "cannot be opened." << endl;
      exit(-1);
    }
	obj->run();
	m_faceRecogInitOK=true;
	prediction=-1;
	countRecog=0;
}
void Vision::runFaceRecognition(){

                cap>>img;
                cv::cvtColor(img,gray,CV_RGB2GRAY);
             // The class object is run on a gray image.
                obj->process(gray);
               // The results are a vector of Rect that enclose the object bei$
                obj->getObjects(faces);
		

	for(int i = 0; i < faces.size(); i++) {
            // Process face by face:
            Rect face_i = faces[i];
            // Crop the face from the image. So simple with OpenCV C++:
            Mat face = gray(face_i);
            // Resizing the face is necessary for Eigenfaces and Fisherfaces. You can easily
            // verify this, by reading through the face recognition tutorial coming with OpenCV.
            // Resizing IS NOT NEEDED for Local Binary Patterns Histograms, so preparing the
            // input data really depends on the algorithm used.
            //
            // I strongly encourage you to play around with the algorithms. See which work best
            // in your scenario, LBPH should always be a contender for robust face recognition.
            //
            // Since I am showing the Fisherfaces algorithm here, I also show how to resize the
            // face you have just found:
	
            Mat face_resized;
            cv::resize(face, face_resized, Size(m_images[0].cols,m_images[0].rows), 1.0, 1.0, INTER_CUBIC);
            // Now perform the prediction, see how easy that is:
            prediction = model->predict(face_resized);
            // And finally write all we've found out to the original image!
            // First of all draw a green rectangle around the detected face:
            rectangle(img, face_i, CV_RGB(0, 255,0), 1);
            // Create the text we will annotate the box with:
            string box_text = format("Pr = %d Nb=%d Fs=%d", prediction,countRecog,faces.size());
            // Calculate the position for annotated text (make sure we don't
            // put illegal values in there):
            int pos_x = std::max(face_i.tl().x - 10, 0);
            int pos_y = std::max(face_i.tl().y - 10, 0);
            // And now put it into the image:
            putText(img, box_text, Point(pos_x, pos_y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
            countRecog++;
	}
	 if(!img.empty() && img.data) cv::imwrite(m_outputStreamFile, img, compression_params);
	 displayNamedWindows(img);

	//Say Hello if a face is recognized
	if(prediction>=0 && countRecog>4){
		m_ocv_feature=0;
		countRecog=0;
		obj->stop();
		proc = new Processes();
		char *say="Bonjour Sebastien";
		char *arguments[] = {"/usr/bin/espeak", "-v", "mb-fr1", "-s" , "200", say, (char *)0 };
                proc->forkExec(arguments);
                delete proc;
	}

}
void Vision::runCamStream(){
	
        if(!cap.read(cam_mat)) std::cout << "No frame" << std::endl;
	cv::imwrite(m_outputStreamFile, cam_mat, compression_params);
	displayNamedWindows(cam_mat);
	
}

/** @function thresh_callback */
void Vision::rgbContourEffet(int, void* )
{

		Mat imgtmp;
                //if(!cam_mat.empty()) imgtmp =  cam_mat.clone();
               cap >> imgtmp;
		 cvtColor(imgtmp,gray,CV_RGB2GRAY);
		blur( gray, gray, Size(3,3) );
  Mat canny_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  RNG rng(12345);
        int thresh = 100;
        int max_thresh = 255;
  /// Detect edges using canny
  Canny(gray, canny_output, thresh, thresh*2, 3 );
  /// Find contours
  findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Draw contours
  //Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
	img = Mat::zeros( canny_output.size(), CV_16UC3); 
     for( int i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( img, contours, i, color, 2, 8, hierarchy, 0, Point() );
	//drawing.convertTo(img, CV_8UC3);
     }
	if(!img.empty() && img.data) cv::imwrite(m_outputStreamFile, img, compression_params);
	displayNamedWindows(img);
}

/** Fonction Canny Edge */
void Vision::grayCannyEdge(int, void* )
{	
	Mat imgtmp, imgedge;
       cap >> imgtmp;

        cvtColor(imgtmp, imgedge, CV_BGR2GRAY);
        Canny(imgedge, img, 30, 60);

	if(!img.empty() && img.data) cv::imwrite(m_outputStreamFile, img, compression_params);
	displayNamedWindows(img);
}
/************OjectTrack Function**********/
std::vector<std::vector<std::string> > Vision::readCsvObjTrack(string filename) {
    
    std::string separator=";";
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    string line, value;
    int pos=0;
    const int colSize =7;
    std::vector<string> col;
    std::vector<std::vector<string> > filelines;
    
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, value);
        for(int i=0;i<colSize;i++){
			pos=value.find(separator);
			col.push_back(value.substr(0,pos));
			value = value.substr(pos+separator.length());
		}
		
		filelines.push_back(col);
		col.resize(0);
	}
	/*
    for(int li=0;li<filelines.size();li++)
    {	for(int co=0;co<colSize;co++){
		
			cout << filelines[li][co] << "-";
		}
		cout <<"\n";
	}*/
	return filelines;
}


void Vision::initObjectTrack(){
	//members for object tracking
	MIN_OBJECT_AREA = 20*20;
	MAX_OBJECT_AREA = m_height*m_width/1.5;
	//create slider bars for HSV filtering
	//createTrackbars();
	m_objTrackParams = readCsvObjTrack(m_otPath);
	m_objTrackInitOK=true;
	m_otLine=0;
	m_otCounterMax=0;
	m_otCounter=m_otCounterMax;
	
	m_objectFound=false;
	m_otName="all";
	setObjTrackParams();
	cout << "Object track init OK\n";
}

void Vision::setObjTrackParams(){
	
	if(m_otCounter>=m_otCounterMax){
			
		m_otCounter=0;			
	
		if(m_otLine>0 && m_otLine<m_objTrackParams.size()){
			if(!m_objectFound){
				m_otName=m_objTrackParams[m_otLine][0];
				H_MIN=atoi(m_objTrackParams[m_otLine][1].c_str());
				H_MAX=atoi(m_objTrackParams[m_otLine][2].c_str());
				S_MIN=atoi(m_objTrackParams[m_otLine][3].c_str());
				S_MAX=atoi(m_objTrackParams[m_otLine][4].c_str());
				V_MIN=atoi(m_objTrackParams[m_otLine][5].c_str());
				V_MAX=atoi(m_objTrackParams[m_otLine][6].c_str());
				
				
				cout<<m_otLine<<"-"<<m_objTrackParams[m_otLine][0]<<"-"<<H_MIN<<"-"<<H_MAX<<"-"<<S_MIN<<"-"<<S_MAX<<"-"<<V_MIN<<"-"<<V_MAX<<"\n";
				cout.flush();
				m_otLine++;
			}
		}
		else m_otLine=1;
	
	}else m_otCounter++;
			
	
	
	
}

void Vision::runObjectTrack(){
	
		setObjTrackParams();
		//store image to matrix
		cap.read(cameraFeed);
		//convert frame from BGR to HSV colorspace
		cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
		//filter HSV image between values and store filtered image to
		//threshold matrix
		inRange(HSV,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX),threshold);
		//perform morphological operations on thresholded image to eliminate noise
		//and emphasize the filtered object(s)
		if(useMorphOps)
		morphOps(threshold);
		//pass in thresholded frame to our object tracking function
		//this function will return the x and y coordinates of the
		//filtered object
		if(trackObjects)
			trackFilteredObject(posTrack[0],posTrack[1],threshold,cameraFeed);
		
		
		//show frames 
		displayNamedWindows(cameraFeed);
		if(displaynamedWindows)	imshow(windowName2,threshold);
		if(!cameraFeed.empty() && cameraFeed.data) cv::imwrite(m_outputStreamFile, cameraFeed, compression_params);

}

void Vision::morphOps(Mat &thresh){
	//create structuring element that will be used to "dilate" and "erode" image.
	//the element chosen here is a 3px by 3px rectangle
	Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
    //dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement( MORPH_RECT,Size(8,8));
	erode(thresh,thresh,erodeElement);
	erode(thresh,thresh,erodeElement);
	dilate(thresh,thresh,dilateElement);
	dilate(thresh,thresh,dilateElement);
}

void Vision::trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed){

	Mat temp;
	threshold.copyTo(temp);
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//find contours of filtered image using openCV findContours function
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();
        //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
        if(numObjects<MAX_NUM_OBJECTS){
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {
				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;
				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				//we only want the object with the largest area so we safe a reference area each
				//iteration and compare it to the area in the next iteration.
                if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea){
					x = moment.m10/area;
					y = moment.m01/area;
					objectFound = true;
					refArea = area;
				}else {
					objectFound = false;
					if(m_otObjNotFoundCount<m_otObjNotFoundCountMax)
						m_otObjNotFoundCount++;
					else{ 
						m_otObjNotFoundCount=0;
						 m_objectFound=false;
						posTrack[0]=m_width/2;
						posTrack[1]=m_height/2;
						}
					}
			}
			
			
			//let user know you found an object
			if(objectFound ==true){
				m_objectFound=objectFound;
				putText(cameraFeed,"Tracking " +m_otName+" Object",Point(0,50),2,1,Scalar(0,255,0),2);
				//draw object location on screen
				drawObject(x,y,cameraFeed);}

		}else{ 
			posTrack[0]=m_width/2;
                        posTrack[1]=m_height/2;
		        putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
		}	
			
	}
}

void Vision::drawObject(int x, int y,Mat &frame){

	//use some of the openCV drawing functions to draw crosshairs
	//on your tracked image!

    //UPDATE:JUNE 18TH, 2013
    //added 'if' and 'else' statements to prevent
    //memory errors from writing off the screen (ie. (-25,-25) is not within the window!)

	circle(frame,Point(x,y),20,Scalar(0,255,0),2);
    if(y-25>0)
    line(frame,Point(x,y),Point(x,y-25),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(x,0),Scalar(0,255,0),2);
    if(y+25<m_height)
    line(frame,Point(x,y),Point(x,y+25),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(x,m_height),Scalar(0,255,0),2);
    if(x-25>0)
    line(frame,Point(x,y),Point(x-25,y),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(0,y),Scalar(0,255,0),2);
    if(x+25<m_width)
    line(frame,Point(x,y),Point(x+25,y),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(m_width,y),Scalar(0,255,0),2);

	putText(frame,intToString(x)+","+intToString(y),Point(x,y+30),1,1,Scalar(0,255,0),2);

}

string Vision::intToString(int number){


	std::stringstream ss;
	ss << number;
	return ss.str();
}

void Vision::createTrackbars(){
	//create window for trackbars


    namedWindow(trackbarWindowName,0);
	//create memory to store trackbar name on window
	char TrackbarName[50];
	sprintf( TrackbarName, "H_MIN", H_MIN);
	sprintf( TrackbarName, "H_MAX", H_MAX);
	sprintf( TrackbarName, "S_MIN", S_MIN);
	sprintf( TrackbarName, "S_MAX", S_MAX);
	sprintf( TrackbarName, "V_MIN", V_MIN);
	sprintf( TrackbarName, "V_MAX", V_MAX);
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH), 
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->      
    createTrackbar( "H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar );
    createTrackbar( "H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar );
    createTrackbar( "S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar );
    createTrackbar( "S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar );
    createTrackbar( "V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar );
    createTrackbar( "V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar );


}

void on_trackbar( int, void* )
{//This function gets called whenever a
	// trackbar position is changed

}


/*
void Vision::initFaceRecognition2(){

    // Read in the data (fails if no valid input filename is given, but you'll get an error message):
    try {
        readCsv(m_csvPath+"/"+m_csvFile);
    } catch (cv::Exception& e) {
        cout << "Error opening file \"" << m_csvPath << "\". Reason: " << e.msg << endl;
        // nothing more we can do
        exit(-1);
    }

    // Create a FaceRecognizer and train it on the given images:
    model = createLBPHFaceRecognizer();
    model->train(m_images, m_labels);
  //  haar_cascade.load(m_haarcascadePath);
    // Check if we can use this device at all:
    if(!cap.isOpened()) {
        cerr << "Capture Device ID " << m_deviceId << "cannot be opened." << endl;
      exit(-1);
    }
	obj->run();
	m_faceRecogInitOK=true;
	prediction=-1;
	countRecog=0;
}

void Vision::runFaceRecognition2(){



   
    int predictedLabel = model->predict(testSample);
    //
    // To get the confidence of a prediction call the model with:
    //
    //      int predictedLabel = -1;
    //      double confidence = 0.0;
    //      model->predict(testSample, predictedLabel, confidence);
    //
    string result_message = format("Predicted class = %d / Actual class = %d.", predictedLabel, testLabel);
    cout << result_message << endl;
    // Sometimes you'll need to get/set internal model data,
    // which isn't exposed by the public cv::FaceRecognizer.
    // Since each cv::FaceRecognizer is derived from a
    // cv::Algorithm, you can query the data.
    //
    // First we'll use it to set the threshold of the FaceRecognizer
    // to 0.0 without retraining the model. This can be useful if
    // you are evaluating the model:
    //
    model->set("threshold", 0.0);
    // Now the threshold of this model is set to 0.0. A prediction
    // now returns -1, as it's impossible to have a distance below
    // it
    predictedLabel = model->predict(testSample);
    cout << "Predicted class = " << predictedLabel << endl;
    // Show some informations about the model, as there's no cool
    // Model data to display as in Eigenfaces/Fisherfaces.
    // Due to efficiency reasons the LBP images are not stored
    // within the model:
    cout << "Model Information:" << endl;
    string model_info = format("\tLBPH(radius=%i, neighbors=%i, grid_x=%i, grid_y=%i, threshold=%.2f)",
            model->getInt("radius"),
            model->getInt("neighbors"),
            model->getInt("grid_x"),
            model->getInt("grid_y"),
            model->getDouble("threshold"));
    cout << model_info << endl;
    // We could get the histograms for example:
    vector<Mat> histograms = model->getMatVector("histograms");
    // But should I really visualize it? Probably the length is interesting:
    cout << "Size of the histograms: " << histograms[0].total() << endl;
    return 0;
}


*/
