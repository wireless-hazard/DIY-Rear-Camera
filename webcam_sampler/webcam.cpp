#include "webcam.hpp"

void webcam::sampler_t::CaptureVideo(void)
{
	//--- GRAB AND WRITE LOOP

    // wait for a new frame from camera and store it into 'frame'
    cap.read(this->frame);
    // check if we succeeded
    if (this->frame.empty()) {
        printf("ERROR! blank frame grabbed\n");
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
    
    if (frame.isContinuous()) {
  		this->array.assign(this->frame.data, this->frame.data + (this->frame.total()) * (this->frame.channels()) );
	}
    #if DEBUG
    printf("size of vector: %lu\n", array.size());
    #endif
	
	return;
}

cv::Mat webcam::sampler_t::CaptureFrame(bool show_frame)
{
	cv::Mat local_frame;
	/* wait for a new frame from camera and store it into 'frame'
	At each reading, it fills (for whatever reason, 5 frames, so
	I'm discarding the other 4)*/
    cap.read(local_frame); /*Reading 1*/
    cap.read(local_frame); /*Reading 2*/
    cap.read(local_frame); /*Reading 3*/
    cap.read(local_frame); /*Reading 4*/
    cap.read(local_frame); /*Reading 5 - Frame returned*/
    // check if we succeeded
    if (local_frame.empty()) {
        printf("ERROR! blank frame grabbed\n");
    }

    if (show_frame)
    {
    	cv::imshow("Live", local_frame);
    	cv::waitKey(0); 	
    }

    return local_frame;
}