import numpy as np
import cv2
from threading import Thread
from time import sleep



class VideoStream:
    def __init__(self, src):
        # initialize the video camera stream and read the first frame
        # from the stream
        self.stream = cv2.VideoCapture(src)
        (self.grabbed, self.frame) = self.stream.read()

        self.stopthread = False

    def start(self):
        # start the thread to read frames from the video stream
        if(self.stream.isOpened()):
            print("Rtsp connection established")
            Thread(target=self.update, args=()).start()
            return(self, True)
        else:
            print("Couldn't start the rtsp stream")
            return self, False
 
    def update(self):
        # keep looping infinitely until the thread is stopped
        while self.stream.isOpened():
            sleep(0.05)
            # if the thread indicator variable is set, stop the thread
            if self.stopthread:
                print("stream has stopped")
                break
 
            # otherwise, read the next frame from the stream
            (self.grabbed, self.frame) = self.stream.read()

            if(self.frame is None):
                # End of stream reached
                break

        print("releasing rtsp stream")
        self.stream.release()
        return
 
    def read(self):
        # return the frame most recently read and resize it
        if self.frame is None:
            return (0, None, False)
        else:
            rgbFrame = cv2.cvtColor(self.frame,cv2.COLOR_BGR2RGB)
            meanColor = np.mean(rgbFrame[:,0: 1])
            return (meanColor, rgbFrame, self.grabbed)
 
    def stop(self):
        # indicate that the thread should be stopped
        self.stopthread = True
