#serial test
import serial
import matplotlib.pyplot as plt
import numpy as np
from FGConnector import FlightGearConnector
from conversions import ned2lla

#serting the serial ports and serial baud rate to be the same (9600)
ser = serial.Serial(port='/dev/cu.usbserial-0001', baudrate=9600)
ser2 = serial.Serial(port='/dev/cu.usbmodem1201', baudrate=9600)

#creating the connection at the posts 5501 and 5502
FG = FlightGearConnector(5501, 5502)

#intializing the variables
loading = True
loaded = False
idx = 0

#timestep and number of timesteps (needs to match on the microcontroller as well)
step = 0.005
nTimesteps = 3000

#point of origin (XYZ = 0)
#uses the toronto airport (CYYZ) runway 35R
lat0_target = np.deg2rad(43.670923)
lon0_target = np.def2rad(-79.614731)
#msl altitude (height above mean sea level)
alt0 = 170 #in meters

#runway heading (flightgear offsets by 10 degrees)
psi = np.deg2rad(-42)

#downrange distance and xyz starting point
downrange = 2000
X = downrange*np.cos(psi + np.pi)
Y = downrange*np.sin(psi + np.pi)

#intiial altitude (with positive down) as well as speped
Z = -500
speed = 125

#empty arrays that can store the incoming serial values
state = np.zeros(nTimesteps+1)
u_input = np.zeros(nTimesteps+1)
time = np.zeros(nTimesteps+1)

if __name__ = '__main__':
    #keeps reading serial data
    while loading:

        #read both of the serial ports from both microcontrollers
        value = ser.readline()
        value2 = ser2.readline()
        valueInString = str(value, 'UTF-8')
        valueInString2 = str(value2, 'UTF-8')

        #if STOP isnt recieved, convert value to float and assign in into the array
        if not "STOP" in valueInString:
            values_ESP32 = valueInString.split(",")
            values_UNO = valueInString2.split(",")
            try:
                state[idx] = float(values_ESP32[1])
                u_input[idx] = float(values_UNO[1])
            except IndexError:
                break
            time[idx] = idx*step
            idx = idx + 1
            print(idx)

        #stop reaidng data if stop is receibved
        if "STOP" in valueInString:
            loading = False
            loaded = True

    #once data is loaded, plot and pass to flightgear inputs
    if loaded:

        #plot pitch angle vs time
        plt.subplot(2,1,1)
        plt.plot(time, state)
        plt.xlabel("time (s)")
        plt.ylabel(r"$\theta$rad()")

        #plot elevator input vs time
        plt.subplot(2,1,2)
        plt.plot(time, u_input)
        plt.xlabel("time (s)")
        plt.ylabel(r"$\delta$rad()")

        #perform landing sim, fill flightgear values
        FG.resetInputs()
        for i in range(0, len(time)):

            #get putch abngle anf elevator input
            theta = state[i]
            delta = u_input[i]

            #eulers method
            X = X + step*(speed*np.cos(psi)*np.cos(theta))
            Y = Y + step*(speed*np.sin(psi)*np.cos(theta))
            Z = Z + step*(-speed*np.sin(theta))

            #Get LLA from XYZ
            P_LLA = ned2lla(X, Y, Z, lat0_target, lon0_target, alt0)

            #pass to flightgear
            FG.loadInputs(P_LLA[0], P_LLA[1], P_LLA[2], 0, theta, psi, delta)


        #run the sim
        FG.initializeConnection()
        FG.runSimulator()
