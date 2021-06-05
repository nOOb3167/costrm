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
	set(LIBR ${PYPATH}/libs/python${VERS}.lib)
	set(LIBD ${PYPATH}/libs/python${VERS}_d.lib)
	set(DLLR ${PYPATH}/python${VERS}.dll)
	set(DLLD ${PYPATH}/python${VERS}_d.dll)
	costrm_check_file_exists(${LIBR})
	costrm_check_file_exists(${LIBD})
	costrm_check_file_exists(${DLLR})
	costrm_check_file_exists(${DLLD})

	set(PYLIBSTEM python${VERS}$<$<CONFIG:Debug>:_d>)

	add_library(pyembed_target SHARED IMPORTED)
	set_property(TARGET pyembed_target PROPERTY IMPORTED_IMPLIB_RELEASE "${LIBR}")
	set_property(TARGET pyembed_target PROPERTY IMPORTED_IMPLIB_DEBUG "${LIBD}")
	set_property(TARGET pyembed_target PROPERTY IMPORTED_LOCATION_RELEASE "${DLLR}")
	set_property(TARGET pyembed_target PROPERTY IMPORTED_LOCATION_DEBUG "${DLLD}")
	target_include_directories(pyembed_target INTERFACE ${PYPATH}/include)
	target_link_libraries(pyembed_target INTERFACE $<$<PLATFORM_ID:Windows>:delayimp.lib>    ${PYPATH}/libs/${PYLIBSTEM}.lib)
	target_compile_definitions(pyembed_target INTERFACE -DPY_SSIZE_T_CLEAN -DCOSTRM_OBJECT_H_DEF)
	target_link_options(pyembed_target INTERFACE $<$<PLATFORM_ID:Windows>:/DELAYLOAD:${PYLIBSTEM}.dll>)

	# set variables for substitutions

	set(COSTRM_PYPATH "${COSTRM_PYPATH}")
	set(COSTRM_PYVERS "${VERS}")

	configure_file(src/costrm.h.in costrm.h @ONLY)
	file(GENERATE OUTPUT costrm_aux.h CONTENT "#define COSTRM_PYDLL_NAME \"${PYLIBSTEM}.dll\"")

	# set variables for result

	set(COSTRM_PYPATH "${COSTRM_PYPATH}" PARENT_SCOPE)
endfunction()
