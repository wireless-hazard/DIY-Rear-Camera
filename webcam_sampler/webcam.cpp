#include "webcam.hpp"

void webcam::sampler_t::CaptureVideo(void)
{
	//--- GRAB AND WRITE LOOP
    //printf("Start grabbing\nPress any key to terminate\n");

    // for (;;)
    {
        // wait for a new frame from camera and store it into 'frame'
        cap.read(this->frame);
        // check if we succeeded
        if (this->frame.empty()) {
            printf("ERROR! blank frame grabbed\n");
            // break;
        }
		cv::resize(this->frame,this->frame,cv::Size(360, 180), cv::INTER_LINEAR);

        // Turn the frame into gray scale (check size X performance)
        cv::cvtColor(this->frame, this->frame, cv::COLOR_BGR2GRAY);
        #if DEBUG
        std::cout<<this->frame.size()<<"\n";
        #endif

        // show live and wait for a key with timeout long enough to show images
        // cv::imshow("Live", frame);
        // cv::waitKey(0);        
    }
    // the camera will be deinitialized automatically in VideoCapture destructor

	if (frame.isContinuous()) {
  		this->array.assign(this->frame.data, this->frame.data + (this->frame.total()) * (this->frame.channels()) );
	}
	//printf("size of vector: %lu\n", array.size());
	
	return;
}