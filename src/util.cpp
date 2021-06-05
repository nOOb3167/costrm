#include <algorithm>
#include <filesystem>
#include <stdexcept>
#include <string>

#include <boost/log/trivial.hpp>

#include <windows.h>
#include <delayimp.h>

#include <costrm.h>
#include <costrm_aux.h>
#include <util.h>

namespace cm
{

bool
try_delayload(const char *n)
{
	// TODO: https://docs.microsoft.com/en-us/cpp/build/reference/error-handling-and-notification?view=msvc-160#delay-load-exception-codes
	// TODO: https://github.com/MicrosoftDocs/cpp-docs/issues/1003
	__try
	{
		return SUCCEEDED(__HrLoadAllImportsForDll(n));
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}
}

void
do_delayload(std::string n)
{
	if (!try_delayload(n.c_str())) {
		BOOST_LOG_TRIVIAL(warning) << "Failed delayload [" << n << "]";
		throw CM_THROW1();
	}
}

CmPathPy
make_path_py()
{
	auto p = search_path_py({ get_pydll_name(), get_pyzip_name() });
	return { .m_exepath = get_exepath(), .m_py = p, .m_pyzip = p / get_pyzip_name() };
}

std::filesystem::path
search_path_py(std::vector<std::string> mark)
{
	auto dir = get_exedir();
	for (auto p = dir; p.has_parent_path(); p = p.parent_path()) {
		auto candidate = p / "py";
		if (std::filesystem::is_directory(candidate) &&
			std::all_of(mark.begin(), mark.end(),
				[&candidate](const std::string& a) { return std::filesystem::is_regular_file(candidate / a); }))
		{
			return candidate;
		}
	}
	throw CM_THROW1();
}

void
set_path_py(void)
{
	std::wstring oldpath(32767 + 1, L'\0');

	if (int i = GetEnvironmentVariableW(L"PATH", oldpath.data(), oldpath.size()); i == 0 || i > oldpath.size())
		throw CM_THROW1();

	std::wstring newpath = make_path_py().m_py.native() + L";" + oldpath;

	if (SetEnvironmentVariableW(L"PATH", newpath.c_str()) == 0)
		throw CM_THROW1();
}

void
set_dlldir_py(void)
{
	set_dlldir(get_exedir() / "py");
}

void
set_dlldir(std::filesystem::path p)
{
	auto r = p.native();
	const wchar_t* t = r.data();

	if (SetDllDirectoryW(t) == 0)
		throw CM_THROW1();
}

std::filesystem::path
get_exepath(void)
{
	std::wstring p(MAX_PATH + 1, L'\0');
	
	if (GetModuleFileNameW(NULL, p.data(), p.size()) == 0)
		throw CM_THROW1();

	return std::filesystem::canonical(std::filesystem::path(p));
}

std::filesystem::path
get_exedir(void)
{
	return get_exepath().parent_path();
}

std::string
get_pydll_name(void)
{
	const char l[] = COSTRM_PYDLL_NAME;
	std::string m(l);
	return m;
}

std::string get_pyzip_name(void)
{
	std::string r = std::string("python") + COSTRM_PYVERS + ".zip";
	return r;
}

}
