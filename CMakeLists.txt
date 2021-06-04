cmake_policy(VERSION 3.20)
cmake_minimum_required(VERSION 3.20)
project(costrm VERSION 3.20)

include(${CMAKE_SOURCE_DIR}/cmake/BoostHelper.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/PropHelper.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/ForbiddenHebrewSorcery000.cmake)

set(Boost_VERBOSE TRUE)
set(Boost_USE_STATIC_LIBS TRUE)

find_package(Boost REQUIRED COMPONENTS log CONFIG)

file(TO_CMAKE_PATH ${COSTRM_PYPATH} COSTRM_PYPATH)

set(COSTRM_PYINC "${COSTRM_PYPATH}/include")
set(COSTRM_PYLIB "${COSTRM_PYPATH}/libs/python${COSTRM_PYVERS}$<$<CONFIG:Debug>:_d>.lib")

configure_file(src/costrm.h.in costrm.h @ONLY)

forbidden_hebrew_sorcery000(${COSTRM_PYINC})

add_executable(costrm
	src/main.cpp
	src/util.cpp		src/util.h
	)
target_include_directories(costrm PUBLIC
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_BINARY_DIR}
	${COSTRM_PYINC}
	)
target_link_libraries(costrm
	${COSTRM_PYLIB}
	delayimp.lib
	Boost::headers
	Boost::log
)
target_compile_definitions(costrm PUBLIC -DPY_SSIZE_T_CLEAN)
target_compile_features(costrm PUBLIC cxx_std_20)
target_link_options(costrm PUBLIC "/DELAYLOAD:python39$<$<CONFIG:Debug>:_d>.dll")

add_custom_target(COSTRM_COPY_DATA
	COMMAND
		${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/py/ py/
	WORKING_DIRECTORY
		${CMAKE_BINARY_DIR}
	)

# https://github.com/MicrosoftDocs/visualstudio-docs/issues/6425
# https://github.com/MicrosoftDocs/visualstudio-docs/blob/master/docs/python/working-with-c-cpp-python-in-visual-studio.md
# https://docs.microsoft.com/en-us/cpp/build/reference/md-mt-ld-use-run-time-library?redirectedfrom=MSDN&view=msvc-160
#   Defines _DEBUG
# https://docs.python.org/3/c-api/init_config.html#isolated-configuration
#   Configuration files are still used with this configuration
# https://docs.microsoft.com/en-us/cpp/build/reference/linker-support-for-delay-loaded-dlls?view=msvc-160
# https://gitlab.kitware.com/cmake/cmake/-/issues/12372
#   DELAYLOAD
# https://docs.microsoft.com/en-us/windows/win32/dlls/dynamic-link-library-search-order?redirectedfrom=MSDN
#   SetDllDirectory NOTE messes with search path, can only have one
# .\Dependencies.exe -modules costrm.exe | Select-String -Pattern ".*boost.*" | % { $_ -replace ".* (.*\.dll).*", "`$1" }
# https://developercommunity.visualstudio.com/t/overriding-path-environment-variable-in-launchvsjs/710395
# https://blog.pcitron.fr/2018/05/30/visual-studio-2017-cmake-and-launch-settings/
#   launch.vs.json env vars PATH
# https://devblogs.microsoft.com/cppblog/customizing-your-environment-with-visual-c-and-open-folder/
#   ahaha the "env": "x=${env.y};foo\u0000y=bar" syntax actually appears in this ms blog
