# HTTP Server
from flask import Flask, render_template, request
from werkzeug import secure_filename

# Neural network
import tensorflow as tf
from mrcnn import utils
import mrcnn.model as modellib
from mrcnn.config import Config

# Other stuff
import json
import numpy as np
import os
import sys

# Loading image from disk
import skimage.io

# Multithreading
import threading
from threading import Thread
import time

# Sleeping
import time



graphFilePath = sys.argv[1]

# Variables for communicating with other threads
shouldScanImage = threading.Condition()
shouldReadImage = threading.Condition()

# Empty variable for our results
results = []

# Create webserver
app = Flask(__name__)


# Configs for neural network
class CocoConfig(Config):
    NAME = "coco"
    IMAGES_PER_GPU = 1
    NUM_CLASSES = 1 + 1
    
class InferenceConfig(CocoConfig):
    GPU_COUNT = 1
    IMAGES_PER_GPU = 1
    DETECTION_MIN_CONFIDENCE = 0.985

# An extension to the json library to serialize numpy arrays to json
class NumpyEncoder(json.JSONEncoder):
    def default(self, obj):
        if isinstance(obj, np.ndarray):
            return obj.tolist()
        return json.JSONEncoder.default(self, obj)

# Disable GPU
os.environ['CUDA_VISIBLE_DEVICES'] = '-1'

# HTTP POST request where an image is sent via. form-data with the key "file"
@app.route('/scanImage', methods = ['GET'])
def scanImage():
    # include global variables
    global shouldScanImage
    global shouldReadImage

    # Making sure the client has sent a post request
    if request.method == 'GET':
        # Saving the image the client has included in the request.
        # TODO: check if the file is too large. Why are sanitizing filenames
        #f = request.files['file']
        #f.save(secure_filename("temp.jpg"))

        # Tell thread to run inference on the image we have written to disk
        shouldScanImage.acquire()
        shouldScanImage.notify()
        shouldScanImage.release()

        # Wait until the neural network has scanned the image
        shouldReadImage.acquire()
        shouldReadImage.wait()

        # Return bounding boxes and confidence scores to the client
        return json.dumps({"boundingboxes": results[0]['rois'], "confidence": results[0]['scores']}, cls=NumpyEncoder)

    return "UNABLE TO PARSE IMAGE"

# This function is called in a seperate thread. 
# This is because everything related to the neural network has to be run in the same scope.
def scan_loop():
    global shouldScanImage
    global shouldReadImage
    global results

    # Load configs
    config = InferenceConfig()
    config.display()

    # Read the model from disk.
    # TODO: add model path as runtime argument

    model = modellib.MaskRCNN(mode="inference", model_dir="./logs", config=config)
    model.load_weights(graphFilePath, by_name=True)

    sys.stdout.flush()
    sys.stdout.write('Model Loaded\n')
    sys.stdout.flush()

    # These are never used, oh well
    class_names = ['BG', 'Cigerette']

    # We are now done loading the models from disk

    # This while loop picks up on calls from scanImage(). 
    while True:
        # First we wait until we get notified to process an image.
        shouldScanImage.acquire()
        shouldScanImage.wait()

        # Load image from disk
        image = skimage.io.imread("temp.jpg")

        # Scan image
        results = model.detect([image], verbose=1)

        shouldScanImage.release()

        # Notify our HTTP function that we are done processing the image, 
        # and that they can return the results to the client
        shouldReadImage.acquire()
        shouldReadImage.notify()
        shouldReadImage.release()

if __name__ == '__main__':
    Thread(target = scan_loop).start()
    app.run(debug=True, use_reloader=False)
