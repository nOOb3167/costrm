function(costrm_backup_include FNAME)
	if(NOT EXISTS ${FNAME}_bkp)
		file(READ ${FNAME} OLD)
		file(WRITE ${FNAME}_bkp "${OLD}")
	endif()
endfunction()

function(costrm_clobber_include FNAME)
	file(READ ${FNAME} HHH)
	string(REGEX REPLACE
		"([ \\t]+)_Py_RefTotal((\\+\\+)|(\\-\\-));"
"#  ifndef COSTRM_OBJECT_H_DEF
\\1_Py_RefTotal\\2    /* COSTRM_CLOBBERED */ ;
#  endif // COSTRM_OBJECT_H_DEF"
		HHH "${HHH}")
	file(WRITE ${FNAME} "${HHH}")
endfunction()

function(forbidden_hebrew_sorcery000)
	if(NOT COSTRM_CLOBBER_INCLUDE)
		message(WARNING "not clobbering include files")
		return()
	endif()

	get_property(PYINC TARGET pyembed_target PROPERTY INTERFACE_INCLUDE_DIRECTORIES)

	# check only one include directory has the marker (object.h)

	foreach(INC IN LISTS PYINC)
		if(EXISTS ${INC}/object.h)
			list(APPEND OHS "${INC}")
		endif()
	endforeach()

	list(LENGTH OHS OHSL)

	if(NOT OHSL EQUAL 1)
		message(FATAL_ERROR "clobberable files")
	endif()

	# clobber but may backup the file first

	message(STATUS "clobbering [${OHS}/object.h]")

	costrm_backup_include(${OHS}/object.h)
	costrm_clobber_include(${OHS}/object.h)
endfunction()
