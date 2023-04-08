#pragma once

#include <stdio.h>
#include <opencv2/opencv.hpp> 
#include <optional>

namespace webcam {

typedef struct {
public:
	inline void Init(std::optional<std::string> cam_path = {})
	{
		// open the default camera using default API if no value was passed
		if (cam_path.has_value())
		{
			cap.open(cam_path.value());
		}
		else
		{
			cap.open(0);
		}
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
	cv::Mat CaptureFrame(bool show_frame=false);
	inline const void *data(void) const { return this->array.data();}
	inline size_t size(void) const { return this->array.size();} 
private:
	cv::Mat frame;
	cv::VideoCapture cap;
	std::vector<uchar> array;

} sampler_t;

}