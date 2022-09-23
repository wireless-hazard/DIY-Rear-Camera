#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <opencv2/opencv.hpp> 

constexpr static int PORT{8000};

static void task_open_socket(void){

    std::string rx_buffer;
    int addr_family;
    int ip_protocol;
	int sock;

    int nodelay = 1;
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void *)&nodelay, sizeof(int));

    while (true) {

        struct sockaddr_in dest_addr;
        dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(PORT);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_IP;

        int listen_sock = socket(addr_family, SOCK_STREAM, ip_protocol);
        if (listen_sock < 0) {
            printf("Unable to create socket: errno %s\n", strerror(errno));
            break;
        }
        printf("Socket created\n");

        int err = bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        if (err != 0) {
            printf("Socket unable to bind: errno %s\n", strerror(errno));
            break;
        }
        printf("Socket bound, port %d\n", PORT);

        err = listen(listen_sock, 1);
        if (err != 0) {
            printf("Error occurred during listen: errno %s\n", strerror(errno));
            break;
        }
        printf("Socket listening\n");
        sleep(2);
        while (true) {
            struct sockaddr_in6 source_addr; // Large enough for both IPv4 or IPv6
            uint addr_len = sizeof(source_addr);
            sock = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);

            if (sock < 0) {
                printf("Unable to accept connection: errno: %s\n", strerror(errno));
                break;
            }
            printf("Socket accepted\n");
            while(true){
                int len = recv(sock, &rx_buffer[0], 1024, 0);
                // Error occurred during receiving
                if (len < 0) {
                    printf("recv failed: %s\n", strerror(errno));
                    break;
                }else if (len == 0) {// Connection closed
                    printf("Connection closed\n");
                    break;
                }else {// Data received
                    // Get the sender's ip address as string
                	printf("%.*s", len, rx_buffer.c_str());
                }
            }
            if (sock != -1) {
                shutdown(sock, 0);
                close(sock);
            }
        }
    }
}

static void webcam_capturer(void){
	cv::Mat frame;
	//--- INITIALIZE VIDEOCAPTURE
    cv::VideoCapture cap;
    // open the default camera using default API
    // cap.open(0);
    // OR advance usage: select any API backend
    int deviceID = 0;             // 0 = open default camera
    int apiID = cv::CAP_ANY;      // 0 = autodetect default API
    // open selected camera using selected API
    cap.open(deviceID, apiID);
    // check if we succeeded
    if (!cap.isOpened()) {
        printf("ERROR! Unable to open camera\n");
        return;
    }

    //--- GRAB AND WRITE LOOP
    printf("Start grabbing\nPress any key to terminate\n");

    // for (;;)
    {
        // wait for a new frame from camera and store it into 'frame'
        cap.read(frame);
        // check if we succeeded
        if (frame.empty()) {
            printf("ERROR! blank frame grabbed\n");
            // break;
        }
        // Turn the frame into gray scale (check size X performance)
        cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);

        // show live and wait for a key with timeout long enough to show images
        cv::imshow("Live", frame);
        cv::waitKey(5);        
    }
    // the camera will be deinitialized automatically in VideoCapture destructor

	std::vector<uchar> array;
	if (frame.isContinuous()) {
  		array.assign(frame.data, frame.data + frame.total()*frame.channels());
	}
	printf("size of vector: %lu\n", array.size());
	
	return;
}
 
int main() 
{ 
	webcam_capturer();
	
	// task_open_socket();

	return 0; 
}