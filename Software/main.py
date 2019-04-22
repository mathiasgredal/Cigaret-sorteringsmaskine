import numpy as np
import PIL.Image, PIL.ImageTk
from tkinter import *
from tkinter import ttk
import arduinoserial
import serial.tools.list_ports
from VideoStreamHelper import VideoStream
from ArtificialVideoStream import ArtificialVideoStream
import time

import os
import sys

enableNeuralNetwork = True

if enableNeuralNetwork:
    import skimage.io
    from skimage.draw import line
    import tensorflow as tf
    from mrcnn import utils
    import mrcnn.model as modellib
    from mrcnn.config import Config

    class CocoConfig(Config):
        NAME = "coco"
        IMAGES_PER_GPU = 1
        NUM_CLASSES = 1 + 1

    class InferenceConfig(CocoConfig):
        GPU_COUNT = 1
        IMAGES_PER_GPU = 1

    os.environ['CUDA_VISIBLE_DEVICES'] = '-1'

    config = InferenceConfig()
    config.display()

    model = modellib.MaskRCNN(mode="inference", model_dir="./logs", config=config)

    model.load_weights("../mask_rcnn_balloon_0030.h5", by_name=True)

    class_names = ['BG', 'Cigerette']


root = Tk()
root.winfo_toplevel().title("Cigarettæller maskine")

#### Trigger Variables
ArduinoBaudRate = StringVar(root)
ArduinoBaudRate.set('Baud Rate')
ArduinoCOMPort = StringVar(root)
ArduinoCOMPort.set('Serial Port')
RTSPURL = StringVar(root)
#RTSPURL.set("rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov")
#RTSPURL.set("file:///Users/mathiasgredal/Downloads/IMG_3358.TRIM.MOV")
RTSPURL.set("Artificial VideoStream")

#### Buttons
buttonregister = {
    "ArduinoConnect": False,
    "RTSPConnect": False,
    "RTSPDisconnect": False,
    "WindowClose": False,
    "ArduinoPush": False,
    "StartConveyor": False,
    "StopConveyor": False,
    "TestScan": False,
    "OverwriteCamTrigger": False
}
def SetButtonAsPressed(name):
    buttonregister[name] = True

#### Scanning Variables
XPlotWidth = 10
plotStartTime = 0

PushingQueue = []
NextCollisionTime = 0

CmPrSek = 2
PxPrCm = 80
DistCamToPusher = 3

Scanning = IntVar()
CalibrationHelp = True
connectedToRTSP = False
sensitivityThresshold = 100
def SetBrightnessSensitivity(val):
    global sensitivityThresshold
    sensitivityThresshold = val

plotX = np.array([0.0])
plotY = np.array([0.0])

def ScanImage(image):

    #results = model.detect([image], verbose=1)
    #n = results[0]['masks'].shape[2]

    #cigeretteLoc = []
    #cigeretteEnd = []

    #for i in range(0,n):
    #    mask = results[0]['masks'][:,:,i]
    #    xmax, ymax = np.max(np.where(mask == True), 1)
    #    xmin, ymin = np.min(np.where(mask == True), 1)

    #    cigeretteLoc.append(int((ymax+ymin)/2))
    #    cigeretteEnd.append(ymax)

    #avg = np.min(np.array(cigeretteLoc))
    #end = cigeretteEnd[np.argmin(np.array(cigeretteLoc))]

    avg = 1
    end = 2

    return (avg, end)

#### Arduino functions
def push():
    # arduino.write("e".encode())
    print("push")
def startconveyor():
    # arduino.write("r".encode())
    print("start")
def stopconveyor():
    # arduino.write("t".encode())
    print("stop")

#### HANDLE WINDOW CLOSE EVENT
root.protocol("WM_DELETE_WINDOW", lambda: SetButtonAsPressed("WindowClose"))

