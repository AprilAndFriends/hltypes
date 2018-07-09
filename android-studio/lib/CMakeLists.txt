cmake_minimum_required(VERSION 3.4.1)
set(CMAKE_VERBOSE_MAKEFILE on)

project(hltypes)

include("../generic/CMakeLists.txt")

file(
	GLOB_RECURSE CppSrc
	"../../src/*.c"
	"../../src/*.cpp"
	"../../lib/miniz/*.cpp"
)

add_library(hltypes STATIC ${CppSrc})

include_directories(AFTER "../../include/hltypes")
include_directories(AFTER "../../lib/miniz")

add_definitions(
	-DHLTYPES_EXPORTS
	-D_ZIPRESOURCE
)

target_link_libraries(
	hltypes
)
