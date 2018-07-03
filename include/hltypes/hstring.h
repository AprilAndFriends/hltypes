/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Provides high level utility methods.

#ifndef HLTYPES_STRING_H
#define HLTYPES_STRING_H

#include <stdarg.h>
#include <stdint.h>
#include <string>

#include "hltypesExport.h"

#if defined(_MSC_VER) && _MSC_VER >= 1600 && defined(_WIN32)
/// @brief Circumvents a compilation problem in VS 2010 and higher
const std::string::size_type std::string::npos = size_t(-1);
#endif

#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace std
{
	typedef std::basic_string<unsigned int, std::char_traits<unsigned int>, std::allocator<unsigned int> > ustring;
}
#endif

namespace hltypes
{
	template <typename T> class Array;
	
	/// @brief Encapsulates std::string and adds high level methods.
	class hltypesExport String : std::basic_string<char>
	{
	public:
		/// @brief Empty constructor.
		String();
		/// @brief Basic constructor.
		/// @param[in] c A character.
		String(const char c);
		/// @brief Basic constructor.
		/// @param[in] c A character.
		/// @param[in] times How many times c should be added.
		String(const char c, const int times);
		/// @brief Copy constructor.
		/// @param[in] string A C-type string.
		String(const char* string);
		/// @brief Copy constructor.
		/// @param[in] string String to copy.
		String(const String& string);
		/// @brief Copy constructor.
		/// @param[in] string A C-type string.
		/// @param[in] length How many characters to copy.
		String(const char* string, const int length);
		/// @brief Copy constructor.
		/// @param[in] string String to copy.
		/// @param[in] length How many characters to copy.
		String(const String& string, const int length);
		/// @brief Type constructor.
		/// @param[in] b Bool to create String of.
		String(const bool b);
		/// @brief Type constructor.
		/// @param[in] s Short integer to create String of.
		String(const short s);
		/// @brief Type constructor.
		/// @param[in] s Unsigned short integer to create String of.
		String(const unsigned short s);
		/// @brief Type constructor.
		/// @param[in] i Integer to create String of.
		String(const int i);
		/// @brief Type constructor.
		/// @param[in] i Unsigned integer to create String of.
		String(const unsigned int i);
		/// @brief Type constructor.
		/// @param[in] i 64-bit integer to create String of.
		String(const int64_t i);
		/// @brief Type constructor.
		/// @param[in] i Unsigned 64-bit integer to create String of.
		String(const uint64_t i);
		/// @brief Type constructor.
		/// @param[in] f Float to create String of.
		String(const float f);
		/// @brief Type constructor.
		/// @param[in] f Float to create String of.
		/// @param[in] precision The floating point precision to use.
		String(const float f, int precision);
		/// @brief Type constructor.
		/// @param[in] d Double to create String of.
		String(const double d);
		/// @brief Type constructor.
		/// @param[in] d Double to create String of.
		/// @param[in] precision The floating point precision to use.
		String(const double d, int precision);
		/// @brief Destructor.
		~String();

		/// @brief Sets a character as C-type string.
		/// @param[in] c A character.
		void set(const char c);
		/// @brief Sets a character as C-type string.
		/// @param[in] c A character.
		/// @param[in] times How many times c should be added.
		void set(const char c, const int times);
		/// @brief Sets String as C-type string.
		/// @param[in] string C-type string value.
		void set(char* string);
		/// @brief Sets String as C-type string.
		/// @param[in] string C-type string value.
		/// @param[in] length How many characters to copy.
		void set(char* string, const int length);
		/// @brief Sets String as C-type string.
		/// @param[in] string C-type string value.
		void set(const char* string);
		/// @brief Sets String as C-type string.
		/// @param[in] string C-type string value.
		/// @param[in] length How many characters to copy.
		void set(const char* string, const int length);
		/// @brief Sets String as other String.
		/// @param[in] string String value.
		void set(const String& string);
		/// @brief Sets String as other String.
		/// @param[in] string String value.
		/// @param[in] length How many characters to copy.
		void set(const String& string, const int length);
		/// @brief Sets String as bool.
		/// @param[in] b Bool value.
		void set(const bool b);
		/// @brief Sets String as short.
		/// @param[in] s Short value.
		void set(const short s);
		/// @brief Sets String as unsigned short.
		/// @param[in] s Unsigned short value.
		void set(const unsigned short s);
		/// @brief Sets String as int.
		/// @param[in] i Int value.
		void set(const int i);
		/// @brief Sets String as unsigned int.
		/// @param[in] i Unsigned int value.
		void set(const unsigned int i);
		/// @brief Sets String as 64-bit int.
		/// @param[in] i 64-bit int value.
		void set(const int64_t i);
		/// @brief Sets String as unsigned 64-bit int.
		/// @param[in] i Unsigned 64-bit int value.
		void set(const uint64_t i);
		/// @brief Sets String as float.
		/// @param[in] f Float value.
		void set(const float f);
		/// @brief Sets String as float.
		/// @param[in] f Float to create String of.
		/// @param[in] precision The floating point precision to use.
		void set(const float f, int precision);
		/// @brief Sets String as double.
		/// @param[in] d Double value.
		void set(const double d);
		/// @brief Sets String as double.
		/// @param[in] d Double to create String of.
		/// @param[in] precision The floating point precision to use.
		void set(const double d, int precision);

