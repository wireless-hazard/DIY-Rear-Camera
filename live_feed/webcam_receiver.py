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
		

def manipulate_video():




	for frame in video:
		frame = np.reshape(np.frombuffer(frame, np.uint8), (180,360))

		face_cascade = cv2.CascadeClassifier("/usr/local/share/opencv4/haarcascades/haarcascade_frontalface_default.xml")
		eyes_cascade = cv2.CascadeClassifier("/usr/local/share/opencv4/haarcascades/haarcascade_frontalface_default.xml")


		frame_gray = cv2.equalizeHist(frame)
		faces = face_cascade.detectMultiScale(frame_gray)

		for (x,y,w,h) in faces:
			center = (x + w//2, y + h//2)
			frame = cv2.ellipse(frame, center, (w//2, h//2), 0, 0, 360, (255, 0, 255), 4)
			faceROI = frame_gray[y:y+h,x:x+w]
			#-- In each face, detect eyes
			eyes = eyes_cascade.detectMultiScale(faceROI)
			for (x2,y2,w2,h2) in eyes:
				eye_center = (x + x2 + w2//2, y + y2 + h2//2)
				radius = int(round((w2 + h2)*0.5))
				frame = cv2.circle(frame, eye_center, radius, (255, 0, 0 ), 4)

		cv2.imshow("Recorded Video", frame)
		cv2.waitKey(10)


def main(host_ip):
	fixed_amount_reading(host_ip)
	manipulate_video()
	


if __name__ == '__main__':
	n = len(sys.argv)
	if (n == 2):
		main(host_ip=sys.argv[1])
	else:
		print("You have to pass a valid IP Address!")