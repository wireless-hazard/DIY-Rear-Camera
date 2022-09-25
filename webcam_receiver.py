import socket
import cv2
import numpy as np

HOST_IP = "127.0.0.1"
HOST_PORT = 8000

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
	s.connect((HOST_IP, HOST_PORT))
	data = s.recv(64800)

print(len(data))
#This converts the data (string) received from the socket into an OpenCV image:
image = np.reshape(np.frombuffer(data, np.uint8), (180,360))

cv2.imshow("WindowNameHere", image)
cv2.waitKey(0)