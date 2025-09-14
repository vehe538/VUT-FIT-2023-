import numpy as np
import matplotlib.pyplot as plt
from scipy.fft import fft, fftfreq
from scipy.io import wavfile
from scipy.signal import butter, filtfilt

car_files = ['xmesikj00/BMW_318i_Drive.wav','xmesikj00/BMW_1_Drive.wav', 'xmesikj00/Opel_Corsa_Drive.wav', 'xmesikj00/Subaru_Forester_Drive.wav']
test_files = ['xmesikj00/test_c.wav', 'xmesikj00/test_m.wav', 'xmesikj00/test_r.wav', 'xmesikj00/test_o.wav']

samplerate1, car_sound = wavfile.read(test_files[1])
time = np.linspace(0, len(car_sound) / samplerate1, len(car_sound))

all_corrcoef = []
all_tests = []
all_cars = []
all_pairs = []


for car in car_files:

  samplerate, car_sound = wavfile.read(car)
  time = np.linspace(0, len(car_sound) / samplerate, len(car_sound))
  fft_car = fft(car_sound)

  
  for test in test_files:



    samplerate, test_sound = wavfile.read(test)
    fft_test = fft(test_sound)

    all_pairs.append(car[10:]+"|"+test[10:])
    x = np.correlate(fft_car, fft_test)
    all_corrcoef.append(x)


off_limits = []
num = 0

while(True):
  if num == 4 :
    break
  
  max_index = all_corrcoef.index(max(all_corrcoef))


  curr_car = all_pairs[max_index][:all_pairs[max_index].find("|")]
  curr_test = all_pairs[max_index][all_pairs[max_index].find("|")+1:]        

  if (curr_car not in off_limits) :


    print(all_pairs[max_index], end=" : ")
    print(all_corrcoef[max_index])
      
    off_limits.append(curr_car)
    off_limits.append(curr_test)

    all_pairs.pop(max_index)
    all_corrcoef.pop(max_index)
    num += 1
  else:
    all_pairs.pop(max_index)
    all_corrcoef.pop(max_index)
    




