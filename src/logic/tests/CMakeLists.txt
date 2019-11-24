add_executable(logictest
	main.cpp
	byte_stream_tests.cpp
)

set_target_properties(logictest
    PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_ROOT_DIRECTORY}/lib"
	LIBRARY_OUTPUT_DIRECTORY "${PROJECT_ROOT_DIRECTORY}/lib"
	RUNTIME_OUTPUT_DIRECTORY "${PROJECT_ROOT_DIRECTORY}/bin"
)

find_package(Catch2 REQUIRED)
target_link_libraries(logictest logic Catch2::Catch2)

add_test(logictest logictest)
