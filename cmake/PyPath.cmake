function(costrm_check_file_exists F)
	message(STATUS "Checking for [${F}]")
	if(NOT EXISTS "${F}")
		message(FATAL_ERROR "Failed to find [${F}]")
	endif()
endfunction()

function(costrm_max_list A RESULT)
	list(REMOVE_DUPLICATES A)
	set(M "")
	foreach(C IN LISTS A)
		string(LENGTH "${C}" LC)
		string(LENGTH "${M}" LM)
		if (LC GREATER LM)
			set(M "${C}")
		endif()
	endforeach()
	set("${RESULT}" "${M}" PARENT_SCOPE)
endfunction()

function(costrm_get_pyvers PYPATH RESULT)
	file(GLOB CS
		 LIST_DIRECTORIES FALSE
		 RELATIVE ${PYPATH}
		 ${PYPATH}/python*.dll
		 )
	foreach(C IN LISTS CS)
		string(REGEX REPLACE "^python([0-9]+).*\\.dll" "\\1" D ${C})
		list(APPEND VERS ${D})
	endforeach()
	costrm_max_list("${VERS}" M)
	set("${RESULT}" "${M}" PARENT_SCOPE)
endfunction()

function(costrm_define_pyvars PYPATH)
	file(TO_CMAKE_PATH  "${PYPATH}" PYPATH)

	message(STATUS "Checking Python Path [${PYPATH}]")
	costrm_get_pyvers(${PYPATH} VERS)
	message(STATUS "Found Python Version [${VERS}]")
	set(LIBR ${PYPATH}/python${VERS}.dll)
	set(LIBD ${PYPATH}/python${VERS}_d.dll)
	costrm_check_file_exists(${LIBR})
	costrm_check_file_exists(${LIBD})

	set(PYLIBSTEM python${VERS}$<$<CONFIG:Debug>:_d>)

	set(COSTRM_PYPATH "${COSTRM_PYPATH}" PARENT_SCOPE)
	set(COSTRM_PYVERS "${VERS}" PARENT_SCOPE)
	set(COSTRM_PYINC ${PYPATH}/include PARENT_SCOPE)
	set(COSTRM_PYLIB ${PYPATH}/libs/${PYLIBSTEM}.lib PARENT_SCOPE)
	set(COSTRM_PYDLL_D_PATH "${LIBD}" PARENT_SCOPE)
	set(COSTRM_PYDLL_NAME "${PYLIBSTEM}.dll" PARENT_SCOPE)
endfunction()
