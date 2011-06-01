/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @author  Ivan Vucica
/// @version 1.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Provides high level utility methods.

#ifndef HLTYPES_UTIL_H
#define HLTYPES_UTIL_H

#include <math.h>

#include "hltypesExport.h"
#include "hstring.h"

/// @brief Rounds a double value to the nearest integer value.
/// @param[in] value The value to be rounded.
/// @return Rounded value as double.
hltypesFnExport double hroundd(double value);
/// @brief Rounds a double value to the nearest integer value.
/// @param[in] value The value to be rounded.
/// @return Rounded value as float.
hltypesFnExport float hroundf(float value);
/// @brief Rounds a double value to the nearest integer value.
/// @param[in] value The value to be rounded.
/// @return Rounded value as integer.
hltypesFnExport int hround(double value);
/// @brief Used for optimized and quick calculation from RAD to DEG.
#define HL_RAD_TO_DEG_RATIO 57.295779513082320876798154814105
/// @brief Used for optimized and quick calculation from DEG to RAD.
#define HL_DEG_TO_RAD_RATIO 0.01745329251994329576923690768489

/// @brief Calculates sin from angle given in degrees.
/// @param[in] degrees Angle in degrees.
/// @return sin(degrees).
#define dsin(degrees) sin((degrees) * HL_DEG_TO_RAD_RATIO)
/// @brief Calculates cos from angle given in degrees.
/// @param[in] degrees Angle in degrees.
/// @return cos(degrees).
#define dcos(degrees) cos((degrees) * HL_DEG_TO_RAD_RATIO)
/// @brief Calculates asin in degrees.
/// @param[in] value sin value.
/// @return asin in degrees.
#define dasin(value) (asin(value) * HL_RAD_TO_DEG_RATIO)
/// @brief Calculates acos in degrees.
/// @param[in] value cos value.
/// @return acos in degrees.
#define dacos(value) (acos(value) * HL_RAD_TO_DEG_RATIO)
/// @brief hltypes e-tolerance.
#define HL_E_TOLERANCE (0.01f)

/// @brief Returns a random int number.
/// @param[in] min Inclusive lower boundary.
/// @param[in] max Exclusive upper boundary.
/// @return Random number between min inclusively and max exclusively.
hltypesFnExport int hrand(int min, int max);
/// @brief Returns a random int number.
/// @param[in] max Exclusive upper boundary.
/// @return Random number between 0 inclusively and max exclusively.
hltypesFnExport int hrand(int max);
/// @brief Returns a random float number.
/// @param[in] min Inclusive lower boundary.
/// @param[in] max Exclusive upper boundary.
/// @return Random number between min inclusively and max exclusively.
hltypesFnExport float hrandf(float min, float max);
/// @brief Returns a random float number.
/// @param[in] max Exclusive upper boundary.
/// @return Random number between 0.0 inclusively and max exclusively.
hltypesFnExport float hrandf(float max);
/// @brief Normalizes a file path by converting all platform specific characters into / and proper removal of "." and ".." where necessary.
/// @param[in] path The path.
/// @return Normalized filepath.
hltypesFnExport hstr normalize_path(chstr path);
/// @brief Converts a unicode unsigned int to a UTF8 string.
/// @param[in] value The unsigned int value.
/// @return UTF8 string.
hltypesFnExport hstr unicode_to_utf8(unsigned int value);
/// @brief Converts a unicode unsigned int string to a UTF8 string.
/// @param[in] string The unsigned int string.
/// @return UTF8 string.
hltypesFnExport hstr unicode_to_utf8(const unsigned int* string);
/// @brief Converts a unicode wchar to a UTF8 string.
/// @param[in] value The wchar value.
/// @return UTF8 string.
hltypesFnExport hstr unicode_to_utf8(wchar_t value);
/// @brief Converts a unicode wchar string to a UTF8 string.
/// @param[in] string The wchar string.
/// @return UTF8 string.
hltypesFnExport hstr unicode_to_utf8(const wchar_t* string);
/// @brief Converts a UTF8 character into the corresponding character code.
/// @param[in] input The UTF8 character as C string.
/// @param[out] character_length Length of character in bytes.
/// @return Charcter code.
hltypesFnExport unsigned int utf8_to_uint(chstr input, int* character_length = NULL);
/// @brief Converts a UTF8 string into a unicode wchar string.
/// @param[in] input The UTF8 string.
/// @param[out] lenght Length of the string.
/// @return The unsiend int string.
/// @note Make sure to use "delete []" on the result to prevent memory leaks.
hltypesFnExport unsigned int* utf8_to_unicode(chstr input, int* length = NULL);

/// @brief Returns the lesser of two elements.
/// @param[in] a First element.
/// @param[in] b Second element.
/// @return The lesser of two elements.
template <class T> T hmin(T a, T b)
{
	return (a < b ? a : b);
}
/// @brief Returns the greater of two elements.
/// @param[in] a First element.
/// @param[in] b Second element.
/// @return The greater of two elements.
template <class T> T hmax(T a, T b)
{
	return (a > b ? a : b);
}
/// @brief Clamps a value between two other values.
/// @param[in] value The element to clamp.
/// @param[in] min Minimum inclusive boundary.
/// @param[in] max Maximum inclusive boundary.
/// @return Clamped value.
template <class T> T hclamp(T value, T min, T max)
{
	return (value < min ? min : (value > max ? max : value));
}
/// @brief Swaps the values of two elements.
/// @param[in,out] a First element.
/// @param[in,out] b Second element.
template <class T> void hswap(T& a, T& b)
{
	T temp = a;
	a = b;
	b = temp;
}
/// @brief Returns Signum of the value.
/// @param[in] value The value.
/// @return Signum of the value.
template <class T> int sgn(T value)
{
	return (value >= 0 ? 1 : -1);
}
/// @brief Checks whether an element is within the range of two other elements.
/// @param[in] value The element to clamp.
/// @param[in] min Minimum inclusive boundary.
/// @param[in] max Maximum inclusive boundary.
/// @return True if element is between minimum and maximum.
template <class T> bool is_between(T value, T min, T max)
{
	return (value >= min && value <= max);
}

/// @brief Debug macro.
/// @todo Implement system("pause") in a nicer, crossplatform way.
#define HLBREAKPT \
	printf("HL Breakpoint:\n"); \
	printf(" -> File: %s\n", __FILE__); \
	printf(" -> Line: %d\n", __LINE__); \
	printf(" -> Func: %s\n", __PRETTY_FUNCTION__); \
	system("pause");

#endif
