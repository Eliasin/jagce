project(logiclib VERSION 0.1 LANGUAGES CXX)

add_library(logic
	src/byte_stream.cpp)

target_include_directories(logic
	PUBLIC
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>

	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(logic PRIVATE -Wall)
target_compile_features(logic PUBLIC cxx_std_17)

set_target_properties(logic
    PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_ROOT_DIRECTORY}/lib"
	LIBRARY_OUTPUT_DIRECTORY "${PROJECT_ROOT_DIRECTORY}/lib"
	RUNTIME_OUTPUT_DIRECTORY "${PROJECT_ROOT_DIRECTORY}/bin"
)

include(GNUInstallDirs)
install(TARGETS logic
    EXPORT logic-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

add_subdirectory(tests)
