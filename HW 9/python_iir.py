import matplotlib.pyplot as plt
import numpy as np

import csv

t = [] # column 0
data1 = [] # column 1

with open('sigA.csv') as f:
    # open the csv file
    reader = csv.reader(f)
    for row in reader:
        # read the rows 1 one by one
        t.append(float(row[0])) # leftmost column
        data1.append(float(row[1])) # second column

new_average = []
A = 996/1000
B = 4/1000
for i in range(len(data1)):
    if i == 0:
        new_average.append(data1[0])
    else: 
        new_average.append(A*new_average[i-1] + B*data1[i])




plt.title('IIR: A = 995/1000 B = 5/1000')
plt.plot(t, data1, 'k')
plt.plot(t,new_average,'r')
plt.xlabel('Time')
plt.ylabel('Amplitude')


plt.show()