# Import libraries
from numpy import *
from pyqtgraph.Qt import QtGui, QtCore
import pyqtgraph as pg
import serial


    # Realtime data plot. Each time this function is called, the data display is updated
def update():
    global curve, ptr, Xm, Xn 
    Xm[:-1] = Xm[1:]                      # shift data in the temporal mean 1 sample left
    Xn[:-1] = Xn[1:]                      # shift data in the temporal mean 1 sample left
    value = ser.readline()                # read line (single value) from the serial port
    if value[0:4] == b'Data':
        line_as_list = value.split(b'===')[1].split(b',')
        
        try:
            i = float(line_as_list[0])
            Xn[-1] = float(line_as_list[1].split(b'\r')[0])
            Xm[-1] = float(line_as_list[2].split(b'\r')[0])

            print(i, end=",")
            print(float(line_as_list[1].split(b'\r')[0]), end=",")
            print(float(line_as_list[2].split(b'\r')[0]))


                                    # update x position for displaying the curve
            curve.setData(Xn)                     # set the curve with this data
            curve.setPos(i,0)                   # set x position in the graph to 0
            QtGui.QApplication.processEvents()    # you MUST process the plot now

        except Exception as e:
            print(e)

    ser.flushInput()
    ser.flushOutput()
# Create object serial port
portName = "/dev/ttyUSB0"                      # replace this port name by yours!
baudrate = 115200
try:
    print("er111")

    ### START QtApp #####
    app = QtGui.QApplication([])   

    ser = serial.Serial(portName,baudrate)
    win = pg.GraphicsWindow(title="Signal from serial port") # creates a window
    p = win.addPlot(title="Realtime plot")  # creates empty space for the plot in the window
    curve = p.plot()   
                         # create an empty "plot" (a curve to plot)

    p.setYRange(-1.5, 1.5, padding=0)
    windowWidth = 500                       # width of the window displaying the curve
    Xm = linspace(0,0,windowWidth)          # create array that will contain the relevant time series  
    Xn = linspace(0,0,windowWidth)          # create array that will contain the relevant time series     
    ptr = -windowWidth                      # set first x position

    ### MAIN PROGRAM #####    
    # this is a brutal infinite loop calling your realtime data plot
    while True: update()
except Exception as e:
    print(e)


### END QtApp ####
pg.QtGui.QApplication.exec_() # you MUST put this at the end
##################
