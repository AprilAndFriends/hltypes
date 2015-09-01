/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Provides high level enum functionality.

#ifndef HLTYPES_ENUM_H
#define HLTYPES_ENUM_H

#include "hltypesExport.h"

#define __HL_EXPAND_MACRO(x) x
#define HL_ENUM_DEFINE_CLASS(classe, code) \
class classe : public henum \
{ \
public: \
	classe() : henum() { } \
	classe(unsigned int value) : henum(value) { } \
	~classe() { } \
	__HL_EXPAND_MACRO code \
};

namespace hltypes
{
	/// @brief Encapsulates enum functionality and adds high level methods.
	class hltypesExport Enum
	{
	public:
		/// @brief The enum value.
		unsigned int value;

		/// @brief Empty constructor.
		Enum();
		/// @brief Basic constructor.
		/// @param[in] value The enum value.
		Enum(unsigned int value);
		/// @brief Destructor.
		virtual ~Enum();

		/// @brief Checks if this Enum is greater than another Enum.
		/// @param[in] other Other Enum.
		/// @return True if this Enum is greater than another Enum.
		bool operator>(const Enum& other) const;
		/// @brief Checks if this Enum is less than another Enum.
		/// @param[in] other Other Enum.
		/// @return True if this Enum is less than another Enum.
		bool operator<(const Enum& other) const;
		/// @brief Checks if this Enum is greater than or equal to another Enum.
		/// @param[in] other Other Enum.
		/// @return True if this Enum is greater than or equal to another Enum.
		bool operator>=(const Enum& other) const;
		/// @brief Checks if this Enum is less than or equal to another Enum.
		/// @param[in] other Other Enum.
		/// @return True if this Enum is less than or equal to another Enum.
		bool operator<=(const Enum& other) const;
		/// @brief Checks if this Enum equals another Enum.
		/// @param[in] other Other Enum.
		/// @return True if this Enum equals another Enum.
		bool operator==(const Enum& other) const;
		/// @brief Checks if this Enum non-equals another Enum.
		/// @param[in] other Other Enum.
		/// @return True if this Enum non-equals another Enum.
		bool operator!=(const Enum& other) const;

	};

}

/// @brief Alias for simpler code.
typedef hltypes::Enum henum;

#endif
