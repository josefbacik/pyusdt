# pyusdt: Code examples of how to use usdt.h in Python

[usdt.h](https://github.com/libbpf/usdt) is a project to quickly and easily add
USDTs (User Statically-Defined tracepionts, or just "user space tracepoints") to
your project.

For more information on the specifics of USDTs, see the documentation in the
above linked repository.

Accessing this from python is a little bit tricky, but it's possible. This repo
serves purely to demonstrate two different methods of accessing USDTs from
python code.

## Method 1: ctypes

This method uses `ctypes` to access the USDT probes. It's the simplest method,
but requires you to have a path to the shared object file that contains the
USDT.  The code for this is in `usdt-ctypes.c` and `loop-ctypes.py`. There are
two parts to this

1. **The USDT function**.  This is the function that will be called from python,
   and simply needs to be a public function that wraps around the USDT macros.
2. **Load the shared library and call the function**. This is done in the python
   script `loop-ctypes.py`.  The drawback here is you need to know the path to
   the shared library.

## Method 2: Python extension module

If you want to do more complicated things, such as have actual python objects,
throw errors, etc. you can use a python extension module. This is a bit more
involved than the above method, but allows for more flexibility. The code for
this is in `pyusdt.c` and `loop-extension-module.py`.  Again there are two parts
for this

1. **The Python Module**. This is the python module that will be imported into
   the python script. It needs to be a shared object file that is loaded by
   python.  This acts just like any other python module, and you can have access
   to python objects that are passed into the trace points.  This can be
   particularly useful if you have complex data structures that you want to pass
   into your trace points.
2. **Import your python module**. This is similar to above, except you simply
   use `import mymodule` and call it like any other python module. This means
   you can install the shared object anywhere python looks for its modules.

## Seeing it in action

```bash
$ make
$ cd src
$ python loop-ctypes.py &
$ sudo bpftrace -e 'usdt:*:ctypes_usdt:start_loop { printf("loop is %d pid %d\n", arg0, pid); }'
$ python loop-extension-module.py &
$ sudo bpftrace -e 'usdt:*:pyusdt:start_loop { printf("loop is %d pid %d\n", arg0, pid); }'
```

## Benchmarking

The tracepoints aren't free, and their cost will greatly depend on the BPF
program attached to the tracepoint. However they are mostly free when they are
off, and there's slight variations on their cost depending on the implementation
you decide to use.  The `benchmark.py` script is a simple script that runs
100,000 loops of calculating 1000 digits of pi, with a tracepoint at the start
and end of the loop, while increasing a loop counter to print out.  This is a
microbenchmark, and should be taken with a grain of salt, but is useful for
showing the low cost of the tracepoints.

1. **Method 1**.  This is with the following `bpftrace` command running

```
usdt:*:ctypes_usdt:start_loop
usdt:*:pyusdt:start_loop
{
  @start[pid] = nsecs;
}

usdt:*:ctypes_usdt:end_loop
usdt:*:pyusdt:end_loop
/@start[pid]/
{
  @total[pid] += nsecs - @start[pid];
}
```

and the results on my machine are as follows

```bash
$ python benchmark.py
My pid is 1443398, press enter to start benchmark
module: 6.061789590865374
ctypes: 6.116902083158493
plain: 5.8920358046889305
```

The `module` is the time it takes to run the python extension module, `ctypes`
is the simpler ctypes module.  As you can see there's an overhead to using the
`ctypes` method, on top of the overhead of the tracepoint itself.  The `module`
has a 2.8% overhead, while the `ctypes` method has a 3.8% overhead.

2. **Method 2**.  This is with no `bpftrace` command running, so the tracepoints
   are disabled, so the values represent the overhead of the tracepoints
   existing without being used.

```bash
$ python benchmark.py
module: 5.875723026692867
ctypes: 5.916282083839178
plain: 5.882706586271524
```

The `module` case is faster than the `plain` case, so we can generally assume
that the overhead of the tracepoints when disabled is negligible if not zero.
