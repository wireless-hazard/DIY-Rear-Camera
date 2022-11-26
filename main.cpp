#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp> 

constexpr static int PORT{8000};
static std::vector<uchar> array;
static int listen_sock;

static void socket_listen(void)
{
    std::string rx_buffer;
    int addr_family;
    int ip_protocol;

    int nodelay = 1;    

    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(PORT);
    addr_family = AF_INET;
    ip_protocol = IPPROTO_IP;

	listen_sock = socket(addr_family, SOCK_STREAM, ip_protocol);
    // setsockopt(listen_sock, IPPROTO_TCP, TCP_NODELAY, (void *)&nodelay, sizeof(int));
    setsockopt(listen_sock, IPPROTO_TCP, SO_REUSEADDR, (void *)&nodelay, sizeof(int));
    if (listen_sock < 0) {
        printf("Unable to create socket: errno %s\n", strerror(errno));
        abort();
    }
    printf("Socket created\n");

    int err = bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err != 0) {
        printf("Socket unable to bind: errno %s\n", strerror(errno));
        abort();
    }
    printf("Socket bound, port %d\n", PORT);

    err = listen(listen_sock, 1);
    if (err != 0) {
        printf("Error occurred during listen: errno %s\n", strerror(errno));
        abort();
    }
    printf("Socket listening\n");
    sleep(2);
}

void accept_connection(void)
{
	struct sockaddr_in6 source_addr; // Large enough for both IPv4 or IPv6
    uint addr_len = sizeof(source_addr);
    listen_sock = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);

    if (listen_sock < 0) 
    {
    	printf("Unable to accept connection: errno: %s\n", strerror(errno));
        abort();
    }
    printf("Socket accepted\n");
    #if 0
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
    #endif
}

static void webcam_capturer(void){
	cv::Mat frame;
	//--- INITIALIZE VIDEOCAPTURE
    cv::VideoCapture cap;
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
		cv::resize(frame,frame,cv::Size(360, 180), cv::INTER_LINEAR);

        // Turn the frame into gray scale (check size X performance)
        cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
        std::cout<<frame.size()<<"\n";

        // show live and wait for a key with timeout long enough to show images
        // cv::imshow("Live", frame);
        // cv::waitKey(0);        
    }
    // the camera will be deinitialized automatically in VideoCapture destructor

	if (frame.isContinuous()) {
  		array.assign(frame.data, frame.data + frame.total()*frame.channels());
	}
	printf("size of vector: %lu\n", array.size());
	
	return;
}
 
int main() 
{ 
	socket_listen();

	accept_connection();


	while(true)
	{
		webcam_capturer();

		int send_err = send(listen_sock, array.data(), array.size(), 0);
    	printf("Unable to send the message: errno: %s\n", strerror(errno));

		if(send_err == -1)break;
	}
	shutdown(listen_sock, 0);
    close(listen_sock);
	
	return 0;
}