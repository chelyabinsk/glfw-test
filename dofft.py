#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import numpy as np
from scipy.fftpack import fft
from pydub import AudioSegment


song = AudioSegment.from_file("black.mp3")
#song = song.set_channels(1)

M = 73728//2

# scale = int(np.floor(song.rate/M*seconds))
# #print(prev_seconds,seconds)
# prev_scale = int(np.floor(self.J.song.rate/self.J.M*(self.J.prev_seconds)))
# slice_num = [self.J.M*(scale),self.J.M*(scale+1)]


slice_num = [0,M]

song_slice = song.get_sample_slice(slice_num[0],slice_num[1]).get_array_of_samples()
spectrum = fft(song_slice)

spectrum = 2*np.abs(spectrum)[:M//2]  

freq_space = (song.frame_rate /1 /M)

with open("fft_python.csv","w") as f:
    #for c,i in enumerate(spectrum):
    for c,i in enumerate(song_slice):
        f.write(f'{c},{i}\n')

invec = [17767,9158,-26519,18547,-9135,23807,-27574,22764,7977]
a=fft(invec)

import matplotlib.pyplot as plt
plt.plot(spectrum)