#### ARDUINO SECTION
# Create Frame for Top Bar
topbar = Frame(root)
topbar.grid(row=0, columnspan=2)
# Add First Label
Label(topbar, text="Arduino Serialport: ").pack(side=LEFT)
# Add Dropdown with all COM Ports
ports = list(serial.tools.list_ports.comports())
OptionMenu(topbar, ArduinoCOMPort, *ports).pack(side=LEFT)
# Add dropdown with the most common baudrates
baudrates = [ 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, 115200]
OptionMenu(topbar, ArduinoBaudRate, *baudrates).pack(side=LEFT)
# Add Button to connect to arduino
Button(topbar,text="Connect", command=lambda: SetButtonAsPressed("ArduinoConnect")).pack(side=LEFT)
#### RTSP SECTION
# Add Text for RTSP with padding to seperate the menus
Label(topbar, text="RTSP camera url+port: ").pack(side=LEFT, padx=(50, 0))
# Add textentry for RTSP connection url, and add default text "rtsp://"
Entry(topbar, textvariable=RTSPURL).pack(side=LEFT)
# Button to connect to RTSP camera
Button(topbar, text="Connect", command=lambda: SetButtonAsPressed("RTSPConnect")).pack(side=LEFT)
# Button to dsconnect to RTSP camera
Button(topbar, text="Disconnect", command=lambda: SetButtonAsPressed("RTSPDisconnect")).pack(side=LEFT)

#### RTSP VIDEO SECTION
videoCanvas = Canvas(root, width = 640, height = 480)
videoCanvas.grid(row=1, column=0)
#### AVERAGE IMAGE BRIGHTNESS PLOT
plotCanvas = Canvas(root, width = 640, height = 480)
plotCanvas.grid(row=1, column=1)

#### CONTROL PANEL
# Create Frame for Top Bar
controlFrame = Frame(root, bg="#fff")
controlFrame.grid(row=2, column=0, padx=(0, 300))
title = Label(controlFrame, text="Kontrolpanel:")
title.config(font=("Courier", 24))
title.grid(row=0, columnspan=2, padx=(100, 100))

Label(controlFrame, text="Set brightness sensitivity").grid(row=1, column=0)
BrightnessSensitivityScale = ttk.Scale(controlFrame, from_=0, to=255, orient=HORIZONTAL, command= SetBrightnessSensitivity)
BrightnessSensitivityScale.set(250)
BrightnessSensitivityScale.grid(row=1, column=1)
Label(controlFrame, text="Activate conveyor scanning").grid(row=2, column=0)
Checkbutton(controlFrame, variable=Scanning).grid(row=2, column=1, sticky="ew")
Label(controlFrame, text="Activate calibration help (100 px)").grid(row=3, column=0)
Checkbutton(controlFrame, variable=CalibrationHelp).grid(row=3, column=1, sticky="ew")
Label(controlFrame, text="Pixels pr. cm").grid(row=4, column=0)
Entry(controlFrame).grid(row=4, column=1, sticky="ew")
Button(controlFrame, command=lambda: SetButtonAsPressed("ArduinoPush"), text="Push").grid(row=5, column=1, sticky="ew")
Button(controlFrame, command=lambda: SetButtonAsPressed("StartConveyor"), text="Start conveyor").grid(row=6, column=1, sticky="ew")
Button(controlFrame, command=lambda: SetButtonAsPressed("StopConveyor"), text="Stop conveyor").grid(row=7, column=1, sticky="ew")
Button(controlFrame, command=lambda: SetButtonAsPressed("TestScan"), text="Test Scan").grid(row=8, column=1, sticky="ew")
Button(controlFrame, command=lambda: SetButtonAsPressed("OverwriteCamTrigger"), text="Test Conveyor Trigger").grid(row=9, column=1, sticky="ew")


