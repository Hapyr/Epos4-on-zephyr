# Import libraries
from numpy import *
from pyqtgraph.Qt import QtGui, QtCore, QtWidgets
import pyqtgraph as pg
import serial
import signal
import time
signal.signal(signal.SIGINT, signal.SIG_DFL)

tmp_data = []
import keyboard

# Realtime data plot. Each time this function is called, the data display is updated
def update():
    start = time.time()

    global curve, ptr, Xm, Xn, tmp_data
    Xm[:-1] = Xm[1:]                      # shift data in the temporal mean 1 sample left
    Xn[:-1] = Xn[1:]                      # shift data in the temporal mean 1 sample left
    #value = ser.readline()                # read line (single value) from the serial port
    value = ser.read(100)
    
    # print(value)
    # g.write(value)


    if len(tmp_data) == 0:
        if b'D=x' in value:
            tmp_data = value[value.index(b'D=x'):]
        else:
            pass
            # print(value)
    else:
        plot_data = []
        plot_data = tmp_data[:tmp_data.rindex(b'\r\n')+2]
        tmp_data = tmp_data[tmp_data.rindex(b'\r\n')+2:]
        tmp_data = tmp_data + value
        
        
        # g.write(plot_data)
        
    
        dataa = plot_data.split(b'\r\n')
    
        #3 - 7
        #10 - 14
        #16 - 20
        #22 - 24
        if len(dataa) > 0:
            d = dataa[0]

            if d != b'' and len(d) == 23:
                #  print(d)
                try:
                    int32_time = int.from_bytes(d[3:7] , 'little', signed=True)
                    int32_pos = int.from_bytes(d[9:13] , 'little', signed=True)

                    #int32_vel = int.from_bytes(d[15:19] , 'little', signed=False)
                    #int32_tor = int.from_bytes(d[21:23] , 'little', signed=True)
    
                    angle = int32_pos*(0.00086666666) + 90 # in rad
                    #velocity = int32_vel*0.10472*(10**(-3))
    
                    # f.write(str(int32_time) + ";" + str(angle) + ";" + str(velocity) + ";" + str(int32_tor) + "\n")
                    # print(int32_time)
    
                    i = float(int32_time)
                    Xn[-1] = angle
    
                    curve.setData(Xn)                     # set the curve with this data
                    curve.setPos(i,0)                   # set x position in the graph to 0
                    QtGui.QApplication.processEvents()    # you MUST process the plot now
                except Exception as e:
                    print(e)
    
    ende = time.time()

def setYRange(self):
    self.enableAutoRange(axis='y')
    self.setAutoVisible(y=True)

f = open("./datalog.csv", "w")
g = open("./datalog_raw_final_40.csv", "wb")
f.write("Time in s; Position in rad; Velocity in rad/s; Torque in mNm\n")


portName = "COM3"                      # replace this port name by yours!
baudrate = 921600
try:
    print("er111")

    ### START QtApp #####
    app = QtWidgets.QApplication([])   
    
    ser = serial.Serial(portName,baudrate)
    win = pg.GraphicsWindow(title="Signal from serial port") # creates a window
    p = win.addPlot(title="Realtime plot")  # creates empty space for the plot in the window
    curve = p.plot()   
                         # create an empty "plot" (a curve to plot)
    keyboard.on_press_key("p", lambda _:ser.write(b'start\r\n'))

    # vb = p.getViewBox() 
    # vb.sigXRangeChanged.connect(setYRange)

    p.setYRange(-20, 190, padding=0)
    windowWidth = 2000                       # width of the window displaying the curve
    Xm = linspace(0,0,windowWidth)          # create array that will contain the relevant time series  
    Xn = linspace(0,0,windowWidth)          # create array that will contain the relevant time series     
    ptr = -windowWidth                      # set first x position

    ### MAIN PROGRAM #####    
    # this is a brutal infinite loop calling your realtime data plot
    while True: update()

    f.close()
except Exception as e:
    print(e)
    f.close()


### END QtApp ####
pg.QtGui.QApplication.exec()
# pg.QtGui.QApplication.exec_() # you MUST put this at the end
##################
