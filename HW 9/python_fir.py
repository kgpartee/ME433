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

h = [
    0.004321734446567168,
    0.004964181359029018,
    0.006726853024102915,
    0.009590710694428429,
    0.013477194377763563,
    0.018250807495402411,
    0.023724953403477973,
    0.029670748540558761,
    0.035828357100486319,
    0.041920237367045872,
    0.047665569920356123,
    0.052795060488147479,
    0.057065280347479520,
    0.060271727052549448,
    0.062259856977999038,
    0.062933454809211808,
    0.062259856977999038,
    0.060271727052549441,
    0.057065280347479534,
    0.052795060488147492,
    0.047665569920356143,
    0.041920237367045886,
    0.035828357100486347,
    0.029670748540558775,
    0.023724953403477980,
    0.018250807495402411,
    0.013477194377763560,
    0.009590710694428432,
    0.006726853024102912,
    0.004964181359029018,
    0.004321734446567168,
]

for i in range(len(data1)):
    if i < len(h):
        s.append(0)
    elif i < len(data1):
        avg = 0
        for j in range(len(h)):
            avg += data1[i-j] * h[j]
        s.append(avg)
        
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
ax1.set_title('lowpass, cutoff 0.1hz, bw 1hz, sr 10hz, hamming window')
ax1.plot(t, yunfilt, 'k')
ax1.plot(t,y,'r')

ax1.set_xlabel('Time')
ax1.set_ylabel('Amplitude')
ax2.loglog(frq,abs(Y),'r') # plotting the fft
ax2.loglog(frq,abs(Yunfilt),'k') # plotting the fft

ax2.set_xlabel('Freq (Hz)')
ax2.set_ylabel('|Y(freq)|')
plt.show()