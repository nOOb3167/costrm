cmake_policy(VERSION 3.20)
cmake_minimum_required(VERSION 3.20)
project(costrm VERSION 3.20)

set(COSTRM_PYINC ${COSTRM_PYPATH}/include)
set(COSTRM_PYLIB ${COSTRM_PYPATH}/libs/python${COSTRM_PYVERS}.lib)

add_executable(costrm
	src/main.cpp
	)
target_include_directories(costrm PUBLIC
	${CMAKE_SOURCE_DIR}/src
	${COSTRM_PYINC}
	)
target_link_libraries(costrm
	${COSTRM_PYLIB}
)
target_compile_definitions(costrm PUBLIC PY_SSIZE_T_CLEAN)

add_custom_target(COSTRM_COPY_DATA
	COMMAND
		${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/py/ .
	WORKING_DIRECTORY
		${CMAKE_BINARY_DIR}
	)