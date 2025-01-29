#include <usdt/usdt.h>

void trace_start_loop(int loop)
{
	if (USDT_IS_ACTIVE(ctypes_usdt, start_loop))
		USDT_WITH_SEMA(ctypes_usdt, start_loop, loop);
}

void trace_stop_loop(int loop)
{
	if (USDT_IS_ACTIVE(ctypes_usdt, stop_loop))
		USDT_WITH_SEMA(ctypes_usdt, stop_loop, loop);
}
