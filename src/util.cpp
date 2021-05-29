#include <filesystem>
#include <stdexcept>

#include <windows.h>

#include <util.h>

namespace cm
{

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
		throw std::runtime_error("SetDllDirectoryW");
}

std::filesystem::path
get_exepath(void)
{
	wchar_t p[MAX_PATH + 1] = {};
	
	if (GetModuleFileNameW(NULL, p, MAX_PATH) == 0)
		throw std::runtime_error("GetModuleFileNameW");

	return std::filesystem::canonical(std::filesystem::path(p));
}

std::filesystem::path
get_exedir(void)
{
	return get_exepath().parent_path();
}

}
