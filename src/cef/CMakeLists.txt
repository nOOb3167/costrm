set(CEF_ROOT ${CMAKE_BINARY_DIR}/cef CACHE PATH "")
list(APPEND CMAKE_MODULE_PATH "${CEF_ROOT}/cmake")

# cef_variables.cmake defaults shoving in the /MT(d) flag but will leave us alone if this variable is cleared
set(CEF_RUNTIME_LIBRARY_FLAG "")

find_package(Cef REQUIRED)
add_subdirectory(${CEF_LIBCEF_DLL_WRAPPER_PATH} libcef_dll_wrapper)
PRINT_CEF_CONFIG()

add_executable(cefmain cefmain.cpp)
SET_EXECUTABLE_TARGET_PROPERTIES(cefmain)
target_link_libraries(cefmain libcef_dll_wrapper ${CEF_STANDARD_LIBS})

include(${CMAKE_SOURCE_DIR}/cmake/PropHelper.cmake)
print_target_properties(cefmain ${CMAKE_BUILD_TYPE})
