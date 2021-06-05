macro(costrm_ensure_pyembed)
	include(FetchContent)

	FetchContent_Declare(pyembed
		URL https://www.python.org/ftp/python/3.9.5/python-3.9.5-embed-amd64.zip
		URL_HASH SHA256=F6954B64AF18386C523988A23C512452FD289E3591218E7DBB76589B9B326D34
		SOURCE_DIR py
	)

	FetchContent_GetProperties(pyembed)
	if(NOT pyembed_POPULATED)
		message(STATUS "pyembed populate")

		FetchContent_Populate(pyembed)

		get_property(PYDLL_D_PATH TARGET pyembed_target PROPERTY IMPORTED_LOCATION_DEBUG)
		
		message(STATUS "pyembed patching")
		message(STATUS "pyembed copying [${PYDLL_D_PATH}]")

		file(COPY "${PYDLL_D_PATH}" DESTINATION py)
	endif()
endmacro()
