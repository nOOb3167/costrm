function(costrm_clobber_include FNAME)
	file(READ ${FNAME} HHH)
	string(REGEX REPLACE
		"\"object.h\""
		"<object.h>"
		HHH "${HHH}")
	file(WRITE ${FNAME} "${HHH}")
endfunction()

function(forbidden_hebrew_sorcery000 PYINC)
	if(NOT DEFINED COSTRM_CLOBBER_INCLUDE)
		message(WARNING "not clobbering include files")
		return()
	endif()

	costrm_clobber_include(${PYINC}/Python.h)
	costrm_clobber_include(${PYINC}/pytime.h)

	file(READ ${PYINC}/object.h OBJECTH)
	string(REGEX REPLACE
		"_Py_RefTotal((\\+\\+)|(\\-\\-))"
		"//COSTRM_EDITED _Py_RefTotal(\\1)"
		OBJECTH2 "${OBJECTH}")
	file(WRITE ${CMAKE_BINARY_DIR}/object.h_ "${OBJECTH2}")

	file(GENERATE OUTPUT ${CMAKE_BINARY_DIR}/object.h INPUT ${CMAKE_BINARY_DIR}/object.h_)
endfunction()
