import numpy as np
import matplotlib.pyplot as plt
import math
import scipy.io.wavfile as wav
import parselmouth # making Pitch contour

'''
global variables
'''
sample_rate, audio = wav.read("../audio/audio.wav")
data = np.array(audio).T[1] # normalize the signal and get the data(y value)
frame_size = int(round(sample_rate * 0.01)) # the frame size is 10 ms (by ppt)
frame_num = math.ceil(len(data) / frame_size)

'''
waveform plotting
'''
plt.figure(figsize=(18, 7))
plt.plot(data)
plt.title("Waveform")
plt.xlabel("Sample Point")
plt.ylabel("Amplitude")
plt.savefig('../figure/wavform.png')

'''
Energy contour plotting
'''
def cal_energy_contour() :
    energy = np.zeros(frame_num)
    for i in range(frame_num) :
        curr_frame = np.array(data)[np.arange(i * frame_size, min((i + 1) * frame_size, len(data)))]
        total = 0
        for val in curr_frame :
            total += pow(val, 2)
        energy[i] = total
    return energy

energy = cal_energy_contour()

plt.figure(figsize=(18, 7))
plt.plot(energy)
plt.title("Energy Contour")
plt.xlabel("Frame")
plt.ylabel("Energy")
plt.savefig('../figure/energy_contour.png')

'''
Zero-crossing rate contour plotting
'''
# reference: https://www.mathworks.com/matlabcentral/answers/50070-how-can-i-calculate-zcr-zero-crossing-rate-threshold-for-a-given-signal
def zero_cross_rate_contour(): 
    zero_cross = np.zeros(frame_num)
    for i in range(frame_num) :
        curr_frame = np.sign(np.array(data)[np.arange(i * frame_size, min((i + 1) * frame_size, len(data)))])
        zero_cross[i] = sum(np.abs(np.diff(curr_frame))) / len(curr_frame)
    return zero_cross

zero_cross = zero_cross_rate_contour()

plt.figure(figsize=(18, 7))
plt.plot(zero_cross)
plt.title("Zero-crossing Rate Contour")
plt.xlabel("Frame")
plt.ylabel("Zero-crossing Rate")
plt.savefig('../figure/zero-crossing_rate_contour.png')

"""
End point detection plotting
"""
# reference: internet and ppt
def end_point_detec() : 
    start_point = []
    end_point = []

    # thres = 10 -> at least the begining top ten frame doesn't have anything
    izct = 0.6 # depend on results

    itl = sum(energy) / len(energy)/ 1.5 # depend on results
    itu = 2.5 * itl

    is_end = 0
    curr = 0 # N1
    spec = 0
    for en_x, en_y in enumerate(energy): 
        if is_end == 0: 
            if spec == 0: 
                if en_y >= itl: 
                    spec = 1
                    curr = en_x
            elif spec == 1: 
                for chk_x, chk_y in enumerate(energy[np.arange(curr, len(energy))]): 
                    if chk_y >= itu: 
                        spec = 2
                        break
                    elif chk_y < itl: 
                        spec = 0
                        break
            else: 
                over_zero_thres = 0
                for chk_x, chk_y in enumerate(zero_cross[np.arange(max(0, curr - 10), curr)]): 
                    if chk_y >= izct: 
                        over_zero_thres += 1
                        if over_zero_thres >= 3: # by ppt
                            curr = chk_x
                start_point.append(curr)
                is_end = 1
                spec = 0
        else: 
            if spec == 0: 
                if en_y < itl: 
                    spec = 1
                    curr = en_x
            else: 
                over_zero_thres = 0
                for chk_x, chk_y in enumerate(zero_cross[np.arange(curr, min(curr + 10, len(zero_cross)))]): 
                    if chk_y >= izct: 
                        over_zero_thres += 1
                        if over_zero_thres >= 3: # by ppt
                            curr = chk_x
                end_point.append(curr)
                is_end = 0
                spec = 0
    return start_point, end_point

start_point, end_point = end_point_detec()
points = np.array([np.array(start_point).dot(frame_size), np.array(end_point).dot(frame_size)]).T # extend to the point in waveform

plt.figure(figsize=(18, 7))
plt.plot(data, label = 'Waveform')
for p in points: 
    plt.axvline(x = p[0], color = "#2ca02c") # start point -> green
    plt.axvline(x = p[1], color = "#d62728") # end point -> red
plt.title("End Point Detection")
plt.xlabel("Sample Point")
plt.ylabel("Amplitude")
plt.savefig('../figure/end_point_detection.png')

"""
Pitch contour plotting
"""
# reference: https://parselmouth.readthedocs.io/en/stable/
pitch = parselmouth.Sound('../audio/audio.wav').to_pitch()
pitchValue = pitch.selected_array['frequency']
pitchValue[pitchValue == 0] = np.nan
plt.figure(figsize=(18, 7))
plt.plot(pitch.xs(), pitchValue)
plt.title("Pitch Contour")
plt.xlabel("Frame")
plt.ylabel("Pitch")
plt.savefig('../figure/pitch_contour.png')