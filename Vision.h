#ifndef VISION_H
#define VISION_H

#include "opencv2/contrib/detection_based_tracker.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/core/mat.hpp"
#include "opencv2/core/operations.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"
#include "opencv2/objdetect/objdetect.hpp"
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>// For POSIX threads
#include "Processes.h"
#include "Myconf.h"
#include "Mythread.h"
#include "Singleton.h"

using namespace std;
using namespace cv;

class Vision: public MyThread,public Singleton<Vision>
{
	friend class Singleton<Vision>;

    private:
        Vision();
        virtual ~Vision();
	int _value;
    public:

	 // Public Interface for Singleton
	  void setValue (int val) { _value = val; }
	  int getValue () { return _value; }
        

	Processes *proc;

        vector< Rect_<int> > faces;
        Mat img,gray,imgResize,imgCrop;
        Rect_<int> face_i;

	Mat src_gray, cam_mat;
	vector<int> compression_params;
        int thresh;
        int max_thresh;

        int cvpipeinput[2], cvpipeoutput[2];
        int status;
        size_t matSizeByteInput, matSizeByteOutput;
	int TrackerCompte;
        bool displaynamedWindows;
	bool debugInImage;
        int posTrack[2];
	int posCamX,posCamY;

	int m_width,m_height,m_fps,m_ocv_feature;
        int numPhotos,imgResolutionInputWidth,imgResolutionInputHeight,m_deviceId;
        unsigned int delay;
        VideoCapture cap;

        vector<Mat> m_images;
        vector<int> m_labels;
	Ptr<FaceRecognizer> model;
	CascadeClassifier haar_cascade;
	bool  m_faceRecogInitOK;
	int prediction,countRecog; // for face recognition
	
	
	
	/*********ObjTrackParams************/
	//Path for ObjTrackParams.csv file 
	std::string m_otPath;
	//HSV Settings
	int H_MIN = 83;
	int H_MAX = 256;
	int S_MIN = 124;
	int S_MAX = 256;
	int V_MIN = 150;
	int V_MAX = 256; 
	//max number of objects to be detected in frame
	int MAX_NUM_OBJECTS=50;
	//minimum and maximum object area
	int MIN_OBJECT_AREA;
	int MAX_OBJECT_AREA;
	//some boolean variables for different functionality within this program
	bool trackObjects = true;
	bool useMorphOps = false;
		//Matrix to store each frame of the webcam feed
	Mat cameraFeed;
	//matrix storage for HSV image
	Mat HSV;
	//matrix storage for binary threshold image
	Mat threshold;
	//named windows names
	const string trackbarWindowName = "Trackbars";
	const string windowName2 = "Thresholded Image";
	//object Track Initialization flag 
	bool m_objTrackInitOK=false;
	//2D vector from csvfile ObjTrackParams
	std::vector<std::vector<std::string> > m_objTrackParams;
	//line id in csv file
	int m_otLine;
	int m_otCounter,m_otCounterMax;
	int m_otObjNotFoundCount=0,m_otObjNotFoundCountMax=50;
	std::string m_otName;
	bool m_objectFound;

	
	/*********FaceTrack Members******/

        Size imgResolutionOutput;

        string m_haarcascadePath,m_saveImagePath,m_extension,m_csvPath,m_csvFile,m_outputStreamFile;
	string mjpgstreamerPath;

        DetectionBasedTracker::Parameters m_param;
        DetectionBasedTracker *obj;

	struct dirent **namelist;
        struct dirent **subnamelist;
        int n,nsub; //number of elements in imagepath;
        
	void goVision();
	void setParameters();
        void setDetectionBasedTracker();
        void setResolutionCapture(int width,int height);
        void setFps(int fps);
	void setNamedWindows(bool onOff);
	void displayNamedWindows(Mat im);
        void setResolutionOutput(int w,int h);
        void setPaths(string,string,string,string,string,string);
	void readCsvFaceRec(string filename);
	void writeCsvFaceRec(string folderPath,string filename);
	void InternalThreadEntry();
	void selectVisionFeature();
	void runCamStream();
        void runTracknSave();
        void runTrack();
	void initFaceRecognition();
        void runFaceRecognition();
       // void initFaceRecognition2();
       // void runFaceRecognition2();
    void rgbContourEffet(int, void*);
	void grayCannyEdge(int, void*);
	/******Object Tracking Methods********/
	std::vector<std::vector<std::string> > readCsvObjTrack(string filename);
	void initObjectTrack();	
	void setObjTrackParams();
	void runObjectTrack();
	void morphOps(Mat &thresh);
	void trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed);
	void drawObject(int x, int y,Mat &frame);
	std::string intToString(int number);
	void createTrackbars();
	
        protected:

        private:
	        char foldCnt[128];
	        char command[128];
		int  folderCount;
      
};
void on_trackbar( int, void* );

#endif // VISION_H
