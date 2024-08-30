import cv2
import numpy as np
import matplotlib.pyplot as plt

image = cv2.imread('plot_image30.png')
output = image.copy()
height, width = image.shape[:2]
maxRadius = int(width/10)
minRadius = int(width/20)

gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
circles = cv2.HoughCircles(image=gray, 
                           method=cv2.HOUGH_GRADIENT, 
                           dp=1.05, 
                           minDist=400,
                           param1=60,
                           param2=30,
                           minRadius=10,
                           maxRadius= 80                         
                          )

if circles is not None:
    # convert the (x, y) coordinates and radius of the circles to integers
    circlesRound = np.round(circles[0, :]).astype("int")
    # loop over the (x, y) coordinates and radius of the circles
    for (x, y, r) in circlesRound:
        cv2.circle(output, (x, y), r, (0, 255, 0), 4)

    plt.imshow(output)
    plt.savefig('vision.png')
else:
    print ('No circles found')
