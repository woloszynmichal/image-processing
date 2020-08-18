from picamera.array import PiRGBArray
import RPi.GPIO as GPIO
import time
import cv2
import picamera
import numpy as np

# Inicjalizacja kamery
camera = picamera.PiCamera()
camera.resolution = (192,112)
camera.framerate = 10
rawCapture = PiRGBArray(camera,size=(192,112))
time.sleep(0.1)

# piny GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup (21, GPIO.OUT)
GPIO.setup (20, GPIO.OUT)
GPIO.setup (16, GPIO.OUT)
GPIO.setup (12, GPIO.OUT)
GPIO.setup (26, GPIO.OUT)
GPIO.setup (19, GPIO.OUT)
GPIO.setup (13, GPIO.OUT)
GPIO.setup (6, GPIO.OUT)


for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port$

        image = frame.array
        #cv2.imshow('img',image)
        image = cv2.flip(image,-1)
        cv2.imshow('image',image)
        key = cv2.waitKey(1) & 0xFF
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        blur = cv2.GaussianBlur(gray,(5,5),0)
        ret,thresh1 = cv2.threshold(blur,100,255,cv2.THRESH_BINARY_INV)
        cv2.imshow('tresh',thresh1)

        mask = cv2.erode(thresh1, None, iterations=2)
        mask = cv2.dilate(mask, None, iterations=2)

        mask_img=mask.copy()
        contours, hierarchy = cv2.findContours(mask_img,1,cv2.CHAIN_APPROX_NONE)

        #cv2.imshow('contours',mask_img)

        if len(contours)>0:
            GPIO.output(21,True)
        else:
            GPIO.output(21,False)

        c = max(contours, key = cv2.contourArea)
        M = cv2.moments(c)
        cx = int(M['m10']/M['m00'])

        xval=(127*cx)/192

        a = format(xval, '#09b')
        print(a)
        GPIO.output(20,int(a[2]))
        GPIO.output(16,int(a[3]))
        GPIO.output(12,int(a[4]))
        GPIO.output(26,int(a[5]))
        GPIO.output(19,int(a[6]))
        GPIO.output(13,int(a[7]))
        GPIO.output(6 ,int(a[8]))

        if key == ord("q"):
            break

        rawCapture.truncate(0)


GPIO.cleanup()
