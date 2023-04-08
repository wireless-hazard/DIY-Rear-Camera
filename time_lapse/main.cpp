#include <chrono>
#include <stdint.h>
#include <stdio.h>
#include <thread>
#include "webcam.hpp"

static constexpr uint32_t NUMBER_OF_SAMPLES = 100U;

int main()
{
	webcam::sampler_t camera;

	/*Pass the path to your webcam. Can be found at /dev/v4l/by-id */
	camera.Init("/dev/v4l/by-id/usb-GENERAL_GENERAL_WEBCAM_JH0319_20200710_v012-video-index0");
	//camera.Init();

	for (uint32_t i = 0; i < NUMBER_OF_SAMPLES; i++)
	{
		std::this_thread::sleep_for(std::chrono::seconds(10));

		auto frame = camera.CaptureFrame();
		auto system_time_chrono = std::chrono::system_clock::now();
		std::time_t system_time = std::chrono::system_clock::to_time_t(system_time_chrono);

		std::string system_time_string = std::ctime(&system_time);
		std::string file_name = std::string{"../photos/ScreenCap_"} + system_time_string + std::string{".png"};
		cv::imwrite(file_name, frame);

	}
}