#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <memory>
#include <stdexcept>

#include <Python.h>

template<typename T>
using sp = ::std::shared_ptr<T>;

class CmPyInFin
{
public:
	CmPyInFin()
	{
		PyPreConfig precon = {};
		PyConfig concon = {};

		PyPreConfig_InitIsolatedConfig(&precon);
		PyConfig_InitIsolatedConfig(&concon);

		if (PyStatus_Exception(Py_PreInitialize(&precon)))
			throw std::runtime_error("");
		if (PyStatus_Exception(Py_InitializeFromConfig(&concon)))
			throw std::runtime_error("");
	}

	~CmPyInFin()
	{
		Py_Finalize();
	}
};

void sp_pyobject_d(PyObject* a)
{
	Py_XDECREF(a);
}

int main(int argc, char** argv)
{
	printf("Compiled Against Python Header Version " PY_VERSION "\n");

	CmPyInFin cpif;

	auto codeobj = sp<PyObject>(Py_CompileString(R"EOF(
import sys
print(sys.path)
)EOF",
		"nofilename",
		Py_file_input), sp_pyobject_d);

	auto modobj = sp<PyObject>(PyImport_ExecCodeModule("mod0", &*codeobj), sp_pyobject_d);

	assert(codeobj && modobj);

	return EXIT_SUCCESS;
}
