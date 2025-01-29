import pyusdt
import time

loop = 0
while True:
    pyusdt.trace_start_loop(loop)
    time.sleep(1)
    pyusdt.trace_stop_loop(loop)
    loop += 1
