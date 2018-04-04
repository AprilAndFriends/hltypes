# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

file (GLOB_RECURSE CppSrc
    "../../include/*.h"
    "../../src/*.h"
    "../../src/*.cpp"
)

add_definitions(
	-D_ZIPRESOURCE
)

add_library(
	hltypes
	STATIC
	${CppSrc}
)

find_library(log-lib log)

target_link_libraries(
	hltypes
	${log-lib}
)
