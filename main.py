import numpy as np
import scipy.io.wavfile as wave

data = []
with open("text.txt", 'r') as f:
    for line in f:
        data.append([int(x) for x in line.split()])

wave.write("test.wav", 50000, np.int16(data))