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

#include "hmap.h"
#include "hltypesExport.h"

#define HL_ENUM_CLASS_PREFIX_DECLARE(prefix, classe, code) \
class prefix classe : public henum \
{ \
public: \
	classe() : henum() { } \
	classe(chstr name) : henum(name) { this->_addNewInstance(#classe, name); } \
	classe(chstr name, unsigned int value) : henum(name, value) { this->_addNewInstance(#classe, name, value); } \
	~classe() { } \
	__HL_EXPAND_MACRO code \
protected: \
	hmap<unsigned int, hstr>& _getInstances() { return _instances; } \
private: \
	static hmap<unsigned int, hstr> _instances; \
};
#define HL_ENUM_CLASS_DECLARE(classe, code) HL_ENUM_CLASS_PREFIX_DECLARE(, classe, code)
#define HL_ENUM_CLASS_DEFINE(classe, code) \
	hmap<unsigned int, hstr> classe::_instances; \
	__HL_EXPAND_MACRO code;

#define __HL_EXPAND_MACRO(x) x
#define HL_ENUM_DECLARE(classe, name) static const classe name;
#define HL_ENUM_DEFINE(classe, name) const classe classe::name(#name);
#define HL_ENUM_DEFINE_VALUE(classe, name, value) const classe classe::name(#name, value);
#define HL_ENUM_DEFINE_NAME(classe, name, stringName) const classe classe::name(stringName);
#define HL_ENUM_DEFINE_NAME_VALUE(classe, name, stringName, value) const classe classe::name(stringName, value);

namespace hltypes
{
	/// @brief Encapsulates enum functionality and adds high level methods.
	class hltypesExport Enum
	{
	public:
		/// @brief The enum value.
		unsigned int value;

		/// @brief Empty constructor.
		/// @note This will NOT auto-generate a value in the internal index.
		Enum();
		/// @brief Basic constructor.
		/// @param[in] name The name as String.
		/// @note This will auto-generate a value in the internal index.
		Enum(const String& name);
		/// @brief Basic constructor.
		/// @param[in] name The name as String.
		/// @param[in] value The enum value.
		/// @note This will NOT auto-generate a value in the internal index.
		Enum(const String& name, unsigned int value);
		/// @brief Destructor.
		virtual ~Enum();

		/// @brief Gets the String name.
		String getName();

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

	protected:
		/// @brief Gets the Map of Enum instances that can exist.
		/// @return The Map of Enum instances that can exist.
		virtual Map<unsigned int, String>& _getInstances() { static Map<unsigned int, String> dummy; return dummy; };
		/// @brief Adds a new possible instance to the list.
		/// @param[in] className Name of the enum.
		/// @param[in] name Name of the enum value.
		/// @note This will add a new auto-generated value to the list.
		void _addNewInstance(const String& className, const String& name);
		/// @brief Adds a new possible instance to the list.
		/// @param[in] className Name of the enum.
		/// @param[in] name Name of the enum value.
		/// @param[in] value Value to add to the list.
		void _addNewInstance(const String& className, const String& name, unsigned int value);

	};

}

/// @brief Alias for simpler code.
typedef hltypes::Enum henum;

#endif
