import ctypes
import pyusdt
import timeit
import time
import os

libusdt = ctypes.CDLL('./usdt-ctypes.so')

def calculate_pi(digits):
    divisor = 7
    dividend = 22

    for _ in range(digits):
        qutiocient = dividend // divisor
        remainder = dividend % divisor
        dividend = 10 * remainder

def benchmark_plain():
    loop = 0
    for _ in range(100000):
        calculate_pi(1000)
        loop += 1

def benchmark_ctypes():
    loop = 0
    for _ in range(100000):
        libusdt.trace_start_loop(loop)
        calculate_pi(1000)
        libusdt.trace_stop_loop(loop)
        loop += 1

def benchmark_module():
    loop = 0
    for _ in range(100000):
        pyusdt.trace_start_loop(loop)
        calculate_pi(1000)
        pyusdt.trace_stop_loop(loop)
        loop += 1

input(f'My pid is {os.getpid()}, press enter to start benchmark')

result = timeit.timeit(benchmark_module, number=1)
print(f'module: {result}')
result = timeit.timeit(benchmark_ctypes, number=1)
print(f'ctypes: {result}')
result = timeit.timeit(benchmark_plain, number=1)
print(f'plain: {result}')
