import ctypes
import time

libusdt = ctypes.CDLL('./usdt-ctypes.so')

loop = 0
while True:
    libusdt.trace_start_loop(loop)
    time.sleep(1)
    libusdt.trace_stop_loop(loop)
    loop += 1
