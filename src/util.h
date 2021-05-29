#include <filesystem>

namespace cm
{

void set_dlldir_py(void);
void set_dlldir(std::filesystem::path p);
std::filesystem::path get_exepath(void);
std::filesystem::path get_exedir(void);

}
