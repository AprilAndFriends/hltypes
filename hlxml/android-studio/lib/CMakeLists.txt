cmake_minimum_required(VERSION 3.4.1)
set(CMAKE_VERBOSE_MAKEFILE on)

project(hlxml)

include("../../../android-studio/generic/CMakeLists.txt")

file(
	GLOB_RECURSE CppSrc
	"../../src/*.c"
	"../../src/*.cpp"
)

add_library(hlxml STATIC ${CppSrc})

include_directories(AFTER "../../include/hlxml")
include_directories(AFTER "../../../include/hltypes")
include_directories(AFTER "../../../lib/rapidxml")

add_definitions(
	-DHLXML_EXPORTS
)

target_link_libraries(
	hlxml
)
