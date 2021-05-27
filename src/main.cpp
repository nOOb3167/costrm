#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <memory>
#include <stdexcept>

#ifdef _DEBUG
#	define _CM_DEBUG_WAS_ENABLED
#	undef _DEBUG  // prevents autolinking in python header (pragma comment(lib,"python39_d.lib"))
#endif
#include <Python.h>
#ifdef _CM_DEBUG_WAS_ENABLED
#	define _DEBUG
#endif

#include <costrm.h>

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

		// https://docs.python.org/3/c-api/init_config.html#isolated-configuration
		// https://docs.python.org/3/c-api/init_config.html#path-configuration
		//   Configuration files are still used with this configuration

		concon.base_exec_prefix = L".";
		concon.base_executable = L".";
		concon.base_prefix = L".";
		concon.exec_prefix = L".";
		concon.executable = L".";
		concon.module_search_paths_set = 0;
		concon.module_search_paths = {};
		concon.prefix = L".";

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
	printf("COSTRM_PYPATH " COSTRM_PYPATH "\n");

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