while True:
    ## Check for changes in input
    if buttonregister["RTSPConnect"]:
        if not connectedToRTSP and RTSPURL.get() == "Artificial VideoStream":
            print("artificial vidstream")
            videostream, connectedToRTSP = ArtificialVideoStream().start()
            plotStartTime = time.time()
        elif not connectedToRTSP:
            videostream, connectedToRTSP = VideoStream(RTSPURL.get()).start()
            plotStartTime = time.time()
        else:
            print("Already connected to RTSP")
        buttonregister["RTSPConnect"] = False

    if buttonregister["RTSPDisconnect"]:
        if connectedToRTSP:
            videostream.stop()
            connectedToRTSP = False
        else:
            print("Not connected to RTSP")
        buttonregister["RTSPDisconnect"] = False

    if buttonregister["ArduinoConnect"]:
        print(ArduinoCOMPort.get())
        buttonregister["ArduinoConnect"] = False

    if buttonregister["WindowClose"]:
        if connectedToRTSP:
            videostream.stop()
            connectedToRTSP = False
        root.destroy()
        buttonregister["WindowClose"] = False
        break

    if buttonregister["ArduinoPush"]:
        push()
        buttonregister["ArduinoPush"] = False

    if buttonregister["StartConveyor"]:
        startconveyor()
        buttonregister["StartConveyor"] = False

    if buttonregister["StopConveyor"]:
        stopconveyor()
        buttonregister["StopConveyor"] = False

    if buttonregister["OverwriteCamTrigger"]:
        pass

    if buttonregister["TestScan"] and enableNeuralNetwork:
        image = skimage.io.imread("./test.jpg")
        start_time = time.time()

        avg, min = ScanImage(image)
        print("--- %s seconds ---" % (time.time() - start_time))

        rr, cc = line(0,avg, image.shape[0]-1,avg)
        image[rr, cc] = (0,0,0)
        rr, cc = line(0,end, image.shape[0]-1,end)
        image[rr, cc] = (0,0,0)

        skimage.io.imsave('./test.png', image)

        buttonregister["TestScan"] = False

    if len(PushingQueue) > 0:
        if PushingQueue[0] < time.time():
            push()
            PushingQueue.pop(0)


    # Update video and plot
    if connectedToRTSP:
        meanColor, frame, succes = videostream.read()

        if frame is None:
            videostream.stop()
            connectedToRTSP = False

        if succes:
            # We have a succesful frame
            image = PIL.ImageTk.PhotoImage(image = PIL.Image.fromarray(frame))
            # Push image to canvas
            videoCanvas.create_image(0, 0, image = image, anchor = NW)

            # Clear plot
            plotCanvas.delete("all")

            # Make sure we only start moving X-Axis limits after the plot is halfway through
            if(plotX.size > 300):
                 plotX = np.delete(plotX, 0)
                 plotX = plotX - 1
                 plotY = np.delete(plotY, 0)

            # # appending to the x,y lists
            if time.time() > NextCollisionTime:
                plotX = np.append(plotX, plotX[-1]+1)
                plotY = np.append(plotY, meanColor)
            else:
                plotX = np.append(plotX, plotX[-1]+1)
                plotY = np.append(plotY, plotY[-1])

            # plot line
            plotCanvas.create_line(*np.vstack((plotX, plotY)).reshape((-1,),order='F'))
            # reference line
            plotCanvas.create_line(0, sensitivityThresshold, 640, sensitivityThresshold)

            if(float(meanColor) < float(sensitivityThresshold) and Scanning.get() and time.time() > NextCollisionTime):
                print("Collision detected")
                # We have registered a colorchange, create timestamp with microsecond accuracy
                collisionTime = time.time()
                avg, end = ScanImage(frame)

                # Now we should calculate time til next collision.
                NextCollisionTime = (float(end)/PxPrCm + DistCamToPusher)/CmPrSek + collisionTime
                PushingQueue.append((float(avg)/PxPrCm + DistCamToPusher)/CmPrSek + collisionTime)

        else:
            print("Couldn't read frame(end of stream reached?)")

    root.update()


root.mainloop()
