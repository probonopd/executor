find_library(X11_LIBRARY X11)
find_library(Xext_LIBRARY Xext)
find_path (X11_INCLUDE_PATH X11/Xresource.h)

if(X11_LIBRARY AND Xext_LIBRARY)

	add_library(front-end-x
            default_vdriver.h
			x.cpp
			x_keycodes.cpp
			x.h
            x_keycodes.h
            sigio_multiplex.h
            sigio_multiplex.cpp
		)
	target_link_libraries(front-end-x syn68k ${X11_LIBRARY} ${Xext_LIBRARY})
	target_include_directories(front-end-x PUBLIC . ${X11_INCLUDE_PATH})


        # X11 front-end is legacy code.
        # it doesn't pay to fix this.
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(front-end-x PRIVATE -Wno-writable-strings)
    elseif(CMAKE_COMPILER_IS_GNUCXX)
        target_compile_options(front-end-x PRIVATE -Wno-write-strings)
    endif()
endif()