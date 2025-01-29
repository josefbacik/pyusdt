#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <usdt/usdt.h>

static PyObject *pyusdt_trace_start(PyObject *self, PyObject *args)
{
	int loop;

	if (!USDT_IS_ACTIVE(pyusdt, start_loop))
		Py_RETURN_NONE;

	if (!PyArg_ParseTuple(args, "i", &loop))
		return NULL;

	USDT_WITH_SEMA(pyusdt, start_loop, loop);
	Py_RETURN_NONE;
}

static PyObject *pyusdt_trace_end(PyObject *self, PyObject *args)
{
	int loop;

	if (!USDT_IS_ACTIVE(pyusdt, stop_loop))
		Py_RETURN_NONE;

	if (!PyArg_ParseTuple(args, "i", &loop))
		return NULL;

	USDT_WITH_SEMA(pyusdt, stop_loop, loop);
	Py_RETURN_NONE;
}

static struct PyMethodDef PyUSDTMethods[] = {
	{"trace_start_loop", pyusdt_trace_start, METH_VARARGS, "Tracpoint for starting the loop"},
	{"trace_stop_loop", pyusdt_trace_end, METH_VARARGS, "Tracepoint for ending the loop"},
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef pyusdtmodule = {
	PyModuleDef_HEAD_INIT,
	"pyusdt",
	NULL,
	-1,
	PyUSDTMethods
};

PyMODINIT_FUNC PyInit_pyusdt(void)
{
	return PyModule_Create(&pyusdtmodule);
}
