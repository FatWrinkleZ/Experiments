from cmath import sin, sqrt
from decimal import Clamped
from pdb import lasti2lineno
import random
import turtle
import pyaudio
import wave
import sys
from turtle import *
import math
import numpy



CHUNK = 1024

shapes = ['circle', 'square', 'triangle','classic','arrow']
selectedShape = 0

ico = turtle.Turtle()
scr = ico.getscreen()
ico.speed(0)
#outline = turtle.Turtle()
#outline.color(0,0,0)
#outline.penup()
#outline.shape('circle')
##ostamp = outline.stamp()
ico.penup()
ico.shape('circle')
ico.color(1,1,1)

if len(sys.argv) < 2:
    print("Plays a wave file.\n\nUsage: %s filename.wav" % sys.argv[0])
    sys.exit(-1)

wf = wave.open(sys.argv[1], 'rb')

# instantiate PyAudio (1)
p = pyaudio.PyAudio()

# open stream (2)
stream = p.open(format=p.get_format_from_width(wf.getsampwidth()),
                channels=wf.getnchannels(),
                rate=wf.getframerate(),
                output=True)

# read data
data = wf.readframes(CHUNK)
def Animate(s, time):
    if(time % 2 == 0):
        ##outline.showturtle()
        ##outline.clearstamp(ostamp)
        ns = s+1
        ico.left(3)
        #outline.left(3)
        #outline.shapesize(ns,ns, 0)
        ##ostamp = outline.stamp()
        #outline.hideturtle()
        ico.shapesize(s,s,0.1)
    elif(time % 3 == 0 ):
        r = abs(math.sin(time/50)) /2
        g = abs(math.sin(time/50 + 1))/2
        b = abs(math.sin(time/50 -1))/2
        ico.screen.bgcolor(r,g,b)


# play stream (3)
ind = 0
tim = 0
lastSize = 0
highestAmp = 0
while len(data) > 0:
    stream.write(data)
    data = wf.readframes(CHUNK)
    blockLinearRms= numpy.sqrt(numpy.mean(data[2]**2))
    if(tim % 50 == 0):
        print("CurrentAmp = " + str(blockLinearRms))
    
    #blockLogRms = (20 * math.log10(blockLinearRms+1) +1)*0.7
    if(highestAmp < math.fabs(blockLinearRms)):
        highestAmp = math.fabs(blockLinearRms)
        print("New Highest Amp : " + str(highestAmp) + "\n")
    if(tim % 3 == 0 and (blockLinearRms <= 1)):
        selectedShape+=1
        if(selectedShape>4):
            selectedShape = 0
        ico.shape(shapes[selectedShape])
        #outline.shape(shapes[selectedShape])
        ico.color(random.random(), random.random(), random.random())
        ind = 0
    s = 20 if blockLinearRms/5.5 < 20 else blockLinearRms/5.5
    s = 30 if s > 30 else s
    if(abs(s-lastSize)>5):
        s += (lastSize-s)/2
    lastSize = s
    Animate(s, tim)
    tim +=1
    ind+=0.1
    # Do all of your DSP processing here i.e. function call or whatever

# stop stream (4)
stream.stop_stream()
stream.close()

# close PyAudio (5)
p.terminate()
sys.exit(-1)