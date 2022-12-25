#pragma once

#include <stdio.h>
#include <opencv2/opencv.hpp> 

namespace webcam {

typedef struct {
public:
	inline void Init(void)
	{
		// open the default camera using default API
    	cap.open(0);
    	// OR advance usage: select any API backend
    	//int deviceID = 0;             // 0 = open default camera
    	//int apiID = cv::CAP_ANY;      // 0 = autodetect default API
    	// open selected camera using selected API
    	//cap.open(deviceID, apiID);
    	// check if we succeeded
    	if (!cap.isOpened()) {
        	printf("ERROR! Unable to open camera\n");
        	return;
    	}

    	return;
	}

	void CaptureVideo(void);
	inline const void *data(void) const { return this->array.data();}
	inline size_t size(void) const { return this->array.size();} 
private:
	cv::Mat frame;
	cv::VideoCapture cap;
	std::vector<uchar> array;

} sampler_t;

}