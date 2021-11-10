#Date: April 15, 2021
#Student Name: Abraham Aziz
#MAC ID: Aziza22
#Student Number: 400261852
#Python version 3.8.9

import serial
import open3d as o3d
import math

s = serial.Serial("COM3", 115200)           # Connecting board to PC

print("Connected with Microcontroller")     # Telling user that the microcontroller is connected
s.write(b'1')
f = open("Data.xyz", "a")                   # Creates the file where the data is being written on

delta_x = 250                               # The varible repersenst the movement done in the x axis
SPINS_Amount = 10                           # The amount of data that is being collected before opening open3d
                                            # Angle starts at zero, ends at angle 360, increment every 2.8125 deg. 
                                            # All the dregree is multiplied by 1000000 so there is no decimal
Temp = list(range(0, 3600000, 28125))       # Makes a temp list

Angle = [None]*len(Temp)                    # Stores all the values of the angle being used
for i in range(len(Temp)):                  # Adds the Value of temp to angle
    Angle[i] = Temp[i]/10000                # The anle is divided by 1000000 so it is back to the exact angle

Amount_of_point = len(Angle)                # Gets the amount of points needs to fill the size
y_axis = [None]*Amount_of_point             # List is made to how thw values of y
Z_axis = [None]*Amount_of_point             # List is made to how thw values of z

for j in range(SPINS_Amount):               # Loops depending on the amount of the data the user wants
    print("Ready for Spin " + str(j+1))     # Shows the user what spin they are on
    for i in range(Amount_of_point):        # Loop till all data point for the angle is met
       
        point = s.readline()                # reads the line of data, thwn decodes that data to a string
        rawdata = point.decode('utf-8')
        DataPoint = rawdata.split(",")      # organizes the data, so that graphing would be easier
        rad = math.radians(Angle[i])        # Converts the angle to degree
        y_axis[i] = int(DataPoint[1])*math.cos(rad)                   # Translathe that point to an y cordnate
        Z_axis[i] = int(DataPoint[1])*math.sin(rad)                   # Translathe that point to an y cordnate

        print("Distances Acquired at Angle: " + str(Angle[i]))        # Prints the angle where the data was collected
        f.write('{} {} {}\n'.format(j*delta_x, y_axis[i], Z_axis[i])) # Writes the data in ta xyz file
        
    print("Spin " + str(j+1) + " Completed" )                         # Shows the user that all the data was measured
f.close()

point_data = o3d.io.read_point_cloud("Data.xyz")                      # Reads the file which the data will be used
o3d.visualization.draw([point_data])                                  # Draws the data
s.close()
