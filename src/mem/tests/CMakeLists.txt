add_executable(memtest
	main.cpp
	static_ram_test.cpp
)

set_target_properties(memtest
    PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_ROOT_DIRECTORY}/lib"
	LIBRARY_OUTPUT_DIRECTORY "${PROJECT_ROOT_DIRECTORY}/lib"
	RUNTIME_OUTPUT_DIRECTORY "${PROJECT_ROOT_DIRECTORY}/bin"
)

find_package(Catch2 REQUIRED)
target_link_libraries(memtest mem Catch2::Catch2)

add_test(memtest memtest)
