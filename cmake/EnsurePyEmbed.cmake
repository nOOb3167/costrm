macro(costrm_ensure_pyembed PYDLL_D_PATH)
	include(FetchContent)

	FetchContent_Declare(pyembed
		URL https://www.python.org/ftp/python/3.9.5/python-3.9.5-embed-amd64.zip
		URL_HASH SHA256=F6954B64AF18386C523988A23C512452FD289E3591218E7DBB76589B9B326D34
		SOURCE_DIR py
	)

	FetchContent_GetProperties(pyembed)
	if(NOT pyembed_POPULATED)
		FetchContent_Populate(pyembed)
		
		file(COPY "${PYDLL_D_PATH}" DESTINATION py)
	endif()
endmacro()
