#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "webcam.hpp"

constexpr static int PORT{8000};
static int listen_sock;

static void socket_listen(void)
{
    int addr_family{AF_INET};
    int ip_protocol{IPPROTO_IP};

    int nodelay = 1;    

    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(PORT);

	listen_sock = socket(addr_family, SOCK_STREAM, ip_protocol);
    // setsockopt(listen_sock, IPPROTO_TCP, TCP_NODELAY, (void *)&nodelay, sizeof(int));
    setsockopt(listen_sock, IPPROTO_TCP, SO_REUSEADDR, reinterpret_cast<void *>(&nodelay), sizeof(int));
    if (listen_sock < 0) {
        printf("Unable to create socket: errno %s\n", strerror(errno));
        abort();
    }
    printf("Socket created\n");

    int err = bind(listen_sock, reinterpret_cast<struct sockaddr *>(&dest_addr), sizeof(dest_addr));
    if (err != 0) {
        printf("Socket unable to bind: %s\n", strerror(errno));
        abort();
    }
    printf("Socket bound, port %d\n", PORT);

    err = listen(listen_sock, 1);
    if (err != 0) {
        printf("Error during listening phase: %s\n", strerror(errno));
        abort();
    }
    printf("Socket listening\n");
    sleep(2);
}

static void accept_connection(void)
{
	struct sockaddr_in6 source_addr; // Large enough for both IPv4 or IPv6
    uint addr_len = sizeof(source_addr);
    listen_sock = accept(listen_sock, reinterpret_cast<struct sockaddr *>(&source_addr), &addr_len);

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

int main() 
{ 
    webcam::sampler_t local_cam;

	socket_listen();

	accept_connection();

    local_cam.Init();

    int send_err;

	do {

        local_cam.CaptureVideo();
        send_err = send(listen_sock, local_cam.data(), local_cam.size(), 0);

	} while(send_err != -1);

    printf("Unable to send the message: errno: %s\n", strerror(errno));

	shutdown(listen_sock, 0);
    close(listen_sock);
	
	return 0;
}