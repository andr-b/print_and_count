cmake_minimum_required(VERSION 3.1)
project(fibonacci_sum)

set(FIRST_VERSION_MAJOR 1)
set(FIRST_VERSION "${FIRST_VERSION_MAJOR}")

set(SOURCES main.cpp)
set(HEADERS)

add_subdirectory(
	"${CMAKE_CURRENT_SOURCE_DIR}/googletest"
	"googletest"
	)

add_executable(main ${SOURCES} ${HEADRS})
add_executable(test_main test/test.cpp)

target_include_directories(
	test_main
	PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}"
	)

set_target_properties(
	main PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	)

set_target_properties(
	test_main PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	)

target_compile_options(
	main 
	PRIVATE
	-lpthread 
	)

target_link_libraries(
	test_main
	PRIVATE
	gtest gtest_main)
target_link_libraries(
	main
	PRIVATE
	pthread
	)
