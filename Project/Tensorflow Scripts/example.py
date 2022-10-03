from distutils.log import Log
import tensorflow as tf
import tensorflow_io as tfio
import matplotlib.pyplot as plt
import os.path
import sys
import utils.Log as l;
import numpy as np
from scipy.io.wavfile import write

audioPath = 'Tensorflow Scripts/speech_data/Luna/'
file1 = "Luna.wav"
file2 = "Luna2.wav"

if not os.path.exists(audioPath):
    l.logError("File doesn't exist")
    exit()

# creating a empty tensor
tensor = tf.reshape(tf.convert_to_tensor(()), (0, 16752))


audio1 = tfio.audio.AudioIOTensor(audioPath + file1)
audio2 = tfio.audio.AudioIOTensor(audioPath + file2)

print("\n\n\n\n\n")

l.log("Tensor Type", type(audio1[100:][0]))
l.log("Array Size", audio1[100:].get_shape()[0])

tensor = audio1[100:].numpy()

# Ploting two audios in the same plot
plt.figure()
plt.plot(tfio.audio.AudioIOTensor(audioPath + file1)[100:], 'g')
plt.plot(tensor, 'b')
plt.show()

# Saving the Tensor data to a wav file
samplerate = 16000
data = tensor

write("example.wav", samplerate, data)
