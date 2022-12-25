import sys, getopt, socket
import cv2, time
import numpy as np

HOST_PORT = 8000

def main(host_ip):
	with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
		s.connect((host_ip, HOST_PORT))
		while(True):
			data = s.recv(64800, socket.MSG_WAITALL)
			#print(len(data))
			if not data:
				break;
			#This converts the data (string) received from the socket into an OpenCV image:
			image = np.reshape(np.frombuffer(data, np.uint8), (180,360))

			cv2.imshow("Live feed", image)
			cv2.waitKey(10)


if __name__ == '__main__':
	n = len(sys.argv)
	if (n == 2):
		main(host_ip=sys.argv[1])
	else:
		print("You have to pass a valid IP Address!")