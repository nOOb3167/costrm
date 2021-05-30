function(get_target_boost_dlls tgt config result_name)
	string (TOUPPER ${config} config)

	set(dlls "")

	get_property(ill TARGET ${tgt} PROPERTY INTERFACE_LINK_LIBRARIES)

	foreach(v ${ill})
		get_target_boost_dlls_2(${v} ${config} res2)
		list(APPEND dlls ${res2})
	endforeach()

	set(dlls2 "")

	foreach(d ${dlls})
		string(REGEX REPLACE "[.]lib$" ".dll" res2 ${d})
		list(APPEND dlls2 ${res2})
	endforeach()

	foreach(d ${dlls2})
		if(NOT EXISTS ${d})
			message(WARNING "Not found [${d}]")
		endif()
	endforeach()

	list(REMOVE_DUPLICATES dlls2)

	set(${result_name} ${dlls2} PARENT_SCOPE)
endfunction()

function(get_target_boost_dlls_2 tgt config result_name)
	set(dlls "")

	if(tgt MATCHES "^Boost::")
		get_property(a TARGET ${tgt} PROPERTY IMPORTED_IMPLIB_${config})
		list(APPEND dlls ${a})

		get_property(ill TARGET ${tgt} PROPERTY INTERFACE_LINK_LIBRARIES)

		foreach(v ${ill})
			if(v MATCHES "^Boost::")
				get_property(a TARGET ${v} PROPERTY IMPORTED_IMPLIB_${config})
				list(APPEND dlls ${a})
			endif()
		endforeach()
	endif()

	set(${result_name} ${dlls} PARENT_SCOPE)
endfunction()
