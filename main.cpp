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
 
int main() 
{ 
	// Read the image file 
	cv::Mat image = cv::imread("/home/magno/Pictures/highway.jpg");
	cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
	cv::imshow("Display Image", image);
	cv::waitKey(0);

	task_open_socket();


	return 0; 
}