		/// @brief Converts char into a String and concatenates the new String at the end of this one.
		/// @param[in] c Character value.
		void add(const char c);
		/// @brief Converts char into a String and concatenates the new String at the end of this one.
		/// @param[in] c Character value.
		/// @param[in] times How many times the characters should be added.
		void add(const char c, int times);
		/// @brief Converts C-type string into a String and concatenates the new String at the end of this one.
		/// @param[in] string C-type string value.
		void add(char* string);
		/// @brief Converts C-type string into a String and concatenates the new String at the end of this one.
		/// @param[in] string C-type string value.
		/// @param[in] length Length of the C-type string.
		void add(char* string, int length);
		/// @brief Converts C-type string into a String and concatenates the new String at the end of this one.
		/// @param[in] string C-type string value.
		void add(const char* string);
		/// @brief Converts C-type string into a String and concatenates the new String at the end of this one.
		/// @param[in] string C-type string value.
		/// @param[in] length Length of the C-type string.
		void add(const char* string, int length);
		/// @brief Concatenates a String at the end of this one.
		/// @param[in] string String value.
		void add(const String& string);
		/// @brief Concatenates a String at the end of this one.
		/// @param[in] string String value.
		/// @param[in] length Length of the String.
		void add(const String& string, int length);
		/// @brief Converts bool into a String and concatenates the new String at the end of this one.
		/// @param[in] b Bool value.
		void add(const bool b);
		/// @brief Converts short into a String and concatenates the new String at the end of this one.
		/// @param[in] s Short value.
		void add(const short s);
		/// @brief Converts unsigned short into a String and concatenates the new String at the end of this one.
		/// @param[in] s Unsigned short value.
		void add(const unsigned short s);
		/// @brief Converts int into a String and concatenates the new String at the end of this one.
		/// @param[in] i Int value.
		void add(const int i);
		/// @brief Converts unsigned int into a String and concatenates the new String at the end of this one.
		/// @param[in] i Unsigned int value.
		void add(const unsigned int i);
		/// @brief Converts 64-bit int into a String and concatenates the new String at the end of this one.
		/// @param[in] i 64-bit int value.
		void add(const int64_t i);
		/// @brief Converts unsigned 64-bit int into a String and concatenates the new String at the end of this one.
		/// @param[in] i Unsigned 64-bit int value.
		void add(const uint64_t i);
		/// @brief Converts float into a String and concatenates the new String at the end of this one.
		/// @param[in] f Float value.
		void add(const float f);
		/// @brief Converts float into a String and concatenates the new String at the end of this one.
		/// @param[in] f Float value.
		/// @param[in] precision The floating point precision to use.
		void add(const float f, int precision);
		/// @brief Converts double into a String and concatenates the new String at the end of this one.
		/// @param[in] d Double value.
		void add(const double d);
		/// @brief Converts double into a String and concatenates the new String at the end of this one.
		/// @param[in] d Double value.
		/// @param[in] precision The floating point precision to use.
		void add(const double d, int precision);
		/// @brief Transforms String into lower case.
		/// @return String in lower case.
		String lowered() const;
		/// @brief Transforms String into upper case.
		/// @return String in upper case.
		String uppered() const;
		/// @brief Reverses String.
		/// @return Reversed String.
		String reversed() const;
		/// @brief Reverses UTF-8 string.
		/// @return Reversed UTF-8 string.
		String utf8Reversed() const;
		/// @brief Left-trims and right-trims String from a specific character.
		/// @param[in] c Character to trim.
		/// @return Trimmed String.
		String trimmed(const char c = ' ') const;
		/// @brief Left-trims String from a specific character.
		/// @param[in] c Character to trim.
		/// @return Left-trimmed String.
		String trimmedLeft(const char c = ' ') const;
		/// @brief Right-trims String from a specific character.
		/// @param[in] c Character to trim.
		/// @return Right-trimmed String.
		String trimmedRight(const char c = ' ') const;
		/// @brief Replaces occurrences of a substring with another substring.
		/// @param[in] what String substring.
		/// @param[in] withWhat String substitution.
		void replace(const String& what, const String& withWhat);
		/// @brief Replaces occurrences of a substring with another substring.
		/// @param[in] what String substring.
		/// @param[in] withWhat String substitution.
		/// @param[in] times How many times to replace the string maximally.
		void replace(const String& what, const String& withWhat, int times);
		/// @brief Replaces occurrences of a substring with another substring.
		/// @param[in] what String substring.
		/// @param[in] withWhat C-string string substitution.
		void replace(const String& what, const char* withWhat);
		/// @brief Replaces occurrences of a substring with another substring.
		/// @param[in] what String substring.
		/// @param[in] withWhat C-string string substitution.
		/// @param[in] times How many times to replace the string maximally.
		void replace(const String& what, const char* withWhat, int times);
		/// @brief Replaces occurrences of a substring with a character.
		/// @param[in] what String substring.
		/// @param[in] withWhat Character substitution.
		/// @param[in] times How many times to insert the character.
		void replace(const String& what, const char withWhat, int times = 1);
		/// @brief Replaces occurrences of a substring with another substring.
		/// @param[in] what C-string substring.
		/// @param[in] withWhat String substitution.
		void replace(const char* what, const String& withWhat);
		/// @brief Replaces occurrences of a substring with another substring.
		/// @param[in] what C-string substring.
		/// @param[in] withWhat String substitution.
		/// @param[in] times How many times to replace the string maximally.
		void replace(const char* what, const String& withWhat, int times);
		/// @brief Replaces occurrences of a substring with another substring.
		/// @param[in] what C-string substring.
		/// @param[in] withWhat C-string string substitution.
		void replace(const char* what, const char* withWhat);
		/// @brief Replaces occurrences of a substring with another substring.
		/// @param[in] what C-string substring.
		/// @param[in] withWhat C-string string substitution.
		/// @param[in] times How many times to replace the string maximally.
		void replace(const char* what, const char* withWhat, int times);
		/// @brief Replaces occurrences of a substring with a character.
		/// @param[in] what C-string substring.
		/// @param[in] withWhat Character substitution.
		/// @param[in] times How many times to insert the character.
		void replace(const char* what, const char withWhat, int times = 1);
		/// @brief Replaces occurrences of a character with a substring.
		/// @param[in] what Character substring.
		/// @param[in] withWhat String substitution.
		void replace(const char what, const String& withWhat);
		/// @brief Replaces occurrences of a character with a substring.
		/// @param[in] what Character substring.
		/// @param[in] withWhat C-string string substitution.
		void replace(const char what, const char* withWhat);
		/// @brief Replaces occurrences of a character with a substring.
		/// @param[in] what Character substring.
		/// @param[in] withWhat Character substitution.
		/// @param[in] times How many times to insert the character.
		void replace(const char what, const char withWhat, int times = 1);
		/// @brief Replaces a sequence of characters with a substring.
		/// @param[in] position Start index of character sequence.
		/// @param[in] count How many characters to replace.
		/// @param[in] string String substitution.
		void replace(int position, int count, const String& string);
		/// @brief Replaces a sequence of characters with a substring.
		/// @param[in] position Start index of character sequence.
		/// @param[in] count How many characters to replace.
		/// @param[in] string C-string substitution.
		void replace(int position, int count, const char* string);
		/// @brief Replaces a sequence of characters with a substring.
		/// @param[in] position Start index of character sequence.
		/// @param[in] count How many characters to replace.
		/// @param[in] character Character substitution.
		/// @param[in] times How many times to insert the character.
		void replace(int position, int count, const char character, int times = 1);
		/// @brief Replaces occurrences of a substring with another substring.
		/// @param[in] what String substring.
		/// @param[in] withWhat String substitution.
		/// @return New String.
		String replaced(const String& what, const String& withWhat) const;
		/// @brief Replaces occurrences of a substring with another substring.
		/// @param[in] what String substring.
		/// @param[in] withWhat C-string string substitution.
		/// @return New String.
		String replaced(const String& what, const char* withWhat) const;
		/// @brief Replaces occurrences of a substring with a character.
		/// @param[in] what String substring.
		/// @param[in] withWhat Character substitution.
		/// @param[in] times How many times to insert the character.
		/// @return New String.
		String replaced(const String& what, const char withWhat, int times = 1) const;
		/// @brief Replaces occurrences of a substring with another substring.
		/// @param[in] what C-string substring.
		/// @param[in] withWhat String substitution.
		/// @return New String.
		String replaced(const char* what, const String& withWhat) const;
		/// @brief Replaces occurrences of a substring with another substring.
		/// @param[in] what C-string substring.
		/// @param[in] withWhat C-string string substitution.
		/// @return New String.
		String replaced(const char* what, const char* withWhat) const;
		/// @brief Replaces occurrences of a substring with a character.
		/// @param[in] what C-string substring.
		/// @param[in] withWhat Character substitution.
		/// @param[in] times How many times to insert the character.
		/// @return New String.
		String replaced(const char* what, const char withWhat, int times = 1) const;
		/// @brief Replaces occurrences of a character with a substring.
		/// @param[in] what Character substring.
		/// @param[in] withWhat String substitution.
		/// @return New String.
		String replaced(const char what, const String& withWhat) const;
		/// @brief Replaces occurrences of a character with a substring.
		/// @param[in] what Character substring.
		/// @param[in] withWhat C-string string substitution.
		/// @return New String.
		String replaced(const char what, const char* withWhat) const;
		/// @brief Replaces occurrences of a character with a substring.
		/// @param[in] what Character substring.
		/// @param[in] withWhat Character substitution.
		/// @param[in] times How many times to insert the character.
		/// @return New String.
		String replaced(const char what, const char withWhat, int times = 1) const;
		/// @brief Replaces a sequence of characters with a substring.
		/// @param[in] position Start index of character sequence.
		/// @param[in] count How many characters to replace.
		/// @param[in] string String substitution.
		/// @return New String.
		String replaced(int position, int count, const String& string) const;
		/// @brief Replaces a sequence of characters with a substring.
		/// @param[in] position Start index of character sequence.
		/// @param[in] count How many characters to replace.
		/// @param[in] string C-string substitution.
		/// @return New String.
		String replaced(int position, int count, const char* string) const;
		/// @brief Replaces a sequence of characters with a substring.
		/// @param[in] position Start index of character sequence.
		/// @param[in] count How many characters to replace.
		/// @param[in] character Character substitution.
		/// @param[in] times How many times to insert the character.
		/// @return New String.
		String replaced(int position, int count, const char character, int times = 1) const;
		/// @brief Inserts a sequence of characters into the String.
		/// @param[in] position Index where to insert.
		/// @param[in] string String to insert.
		void insertAt(int position, const String& string);
		/// @brief Inserts a sequence of characters into the String.
		/// @param[in] position Index where to insert.
		/// @param[in] string String to insert.
		void insertAt(int position, const char* string);
		/// @brief Inserts a character into the String.
		/// @param[in] position Index where to insert.
		/// @param[in] character Character to insert.
		/// @param[in] times How many times to insert the character.
		void insertAt(int position, const char character, int times = 1);
		/// @brief Inserts a sequence of characters into the String.
		/// @param[in] position Index where to insert.
		/// @param[in] string String to insert.
		/// @return New String.
		String insertedAt(int position, const String& string) const;
		/// @brief Inserts a sequence of characters into the String.
		/// @param[in] position Index where to insert.
		/// @param[in] string String to insert.
		/// @return New String.
		String insertedAt(int position, const char* string) const;
		/// @brief Inserts a character into the String.
		/// @param[in] position Index where to insert.
		/// @param[in] character Character to insert.
		/// @param[in] times How many times to insert the character.
		/// @return New String.
		String insertedAt(int position, const char character, int times = 1) const;
		/// @brief Randomizes the String's characters.
		void randomize();
		/// @brief Randomizes the UTF-8 String's characters.
		void utf8Randomize();
		/// @brief Creates a randomized copy of the String's characters.
		/// @return New String.
		String randomized() const;
		/// @brief Creates a randomized copy of the String's characters.
		/// @return New String.
		String utf8Randomized() const;
		/// @brief Splits the String with the delimiter once.
		/// @param[in] delimiter The character acting as splitting delimiter.
		/// @param[out] outLeft First portion of the split String.
		/// @param[out] outRight Second portion of the split String.
		/// @return True if String was split.
		bool split(const char delimiter, String& outLeft, String& outRight) const;
		/// @brief Splits the String with the delimiter once.
		/// @param[in] delimiter The C-type string acting as splitting delimiter.
		/// @param[out] outLeft First portion of the split String.
		/// @param[out] outRight Second portion of the split String.
		/// @return True if String was split.
		bool split(const char* delimiter, String& outLeft, String& outRight) const;
		/// @brief Splits the String with the delimiter once.
		/// @param[in] delimiter The String acting as splitting delimiter.
		/// @param[out] outLeft First portion of the split String.
		/// @param[out] outRight Second portion of the split String.
		/// @return True if String was split.
		bool split(const String& delimiter, String& outLeft, String& outRight) const;
		/// @brief Splits all characters in the String.
		/// @return Array of chars.
		Array<char> split() const;
		/// @brief Splits the String with the delimiter.
		/// @param[in] delimiter The character acting as splitting delimiter.
		/// @param[in] times How many times the string should be split maximally.
		/// @param[in] removeEmpty Whether to remove empty strings from result.
		/// @return Array of Strings.
		Array<String> split(const char delimiter, int times = -1, bool removeEmpty = false) const;
		/// @brief Splits the String with the delimiter.
		/// @param[in] delimiter The C-type string acting as splitting delimiter.
		/// @param[in] times How many times the string should be split maximally.
		/// @param[in] removeEmpty Whether to remove empty strings from result.
		/// @return Array of Strings.
		Array<String> split(const char* delimiter, int times = -1, bool removeEmpty = false) const;
		/// @brief Splits the String with the delimiter.
		/// @param[in] delimiter The String acting as splitting delimiter.
		/// @param[in] times How many times the string should be split maximally.
		/// @param[in] removeEmpty Whether to remove empty strings from result.
		/// @return Array of Strings.
		Array<String> split(const String& delimiter, int times = -1, bool removeEmpty = false) const;
		/// @brief Reverse splits the String with the delimiter once.
		/// @param[in] delimiter The character acting as splitting delimiter.
		/// @param[out] outLeft First portion of the split String.
		/// @param[out] outRight Second portion of the split String.
		/// @return True if String was split.
		bool rsplit(const char delimiter, String& outLeft, String& outRight) const;
		/// @brief Reverse splits the String with the delimiter once.
		/// @param[in] delimiter The C-type string acting as splitting delimiter.
		/// @param[out] outLeft First portion of the split String.
		/// @param[out] outRight Second portion of the split String.
		/// @return True if String was split.
		bool rsplit(const char* delimiter, String& outLeft, String& outRight) const;
		/// @brief Reverse splits the String with the delimiter once.
		/// @param[in] delimiter The String acting as splitting delimiter.
		/// @param[out] outLeft First portion of the split String.
		/// @param[out] outRight Second portion of the split String.
		/// @return True if String was split.
		bool rsplit(const String& delimiter, String& outLeft, String& outRight) const;
		/// @brief Reverse splits the String with the delimiter.
		/// @param[in] delimiter The character acting as splitting delimiter.
		/// @param[in] times How many times the string should be split maximally.
		/// @param[in] removeEmpty Whether to remove empty strings from result.
		/// @return Array of Strings.
		Array<String> rsplit(const char delimiter, int times = -1, bool removeEmpty = false) const;
		/// @brief Reverse splits the String with the delimiter.
		/// @param[in] delimiter The C-type string acting as splitting delimiter.
		/// @param[in] times How many times the string should be split maximally.
		/// @param[in] removeEmpty Whether to remove empty strings from result.
		/// @return Array of Strings.
		Array<String> rsplit(const char* delimiter, int times = -1, bool removeEmpty = false) const;
		/// @brief Reverse splits the String with the delimiter.
		/// @param[in] delimiter The String acting as splitting delimiter.
		/// @param[in] times How many times the string should be split maximally.
		/// @param[in] removeEmpty Whether to remove empty strings from result.
		/// @return Array of Strings.
		Array<String> rsplit(const String& delimiter, int times = -1, bool removeEmpty = false) const;
		/// @brief Finds the first index of a character.
		/// @param[in] c Character to search for.
		/// @param[in] start Starting index.
		/// @return The index of the first occurrence of the character.
		int indexOf(const char c, int start = 0) const;
		/// @brief Finds the first index of a character.
		/// @param[in] string C-string to search for.
		/// @param[in] start Starting index.
		/// @return The index of the first occurrence of the character.
		int indexOf(const char* string, int start = 0) const;
		/// @brief Finds the first index of a character.
		/// @param[in] string String to search for.
		/// @param[in] start Starting index.
		/// @return The index of the first occurrence of the character.
		int indexOf(const String& string, int start = 0) const;
		/// @brief Finds the first index of a character.
		/// @param[in] c Character to search for.
		/// @param[in] start Starting index.
		/// @return The index of the first occurrence of the character.
		int utf8IndexOf(const char c, int start = 0) const;
		/// @brief Finds the first index of a character.
		/// @param[in] string C-string to search for.
		/// @param[in] start Starting index.
		/// @return The index of the first occurrence of the character.
		int utf8IndexOf(const char* string, int start = 0) const;
		/// @brief Finds the first index of a character.
		/// @param[in] string String to search for.
		/// @param[in] start Starting index.
		/// @return The index of the first occurrence of the character.
		int utf8IndexOf(const String& string, int start = 0) const;
		/// @brief Finds the first index of a character searching from the back.
		/// @param[in] c Character to search for.
		/// @param[in] start Starting index.
		/// @return The index of the first occurrence of the character searching from the back.
		int rindexOf(const char c, int start = -1) const;
		/// @brief Finds the first index of a character searching from the back.
		/// @param[in] string C-string to search for.
		/// @param[in] start Starting index from the ending of the string.
		/// @return The index of the first occurrence of the character searching from the back.
		/// @note Searching is done from the "start" index to the beginning of the string. If start is negative, the entire string is searched.
		int rindexOf(const char* string, int start = -1) const;
		/// @brief Finds the first index of a character searching from the back.
		/// @param[in] string String to search for.
		/// @param[in] start Starting index from the ending of the string.
		/// @return The index of the first occurrence of the character searching from the back.
		/// @note Searching is done from the "start" index to the beginning of the string. If start is negative, the entire string is searched.
		int rindexOf(const String& string, int start = -1) const;
		/// @brief Finds the first index of a character searching from the back.
		/// @param[in] c Character to search for.
		/// @param[in] start Starting index.
		/// @return The index of the first occurrence of the character searching from the back.
		int utf8RindexOf(const char c, int start = -1) const;
		/// @brief Finds the first index of a character searching from the back.
		/// @param[in] string C-string to search for.
		/// @param[in] start Starting index from the ending of the string.
		/// @return The index of the first occurrence of the character searching from the back.
		/// @note Searching is done from the "start" index to the beginning of the string. If start is negative, the entire string is searched.
		int utf8RindexOf(const char* string, int start = -1) const;
		/// @brief Finds the first index of a character searching from the back.
		/// @param[in] string String to search for.
		/// @param[in] start Starting index from the ending of the string.
		/// @return The index of the first occurrence of the character searching from the back.
		/// @note Searching is done from the "start" index to the beginning of the string. If start is negative, the entire string is searched.
		int utf8RindexOf(const String& string, int start = -1) const;
		/// @brief Finds the first index of any character.
		/// @param[in] string Characters as C-string to search for.
		/// @param[in] start Starting index.
		/// @return The index of the first occurrence of any of the characters.-
		int indexOfAny(const char* string, int start = 0) const;
		/// @brief Finds the first index of any character.
		/// @param[in] string Characters as String to search for.
		/// @param[in] start Starting index.
		/// @return The index of the first occurrence of any of the characters.
		int indexOfAny(const String& string, int start = 0) const;
		/// @brief Finds the first index of any character.
		/// @param[in] string Characters as C-string to search for.
		/// @param[in] start Starting index.
		/// @return The index of the first occurrence of any of the characters.-
		int utf8IndexOfAny(const char* string, int start = 0) const;
		/// @brief Finds the first index of any character.
		/// @param[in] string Characters as String to search for.
		/// @param[in] start Starting index.
		/// @return The index of the first occurrence of any of the characters.
		int utf8IndexOfAny(const String& string, int start = 0) const;
		/// @brief Finds the first index of any character searching from the back.
		/// @param[in] string Characters as C-string to search for.
		/// @param[in] start Starting index from the ending of the string.
		/// @return The index of the first occurrence of any of the characters searching from the back.
		/// @note Searching is done from the "start" index to the beginning of the string. If start is negative, the entire string is searched.
		int rindexOfAny(const char* string, int start = -1) const;
		/// @brief Finds the first index of any character searching from the back.
		/// @param[in] string Characters as String to search for.
		/// @param[in] start Starting index from the ending of the string.
		/// @return The index of the first occurrence of any of the characters searching from the back.
		/// @note Searching is done from the "start" index to the beginning of the string. If start is negative, the entire string is searched.
		int rindexOfAny(const String& string, int start = -1) const;
		/// @brief Finds the first index of any character searching from the back.
		/// @param[in] string Characters as C-string to search for.
		/// @param[in] start Starting index from the ending of the string.
		/// @return The index of the first occurrence of any of the characters searching from the back.
		/// @note Searching is done from the "start" index to the beginning of the string. If start is negative, the entire string is searched.
		int utf8RindexOfAny(const char* string, int start = -1) const;
		/// @brief Finds the first index of any character searching from the back.
		/// @param[in] string Characters as String to search for.
		/// @param[in] start Starting index from the ending of the string.
		/// @return The index of the first occurrence of any of the characters searching from the back.
		/// @note Searching is done from the "start" index to the beginning of the string. If start is negative, the entire string is searched.
		int utf8RindexOfAny(const String& string, int start = -1) const;
		/// @brief Counts the occurrences of a substring.
		/// @param[in] c The character to look for.
		/// @return Number of occurrences of the substring.
		int count(const char c) const;
		/// @brief Counts the occurrences of a substring.
		/// @param[in] string The C-type string to look for.
		/// @return Number of occurrences of the substring.
		int count(const char* string) const;
		/// @brief Counts the occurrences of a substring.
		/// @param[in] string The String to look for.
		/// @return Number of occurrences of the substring.
		int count(const String& string) const;
		/// @brief Checks if the string starts with a specific string.
		/// @param[in] string The C-type string to check.
		/// @return True if String starts with s.
		bool startsWith(const char* string) const;
		/// @brief Checks if the string starts with a specific string.
		/// @param[in] string The String to check.
		/// @return True if String starts with s.
		bool startsWith(const String& string) const;
		/// @brief Checks if the string ends with a specific string.
		/// @param[in] string The C-type string to check.
		/// @return True if String ends with s.
		bool endsWith(const char* string) const;
		/// @brief Checks if the string ends with a specific string.
		/// @param[in] string The String to check.
		/// @return True if String ends with s.
		bool endsWith(const String& string) const;
		/// @brief Checks if String contains character.
		/// @param[in] c Character to search for.
		/// @return True if String contains character.
		bool contains(const char c) const;
		/// @brief Checks if String contains C-type string.
		/// @param[in] string C-type string to search for.
		/// @return True if String contains C-type string.
		bool contains(const char* string) const;
		/// @brief Checks if String contains another String.
		/// @param[in] string String to search for.
		/// @return True if this String contains other String.
		bool contains(const String& string) const;
		/// @brief Checks if String contains any character from a C-type string.
		/// @param[in] string C-type string to search for.
		/// @return True if String contains any character from a C-type string.
		bool containsAny(const char* string) const;
		/// @brief Checks if String contains any character from another String.
		/// @param[in] string String to search for.
		/// @return True if this String contains any character from another String.
		bool containsAny(const String& string) const;
		/// @brief Checks if String contains all characters from a C-type string.
		/// @param[in] string C-type string to search for.
		/// @return True if String contains all characters from a C-type string.
		bool containsAll(const char* string) const;
		/// @brief Checks if String contains all characters from another String.
		/// @param[in] string String to search for.
		/// @return True if this String contains all characters from another String.
		bool containsAll(const String& string) const;
		/// @brief Checks if all elements of the string contain only one digit '0'-'9'.
		/// @return True if string is a digit.
		bool isDigit() const;
		/// @brief Checks if the string is an integer.
		/// @return True if string is a number.
		bool isInt() const;
		/// @brief Checks if the string is a float.
		/// @param[in] requireDot If this parameter is false, then a decimal point is not required to designate a float value.
		/// @return True if string is a number.
		bool isFloat(bool requireDot = true) const;
		/// @brief Checks if the string is a number, positive or negative integer or float.
		/// @return True if string is a number.
		bool isNumber() const;
		/// @brief Checks if the string is a hexadecimal number, case ignored.
		/// @return True if string is a hex number.
		/// @note This method is case insensitive.
		/// @note Hex numbers cannot be negative in this context.
		bool isHex() const;
		/// @brief Checks if string contains ASCII only characters.
		/// @return True if String contains only ASCII-7 characters.
		bool isAscii() const;
		/// @brief Creates a substring from this String.
		/// @param[in] start Start index of the substring.
		/// @param[in] count Character length of the substring (byte-length, not UT8 character count).
		/// @return The substring.
		String subString(int start, int count) const;
		/// @brief Creates a substring from this String.
		/// @param[in] start Start index of the substring.
		/// @param[in] count Character length of the substring (byte-length, not UT8 character count).
		/// @param[in] step Every nth character only.
		/// @return The substring.
		String subString(int start, int count, int step) const;
		/// @brief Creates a substring from UTF8-indexed characters.
		/// @param[in] start Start index of the substring.
		/// @param[in] count Character length of the substring (UT8 character count, not byte-length).
		/// @return The substring.
		String utf8SubString(int start, int count) const;
		/// @brief Gets the byte length of the String.
		/// @return Byte length of String.
		int size() const;
		/// @brief Gets the character length of the String.
		/// @return Character length of String.
		int utf8Size() const;
		/// @brief Creates a string with characters converted using the %02X format.
		/// @return String of hex values of the characters.
		String toHex() const;
		/// @brief Creates an unsigned int from hex value string.
		/// @return An unsigned int.
		/// @note Will return 0 if string is not a hex number. Use String::is_hex() to check first.
		/// @note Hex strings above 0xFFFFFFFF cause undefined behavior.
		unsigned int unhex() const;

