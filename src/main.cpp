#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <filesystem>
#include <memory>
#include <stdexcept>

#include <boost/log/trivial.hpp>

#ifdef _DEBUG
#	define _CM_DEBUG_WAS_ENABLED
#	undef _DEBUG  // prevents autolinking in python header (pragma comment(lib,"python39_d.lib"))
#endif
#include <Python.h>
#ifdef _CM_DEBUG_WAS_ENABLED
#	define _DEBUG
#endif

#include <costrm.h>
#include <util.h>

template<typename T>
using sp = ::std::shared_ptr<T>;

namespace cm
{

class CmPyCon
{
public:
	class cm_py_con_isolated_tag_t {};

	CmPyCon(cm_py_con_isolated_tag_t) :
		m_d()
	{
		PyConfig_InitIsolatedConfig(&m_d);
	}

	~CmPyCon()
	{
		PyConfig_Clear(&m_d);
	}

	PyConfig m_d;
};

class CmPyInFin
{
public:
	CmPyInFin()
	{
		PyPreConfig precon = {};

		PyPreConfig_InitIsolatedConfig(&precon);

		if (PyStatus_Exception(Py_PreInitialize(&precon)))
			throw std::runtime_error("");

		// https://docs.python.org/3/c-api/init_config.html#isolated-configuration
		// https://docs.python.org/3/c-api/init_config.html#path-configuration
		//   Configuration files are still used with this configuration

		CmPyCon concon = CmPyCon(CmPyCon::cm_py_con_isolated_tag_t());

		CmPathPy pp = make_path_py();

		PyConfig_SetString(&concon.m_d, &concon.m_d.base_exec_prefix, pp.m_py.native().c_str());
		PyConfig_SetString(&concon.m_d, &concon.m_d.base_executable, pp.m_exepath.native().c_str());
		PyConfig_SetString(&concon.m_d, &concon.m_d.base_prefix, pp.m_py.native().c_str());
		PyConfig_SetString(&concon.m_d, &concon.m_d.exec_prefix, pp.m_py.native().c_str());
		PyConfig_SetString(&concon.m_d, &concon.m_d.executable, pp.m_exepath.native().c_str());
		PyConfig_SetString(&concon.m_d, &concon.m_d.prefix, pp.m_py.native().c_str());
		concon.m_d.module_search_paths_set = 1;
		PyWideStringList_Append(&concon.m_d.module_search_paths, pp.m_pyzip.c_str());
		PyWideStringList_Append(&concon.m_d.module_search_paths, pp.m_py.c_str());

		if (PyStatus s = Py_InitializeFromConfig(&concon.m_d); PyStatus_Exception(s)) {
			Py_ExitStatusException(s);
			throw CM_THROW1();  /* unreachable */
		}
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

void stuff()
{
	BOOST_LOG_TRIVIAL(warning) << "Compiled Against Python Header Version " PY_VERSION;
	BOOST_LOG_TRIVIAL(warning) << "COSTRM_PYPATH " COSTRM_PYPATH;

	CmPyInFin cpif;

	auto codeobj = sp<PyObject>(Py_CompileString(R"EOF(
import sys
print(sys.path)
)EOF",
		"nofilename",
		Py_file_input), sp_pyobject_d);

	auto modobj = sp<PyObject>(PyImport_ExecCodeModule("mod0", &*codeobj), sp_pyobject_d);

	assert(codeobj && modobj);

	std::filesystem::path p = get_exepath();
	std::filesystem::path r = get_exedir();
}

}

int main(int argc, char** argv)
{
	cm::set_path_py();

	cm::stuff();

	return EXIT_SUCCESS;
}
