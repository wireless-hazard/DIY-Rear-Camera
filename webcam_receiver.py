import socket
import cv2, time
import numpy as np

HOST_IP = "192.168.5.110"
HOST_PORT = 8000

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
	s.connect((HOST_IP, HOST_PORT))
	while(True):
		data = s.recv(64800, socket.MSG_WAITALL)
		print(len(data))
		if not data:
			break;
		#This converts the data (string) received from the socket into an OpenCV image:
		image = np.reshape(np.frombuffer(data, np.uint8), (180,360))

		cv2.imshow("WindowNameHere", image)
		cv2.waitKey(1000)