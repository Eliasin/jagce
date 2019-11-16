add_executable(memtest
	main.cpp
	static_ram_test.cpp
)

find_package(Catch2 REQUIRED)
target_link_libraries(memtest mem Catch2::Catch2)

add_test(memtest memtest)
