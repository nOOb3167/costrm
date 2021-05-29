#include <algorithm>
#include <filesystem>
#include <stdexcept>
#include <string>

#include <windows.h>

#include <util.h>

namespace cm
{

CmPathPy
make_path_py()
{
	auto p = search_path_py({ "python39.dll", "python39.zip" });
	return { .m_exepath = get_exepath(), .m_py = p, .m_pyzip = p / "python39.zip" };
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

}