		/// @brief Returns character at position in form of String.
		/// @param[in] index Index of the character.
		/// @return A String.
		String operator()(int index) const;
		/// @brief Creates a substring.
		/// @param[in] start Start index of the substring.
		/// @param[in] count Length of the substring.
		/// @return The substring.
		String operator()(int start, int count) const;
		/// @brief Creates a substring.
		/// @param[in] start Start index of the substring.
		/// @param[in] count Length of the substring.
		/// @param[in] step Every nth character only.
		/// @return The substring.
		/// @note count defines the length of the substring, step only defines which characters are used for the new String.
		String operator()(int start, int count, int step) const;
		/// @brief Returns character at position.
		/// @param[in] index Index of the character.
		/// @return A character.
		char& operator[](int index);
		/// @brief Returns character at position.
		/// @param[in] index Index of the character.
		/// @return A character.
		const char& operator[](int index) const;
		/// @brief Casts String into bool.
		/// @note "false", "0" and "" are regarded as false, everything else is regarded as true.
		operator bool() const;
		/// @brief Casts String into short.
		operator short() const;
		/// @brief Casts String into unsigned short.
		operator unsigned short() const;
		/// @brief Casts String into int.
		operator int() const;
		/// @brief Casts String into unsigned int.
		operator unsigned int() const;
		/// @brief Casts String into 64-bit int.
		operator int64_t() const;
		/// @brief Casts String into unsigned 64-bit int.
		operator uint64_t() const;
		/// @brief Casts String into float.
		operator float() const;
		/// @brief Casts String into double.
		operator double() const;
		/// @brief Converts C-type string into String.
		/// @param[in] string C-type string value.
		/// @return This modified String.
		String operator=(char* string);
		/// @brief Converts C-type string into String.
		/// @param[in] string C-type string value.
		/// @return This modified String.
		String operator=(const char* string);
		/// @brief Assigns String to this one.
		/// @param[in] string String value.
		/// @return This modified String.
		String operator=(const String& string);
		/// @brief Converts bool into String.
		/// @param[in] b Bool value.
		/// @return This modified String.
		String operator=(const bool b);
		/// @brief Converts short into String.
		/// @param[in] s Short value.
		/// @return This modified String.
		String operator=(const short s);
		/// @brief Converts unsigned short into String.
		/// @param[in] s Unsigned short value.
		/// @return This modified String.
		String operator=(const unsigned short s);
		/// @brief Converts int into String.
		/// @param[in] i Int value.
		/// @return This modified String.
		String operator=(const int i);
		/// @brief Converts unsigned int into String.
		/// @param[in] i Unsigned int value.
		/// @return This modified String.
		String operator=(const unsigned int i);
		/// @brief Converts 64-bit int into String.
		/// @param[in] i 64-bit int value.
		/// @return This modified String.
		String operator=(const int64_t i);
		/// @brief Converts unsigned 64-bit int into String.
		/// @param[in] i Unsigned 64-bit int value.
		/// @return This modified String.
		String operator=(const uint64_t i);
		/// @brief Converts float into String.
		/// @param[in] f Float value.
		/// @return This modified String.
		String operator=(const float f);
		/// @brief Converts double into String.
		/// @param[in] d Double value.
		/// @return This modified String.
		String operator=(const double d);
		/// @brief Converts char into a String and concatenates the new String at the end of this one.
		/// @param[in] c Character value.
		void operator+=(const char c);
		/// @brief Converts C-type string into a String and concatenates the new String at the end of this one.
		/// @param[in] string C-type string value.
		void operator+=(char* string);
		/// @brief Converts C-type string into a String and concatenates the new String at the end of this one.
		/// @param[in] string C-type string value.
		void operator+=(const char* string);
		/// @brief Concatenates a String at the end of this one.
		/// @param[in] string String value.
		void operator+=(const String& string);
		/// @brief Converts bool into a String and concatenates the new String at the end of this one.
		/// @param[in] b Bool value.
		void operator+=(const bool b);
		/// @brief Converts short into a String and concatenates the new String at the end of this one.
		/// @param[in] s Short value.
		void operator+=(const short s);
		/// @brief Converts unsigned short into a String and concatenates the new String at the end of this one.
		/// @param[in] s Unsigned short value.
		void operator+=(const unsigned short s);
		/// @brief Converts int into a String and concatenates the new String at the end of this one.
		/// @param[in] i Int value.
		void operator+=(const int i);
		/// @brief Converts unsigned int into a String and concatenates the new String at the end of this one.
		/// @param[in] i Unsigned int value.
		void operator+=(const unsigned int i);
		/// @brief Converts 64-bit int into a String and concatenates the new String at the end of this one.
		/// @param[in] i 64-bit int value.
		void operator+=(const int64_t i);
		/// @brief Converts unsigned 64-bit int into a String and concatenates the new String at the end of this one.
		/// @param[in] i Unsigned 64-bit int value.
		void operator+=(const uint64_t i);
		/// @brief Converts float into a String and concatenates the new String at the end of this one.
		/// @param[in] f Float value.
		void operator+=(const float f);
		/// @brief Converts double into a String and concatenates the new String at the end of this one.
		/// @param[in] d Double value.
		void operator+=(const double d);
		/// @brief Merges String with a character converted into String first.
		/// @param[in] c Character value.
		/// @return New String.
		String operator+(const char c) const;
		/// @brief Merges String with a C-type string converted into String first.
		/// @param[in] string C-type string value.
		/// @return New String.
		String operator+(char* string) const;
		/// @brief Merges String with a C-type string converted into String first.
		/// @param[in] string C-type string value.
		/// @return New String.
		String operator+(const char* string) const;
		/// @brief Merges String with another String.
		/// @param[in] string String value.
		/// @return New String.
		String operator+(const String& string) const;
		/// @brief Compares String for equivalency.
		/// @param[in] s C-type string value.
		/// @return True if other String is equal to this one.
		bool operator==(const char* s) const;
		/// @brief Compares String for equivalency.
		/// @param[in] string String value.
		/// @return True if other String is equal to this one.
		bool operator==(const String& string) const;
		/// @brief Compares String for equivalency.
		/// @param[in] b bool value.
		/// @return True if value converted into String is equal to this one.
		bool operator==(const bool b) const;
		/// @brief Compares String for equivalency.
		/// @param[in] s Short value.
		/// @return True if value converted into String is equal to this one.
		bool operator==(const short s) const;
		/// @brief Compares String for equivalency.
		/// @param[in] s Unsigned short value.
		/// @return True if value converted into String is equal to this one.
		bool operator==(const unsigned short s) const;
		/// @brief Compares String for equivalency.
		/// @param[in] i Int value.
		/// @return True if value converted into String is equal to this one.
		bool operator==(const int i) const;
		/// @brief Compares String for equivalency.
		/// @param[in] i Unsigned int value.
		/// @return True if value converted into String is equal to this one.
		bool operator==(const unsigned int i) const;
		/// @brief Compares String for equivalency.
		/// @param[in] i 64-bit int value.
		/// @return True if value converted into String is equal to this one.
		bool operator==(const int64_t i) const;
		/// @brief Compares String for equivalency.
		/// @param[in] i Unsigned 64-bit int value.
		/// @return True if value converted into String is equal to this one.
		bool operator==(const uint64_t i) const;
		/// @brief Compares String for equivalency.
		/// @param[in] f Float value.
		/// @return True if value converted into String is equal to this one.
		bool operator==(const float f) const;
		/// @brief Compares String for equivalency.
		/// @param[in] d Double value.
		/// @return True if value converted into String is equal to this one.
		bool operator==(const double d) const;
		/// @brief Compares String for non-equivalency.
		/// @param[in] s C-type string value.
		/// @return True if other String is not equal to this one.
		bool operator!=(const char* s) const;
		/// @brief Compares String for non-equivalency.
		/// @param[in] string String value.
		/// @return True if other String is not equal to this one.
		bool operator!=(const String& string) const;
		/// @brief Compares String for non-equivalency.
		/// @param[in] b bool value.
		/// @return True if value converted into String is not equal to this one.
		bool operator!=(const bool b) const;
		/// @brief Compares String for non-equivalency.
		/// @param[in] s Short value.
		/// @return True if value converted into String is not equal to this one.
		bool operator!=(const short s) const;
		/// @brief Compares String for non-equivalency.
		/// @param[in] s Unsigned short value.
		/// @return True if value converted into String is not equal to this one.
		bool operator!=(const unsigned short s) const;
		/// @brief Compares String for non-equivalency.
		/// @param[in] i Int value.
		/// @return True if value converted into String is not equal to this one.
		bool operator!=(const int i) const;
		/// @brief Compares String for non-equivalency.
		/// @param[in] i Unsigned int value.
		/// @return True if value converted into String is not equal to this one.
		bool operator!=(const unsigned int i) const;
		/// @brief Compares String for non-equivalency.
		/// @param[in] i 64-bit int value.
		/// @return True if value converted into String is not equal to this one.
		bool operator!=(const int64_t i) const;
		/// @brief Compares String for non-equivalency.
		/// @param[in] i Unsigned 64-bit int value.
		/// @return True if value converted into String is not equal to this one.
		bool operator!=(const uint64_t i) const;
		/// @brief Compares String for non-equivalency.
		/// @param[in] f Float value.
		/// @return True if value converted into String is not equal to this one.
		bool operator!=(const float f) const;
		/// @brief Compares String for non-equivalency.
		/// @param[in] d Double value.
		/// @return True if value converted into String is not equal to this one.
		bool operator!=(const double d) const;
		/// @brief Compares String for being less than another String.
		/// @param[in] string String value.
		/// @return True if this String is less than other String.
		bool operator<(const String& string) const;
		/// @brief Compares String for being greater than another String.
		/// @param[in] string String value.
		/// @return True if this String is greater than other String.
		bool operator>(const String& string) const;
		/// @brief Compares String for being less than or equal to another String.
		/// @param[in] string String value.
		/// @return True if this String is less than or equal to other String.
		bool operator<=(const String& string) const;
		/// @brief Compares String for being greater than or equal to another String.
		/// @param[in] string String value.
		/// @return True if this String is greater than or equal to other String.
		bool operator>=(const String& string) const;

