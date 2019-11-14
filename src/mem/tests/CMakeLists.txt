add_executable(memtest
	main.cpp
	static_ram_test.cpp
)

target_link_libraries(memtest mem)

add_test(memtest memtest)
