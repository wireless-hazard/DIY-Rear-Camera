import cv2
import numpy as np
import glob

frameSize = (640, 480)

out = cv2.VideoWriter('output_video.avi',cv2.VideoWriter_fourcc(*'DIVX'), 10, frameSize)

for filename in sorted(glob.glob('photos/*.png')):
	print(filename)
	img = cv2.imread(filename)
	out.write(img)

out.release()