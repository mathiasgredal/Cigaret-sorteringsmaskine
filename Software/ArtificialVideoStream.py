import numpy as np
import cv2
from threading import Thread
import time
import random

import PIL

CmPrSek = 2
PxPrCm = 80


class ArtificialVideoStream:
    def __init__(self):
        self.butts = []
        self.startTime = time.time()

        self.stopthread = False

        self.cigImg = PIL.Image.open("cigerrette.jpg")

        Thread(target=self.update, args=()).start()

    def start(self):
        return self, True

    def update(self):
        global CmPrSek
        while not self.stopthread:
            distanceToNextButt = random.uniform(2.5, 3.0)
            self.butts.append(np.sum(self.butts)+distanceToNextButt)
            timeToGenerateNextButt = distanceToNextButt/CmPrSek

            time.sleep(timeToGenerateNextButt)

        return



    def read(self):
        timeSinceBeginning = time.time() - self.startTime
        distSinceBeginning = timeSinceBeginning/CmPrSek
        buttsRelativePosition = [(x - distSinceBeginning)*PxPrCm for x in self.butts]

        relevantButts =  [x for x in buttsRelativePosition if x<=640 and x > -200]
        background = PIL.Image.new('RGBA', (640, 480), (255, 255, 255, 255))
        buttwidth = 180
        buttheight = 55

        y = (480+buttheight)*0.5

        for butt in relevantButts:
            # Generate XY coords for img
            background.paste(self.cigImg, (int(butt),int(y)))

        npImg = np.array(background)
        meanColor = np.mean(npImg[:,0: 1])
        return (meanColor, npImg, True)


    def stop(self):
        # indicate that the thread should be stopped
        self.stopthread = True