		/// @brief Gets the C-string.
		/// @return This String's C-string.
		const char* cStr() const;
		/// @brief Creates an unicode (unsigned int) string.
		/// @return A unicode (unsigned int) string.
		std::ustring uStr() const;
		/// @brief Creates a wchar string.
		/// @return A wchar string.
		std::wstring wStr() const;
		/// @brief Converts first UTF8 character into the corresponding character code.
		/// @param[in] index The byte position of the first character in the string.
		/// @param[out] byteCount Length of UTF8 character in bytes.
		/// @return Character code.
		/// @note The out value of byteCount parameter can be used to move a character iterator forward.
		unsigned int firstUnicodeChar(int index = 0, int* byteCount = NULL) const;

		/// @brief Converts a unicode unsigned int to a UTF-8 string.
		/// @param[in] value The unsigned int value.
		/// @return UTF-8 string.
		static String fromUnicode(unsigned int value);
		/// @brief Converts a unicode wchar to a UTF-8 string.
		/// @param[in] value The wchar value.
		/// @return UTF-8 string.
		static String fromUnicode(wchar_t value);
		/// @brief Converts a char to a UTF-8 string.
		/// @param[in] value The char.
		/// @return UTF-8 string.
		static String fromUnicode(const char value);
		/// @brief Converts an unsigned char to a UTF-8 string.
		/// @param[in] value The unsigned char.
		/// @return UTF-8 string.
		static String fromUnicode(unsigned char value);
		/// @brief Converts a unicode unsigned int string to a UTF-8 string.
		/// @param[in] string The unsigned int string.
		/// @return UTF-8 string.
		static String fromUnicode(const unsigned int* string);
		/// @brief Converts a unicode wchar string to a UTF-8 string.
		/// @param[in] string The wchar string.
		/// @return UTF-8 string.
		static String fromUnicode(const wchar_t* string);
		/// @brief Converts a char string to a UTF-8 string.
		/// @param[in] string The char string.
		/// @return UTF-8 string.
		static String fromUnicode(const char* string);
		/// @brief Converts an unsigned char string to a UTF-8 string.
		/// @param[in] string The unsigned char string.
		/// @return UTF-8 string.
		static String fromUnicode(const unsigned char* string);
		/// @brief Converts a unicode unsigned int Array to a UTF-8 string.
		/// @param[in] chars The unsigned int characters.
		/// @return UTF-8 string.
		static String fromUnicode(Array<unsigned int> chars);
		/// @brief Converts a unicode wchar Array to a UTF-8 string.
		/// @param[in] chars The wchar characters.
		/// @return UTF-8 string.
		static String fromUnicode(Array<wchar_t> chars);
		/// @brief Converts a char Array to a UTF-8 string.
		/// @param[in] chars The char characters.
		/// @return UTF-8 string.
		static String fromUnicode(Array<char> chars);
		/// @brief Converts an unsigned char Array to a UTF-8 string.
		/// @param[in] chars The unsigned char characters.
		/// @return UTF-8 string.
		static String fromUnicode(Array<unsigned char> chars);

	};
}

