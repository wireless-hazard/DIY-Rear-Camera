import sys, getopt, socket
import cv2, time
import numpy as np

HOST_PORT = 8000
NUMBER_OF_FRAMES = 24*3

video=[]

def infinite_reading(host_ip):
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

def fixed_amount_reading(host_ip):
	with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
		s.connect((host_ip, HOST_PORT))
		for x in range(0,NUMBER_OF_FRAMES):
			data = s.recv(64800, socket.MSG_WAITALL)
			if not data:
				break;
			video.append(data)
			
	out = cv2.VideoWriter("camera.avi", fourcc=cv2.VideoWriter_fourcc(*"DIVX"), fps=24, frameSize=(180,360),isColor=True)

	for image in video:
		image = np.reshape(np.frombuffer(image, np.uint8), (180,360))
		print(type(image))
		cv2.imshow("Recorded Video", image)
		cv2.waitKey(10)
		#out.write(image)
	out.release()

def manipulate_video():
	print(type(video))
	print(np.size(video[0]))
	print(type(video[0]))

def main(host_ip):
	fixed_amount_reading(host_ip)
	manipulate_video()
	


if __name__ == '__main__':
	n = len(sys.argv)
	if (n == 2):
		main(host_ip=sys.argv[1])
	else:
		print("You have to pass a valid IP Address!")