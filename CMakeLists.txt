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

add_custom_target(COSTRM_COPY_DLLS COMMAND
	${CMAKE_EXECUTABLE} -E echo ${CMAKE_SOURCE_DIR}/py/python${COSTRM_PYVERS}.dll ${CMAKE_BINARY_DIR}}
	)