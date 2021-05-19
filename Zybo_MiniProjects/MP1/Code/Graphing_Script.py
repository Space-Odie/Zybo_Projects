import csv
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import StepPatch

from tkinter import * 
from tkinter.filedialog import askopenfilename

import os
import math

#Configure These Values
x_data_points = 50              #Enter # Of X Data Points
xlabel = 'Period'              
ylabel = 'Value'               
Title = 'Mini Project 1 - Test 3'

#School Colors:
Black = '#000000'
Patone = '#D22030'
#Secondary Colors:
Cool_Gray = '#55565A'

#Set Colors
y_color = Black
x_color = Black
graph_color = Patone
Title_Color = Patone


filepath = askopenfilename() 
print(filepath)
X_Axis = []
Y_Axis = []
with open(filepath, 'r') as csvfile:
    plots = csv.reader(csvfile,delimiter =',',)
    for i, row in enumerate(plots):
        if i == 0:                                                          #Initialize Graph
            Header = row                                                    #Grab the Header Descriptions
            Total_Points = len(Header) - 1                                  #Calcualte how many Y values there are 

        else:
            x_obj = int(row[0])
            X_Axis.append(x_obj)
            y_obj = int(row[1])
            Y_Axis.append(y_obj)


step = math.ceil(len(X_Axis)/x_data_points)

fig, ax1 = plt.subplots()


lines, labels = ax1.get_legend_handles_labels()
ax1.legend(lines, labels, loc = 0 )
ax1.set_xticklabels(X_Axis[::step], rotation=45)
ax1.tick_params(axis = 'y', labelcolor = y_color)
ax1.tick_params(axis = 'x', labelcolor = x_color)
ax1.set_xlabel(xlabel)
ax1.set_ylabel(ylabel)
ax1.set_title(Title, color=Title_Color)
#t = plt.title(Title)



plt.step(X_Axis, Y_Axis, color=graph_color)
fig.tight_layout()
plt.grid(True)
plt.show()