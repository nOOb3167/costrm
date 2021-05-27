cmake_policy(VERSION 3.20)
cmake_minimum_required(VERSION 3.20)
project(costrm VERSION 3.20)

file(TO_CMAKE_PATH ${COSTRM_PYPATH} COSTRM_PYPATH)

set(COSTRM_PYINC ${COSTRM_PYPATH}/include)
set(COSTRM_PYLIB ${COSTRM_PYPATH}/libs/python${COSTRM_PYVERS}.lib)

configure_file(src/costrm.h.in costrm.h @ONLY)

add_executable(costrm
	src/main.cpp
	)
target_include_directories(costrm PUBLIC
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_BINARY_DIR}
	${COSTRM_PYINC}
	)
target_link_libraries(costrm
	${COSTRM_PYLIB}
)
target_compile_definitions(costrm PUBLIC -DPY_SSIZE_T_CLEAN)

add_custom_target(COSTRM_COPY_DATA
	COMMAND
		${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/py/ .
	WORKING_DIRECTORY
		${CMAKE_BINARY_DIR}
	)

# https://github.com/MicrosoftDocs/visualstudio-docs/issues/6425
# https://github.com/MicrosoftDocs/visualstudio-docs/blob/master/docs/python/working-with-c-cpp-python-in-visual-studio.md
# https://docs.microsoft.com/en-us/cpp/build/reference/md-mt-ld-use-run-time-library?redirectedfrom=MSDN&view=msvc-160
#   Defines _DEBUG
# https://docs.python.org/3/c-api/init_config.html#isolated-configuration
#   Configuration files are still used with this configuration
