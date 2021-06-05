#pragma once

#include <filesystem>
#include <stdexcept>
#include <string>

#define _CM_THROW1(z) # z

#define CM_THROW1()  std::runtime_error(__FILE__ ":" _CM_THROW1(__LINE__) ":" __FUNCTION__)

namespace cm
{

struct CmPathPy
{
	std::filesystem::path m_exepath;
	std::filesystem::path m_py;
	std::filesystem::path m_pyzip;
};

bool try_delayload(const char *n);
void do_delayload(std::string n);

CmPathPy make_path_py();

std::filesystem::path search_path_py(std::vector<std::string> mark);
void set_path_py(void);
[[deprecated]] void set_dlldir_py(void);
[[deprecated]] void set_dlldir(std::filesystem::path p);
std::filesystem::path get_exepath(void);
std::filesystem::path get_exedir(void);
std::string get_pydll_name(void);
std::string get_pyzip_name(void);

}
