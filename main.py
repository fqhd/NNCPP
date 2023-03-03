import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import losses
from tensorflow.keras import layers
import time

model = keras.Sequential([
	layers.Dense(32, activation='sigmoid'),
	layers.Dense(24, activation='sigmoid'),
	layers.Dense(16, activation='sigmoid'),
	layers.Dense(16, activation='sigmoid'),
	layers.Dense(8, activation='sigmoid'),
	layers.Dense(4, activation='sigmoid'),
	layers.Dense(1, activation='sigmoid')
])

x = tf.random.uniform((500, 32))

start = time.time()
model(x)
t = time.time() - start
print(f'Time: {t} seconds')

