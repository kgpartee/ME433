import matplotlib.pyplot as plt
import numpy as np

import csv

t = [] # column 0
data1 = [] # column 1

with open('sigD.csv') as f:
    # open the csv file
    reader = csv.reader(f)
    for row in reader:
        # read the rows 1 one by one
        t.append(float(row[0])) # leftmost column
        data1.append(float(row[1])) # second column

s = []

for i in range(len(data1)):
    if i < len(data1)-300:
        avg = 0
        for j in range(300):
            avg += data1[i+j]
        s.append(avg/300)
        
    else:
        s.append(0)


dt = t[1] - t[0] # take difference in times

# a constant plus 100Hz and 1000Hz
# s = 4.0 * np.sin(2 * np.pi * 100 * t) + 0.25 * np.sin(2 * np.pi * 1000 * t) + 25

Fs = 1 / dt # sample rate
Ts = 1.0/Fs; # sampling interval
ts = np.arange(0,t[-1],Ts) # time vector
y = s # the data to make the fft from
yunfilt = data1
n = len(y) # length of the signal
k = np.arange(n)
T = n/Fs
frq = k/T # two sides frequency range
frq = frq[range(int(n/2))] # one side frequency range
Y = np.fft.fft(y)/n # fft computing and normalization
Y = Y[range(int(n/2))]
Yunfilt = np.fft.fft(yunfilt)/n
Yunfilt = Yunfilt[range(int(n/2))]


fig, (ax1, ax2) = plt.subplots(2, 1)
ax1.set_title('Moving Avg: 300')
ax1.plot(t, yunfilt, 'k')
ax1.plot(t,y,'r')

ax1.set_xlabel('Time')
ax1.set_ylabel('Amplitude')
ax2.loglog(frq,abs(Y),'r') # plotting the fft
ax2.loglog(frq,abs(Yunfilt),'k') # plotting the fft

ax2.set_xlabel('Freq (Hz)')
ax2.set_ylabel('|Y(freq)|')
plt.show()