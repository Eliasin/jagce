project(memlib VERSION 0.1 LANGUAGES CXX)

add_library(mem
	src/static_ram.cpp)

target_include_directories(mem
	PUBLIC
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>

	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(mem PRIVATE -Wall)
target_compile_features(mem PRIVATE cxx_std_17)

include(GNUInstallDirs)
install(TARGETS mem
    EXPORT mem-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

