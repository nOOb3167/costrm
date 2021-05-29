#include <filesystem>
#include <stdexcept>
#include <string>

#include <windows.h>

#include <util.h>

namespace cm
{

std::filesystem::path
search_path_py(void)
{
	auto dir = get_exedir();
	for (auto p = dir; p.has_parent_path(); p = p.parent_path()) {
		auto candidate = p / "py";
		auto marker = candidate / "python39.dll";
		if (std::filesystem::is_directory(candidate) && std::filesystem::is_regular_file(marker))
			return candidate;
	}
	throw CM_THROW1();
}

void
set_path_py(void)
{
	std::wstring oldpath(32767 + 1, L'\0');

	if (int i = GetEnvironmentVariableW(L"PATH", oldpath.data(), oldpath.size()); i == 0 || i > oldpath.size())
		throw CM_THROW1();

	std::wstring newpath = search_path_py().native() + L";" + oldpath;

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
