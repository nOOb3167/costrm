function(costrm_clobber_include FNAME)
	file(READ ${FNAME} HHH)
	string(REGEX REPLACE
		"([ \\t]+)_Py_RefTotal((\\+\\+)|(\\-\\-));"
"#  ifndef COSTRM_OBJECT_H_DEF
\\1_Py_RefTotal\\2    /* do_not_replace_twice */ ;
#  endif // COSTRM_OBJECT_H_DEF"
		HHH "${HHH}")
	file(WRITE ${FNAME} "${HHH}")
endfunction()

function(forbidden_hebrew_sorcery000 PYINC)
	if(NOT DEFINED COSTRM_CLOBBER_INCLUDE)
		message(WARNING "not clobbering include files")
		return()
	endif()

	if(NOT EXISTS ${PYINC}/object.h_bkp)
		file(READ ${PYINC}/object.h OLD)
		file(WRITE ${PYINC}/object.h_bkp "${OLD}")
	endif()

	costrm_clobber_include(${PYINC}/object.h)
endfunction()
