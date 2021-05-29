#include <filesystem>
#include <stdexcept>

#define _CM_THROW1(z) # z

#define CM_THROW1()  std::runtime_error(__FILE__ ":" _CM_THROW1(__LINE__) ":" __FUNCTION__)

namespace cm
{

std::filesystem::path search_path_py(void);
void set_path_py(void);
[[deprecated]] void set_dlldir_py(void);
[[deprecated]] void set_dlldir(std::filesystem::path p);
std::filesystem::path get_exepath(void);
std::filesystem::path get_exedir(void);

}
