CFLAGS=-Wall -Werror -g -O0 -fPIC -shared -I ../
PYUSDT_CFLAGS=`pkg-config --cflags --libs python3-embed`

all:
	gcc $(CFLAGS) $(PYUSDT_CFLAGS) -o pyusdt.so pyusdt.c
	gcc $(CFLAGS) -o usdt-ctypes.so usdt-ctypes.c
