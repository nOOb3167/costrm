# https://bitbucket.org/chromiumembedded/cef/issues/2120/windows-warnings-building-cefclient-with
#   some joker PRed a change clearing out CMAKE_CXX_FLAGS variables and it got through
#   but wasnt smart enough to use the CACHE option so the set statements are directory-scoped

macro(costrm_ensure_cefthree)
	include(FetchContent)

	FetchContent_Declare(cefthree
		URL https://cef-builds.spotifycdn.com/cef_binary_90.6.7+g19ba721+chromium-90.0.4430.212_windows64.tar.bz2
		URL_HASH SHA256=5E7902B2A24D74F335B74B07328EB51599AD79A108905593BE5F95EBC7440D54
		SOURCE_DIR cef
	)

	FetchContent_GetProperties(cefthree)
	if(NOT cefthree_POPULATED)
		message(STATUS "cefthree populate")

		FetchContent_Populate(cefthree)
	endif()
endmacro()