/// @brief Alias for simpler code.
typedef hltypes::String hstr;
/// @brief Alias for simpler code.
typedef hltypes::String const& chstr;

/// @brief Merges a C-type string and a String into a new String.
/// @param[in] string1 C-type string to merge.
/// @param[in] string2 String to merge.
/// @return Merged String.
hltypesFnExport inline hltypes::String operator+(const char* string1, const hltypes::String& string2)
{
	hltypes::String result(string1);
	result.add(string2.cStr());
	return result;
}
/// @brief Merges a C-type string and a String into a new String.
/// @param[in] string1 C-type string to merge.
/// @param[in] string2 String to merge.
/// @return Merged String.
hltypesFnExport inline hltypes::String operator+(char* string1, const hltypes::String& string2)
{
	hltypes::String result(string1);
	result.add(string2.cStr());
	return result;
}
/// @brief Applies formatting to a string.
/// @param[in] format C-type string containing format.
/// @param[in] args Variable argument list.
/// @return Formatted String.
hltypesFnExport hltypes::String hvsprintf(const char* format, va_list args);
/// @brief Applies formatting to a string.
/// @param[in] format C-type string containing format.
/// @param[in] ... Formatting arguments.
/// @return Formatted String.
hltypesFnExport hltypes::String hsprintf(const char* format, ...);

#endif

