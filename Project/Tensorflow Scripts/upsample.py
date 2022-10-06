import os.path
import numpy as np
from tqdm.notebook import tqdm
import tensorflow as tf
import tensorflow_io as tfio
from tensorflow.io import gfile
from scipy.io.wavfile import write
import utils.mixaudio as maux
import shutil
import matplotlib.pyplot as plt
import random

SPEECH_DATA='Tensorflow Scripts/speech_data'

background_folder = 'background_noise'
output_folder = SPEECH_DATA + '/upsample'
samplerate = 16000

# get the location of the voice
def get_voice_position(audio, noise_floor):
    for x in range(0, len(audio)):
        if (audio[x] > noise_floor):
            return audio[x:]
    return audio

# get all the files in a directory
def get_files(word):
    return gfile.glob(SPEECH_DATA + '/'+word+'/*.wav')

# process the files for a word into the spectrogram and one hot encoding word value
def process_word(word):
    # the index of the word word we are processing
    label = folders.index(word)
    # get a list of files names for the word
    file_names = [file_name for file_name in tqdm(get_files(word), desc="Checking", leave=False)]
    background_noise_samples = [file_noise for file_noise in tqdm(get_files(background_folder), desc="Checking", leave=False)]
    x = 0
    for name in file_names:
        # print (name)
        file = tfio.audio.AudioIOTensor(name)[0:].numpy().copy()
        for bg_noise in background_noise_samples:
            for loop in range(0, 5):
                x = x + 1
                audio = get_voice_position(tfio.audio.AudioIOTensor(name)[0:].numpy(), 500)
                audio = maux.resize_random(audio, samplerate)
                noise = tfio.audio.AudioIOTensor(bg_noise)

                rd = random.randrange(len(noise) - samplerate)

                data = maux.sum(audio, noise[rd:].numpy(), samplerate)
                # print("File Name:" + name)
                # print("Noise Name:" + bg_noise)
                write(output_folder + "/" + str(x) + ".wav", samplerate, data)

                # # Ploting two audios in the same plot
                # plt.figure()
                # plt.title(bg_noise)
                # plt.plot(data, 'b')
                # plt.plot(audio, 'g')
                # plt.show()

if os.path.exists(output_folder):
    shutil.rmtree(output_folder)
os.makedirs(output_folder)

# The audio is all sampled at 16KHz and should all be 1 second in length - so 1 second is 16000 samples
EXPECTED_SAMPLES=16000
# Noise floor to detect if any audio is present
NOISE_FLOOR=0.1
# How many samples should be abover the noise floor?
MINIMUM_VOICE_LENGTH=EXPECTED_SAMPLES/4

# list of folders we want to process in the speech_data folder
from tensorflow.python.ops import gen_audio_ops as audio_ops
folders = [
    'Luna',
]


# process all the words and all the files
for word in tqdm(folders, desc="Processing words"):
    process_word(word)
