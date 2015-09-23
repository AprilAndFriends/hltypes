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
/// Provides high level enumeration functionality.

#ifndef HLTYPES_ENUMERATION_H
#define HLTYPES_ENUMERATION_H

#include "harray.h"
#include "hmap.h"
#include "hltypesExport.h"

#define __HL_EXPAND_MACRO(x) x
#define HL_ENUM_CLASS_PREFIX_DECLARE(prefix, classe, code) \
class prefix classe : public henum \
{ \
public: \
	classe() : henum() { } \
	classe(chstr name) : henum() { this->_addNewInstance(#classe, name); } \
	classe(chstr name, unsigned int value) : henum(value) { this->_addNewInstance(#classe, name, value); } \
	~classe() { } \
	classe next() \
	{ \
		harray<classe> values = classe::getValues(); \
		values.sort(); \
		return values[(values.indexOf(*this) + 1) % values.size()]; \
	} \
	classe previous() \
	{ \
		harray<classe> values = classe::getValues(); \
		values.sort(); \
		return values[(values.indexOf(*this) + values.size() - 1) % values.size()]; \
	} \
	static classe fromInt(int value) \
	{ \
		return fromUint((unsigned int)value); \
	} \
	static classe fromUint(unsigned int value) \
	{ \
		if (!_instances.hasKey(value)) \
		{ \
			throw EnumerationValueNotExistsException(value); \
		} \
		return classe(value); \
	} \
	static bool hasValueFor(int value) \
	{ \
		return _instances.hasKey((unsigned int)value); \
	} \
	static bool hasValueFor(unsigned int value) \
	{ \
		return _instances.hasKey(value); \
	} \
	static harray<classe> getValues() \
	{ \
		harray<classe> result; \
		foreach_map (unsigned int, hstr, it, _instances) \
		{ \
			result += classe::fromUint(it->first); \
		} \
		return result; \
	} \
	__HL_EXPAND_MACRO code \
protected: \
	classe(unsigned int value) : henum(value) { } \
	hmap<unsigned int, hstr>& _getInstances() const { return _instances; } \
private: \
	static hmap<unsigned int, hstr> _instances; \
};
#define HL_ENUM_CLASS_DECLARE(classe, code) HL_ENUM_CLASS_PREFIX_DECLARE(, classe, code)
#define HL_ENUM_CLASS_DEFINE(classe, code) \
	hmap<unsigned int, hstr> classe::_instances; \
	__HL_EXPAND_MACRO code;

#define HL_ENUM_DECLARE(classe, name) static const classe name;
#define HL_ENUM_DEFINE(classe, name) const classe classe::name(#name);
#define HL_ENUM_DEFINE_VALUE(classe, name, value) const classe classe::name(#name, value);
#define HL_ENUM_DEFINE_NAME(classe, name, stringName) const classe classe::name(stringName);
#define HL_ENUM_DEFINE_NAME_VALUE(classe, name, stringName, value) const classe classe::name(stringName, value);

namespace hltypes
{
	/// @brief Encapsulates enumeration functionality and adds high level methods.
	class hltypesExport Enumeration
	{
	public:
		/// @brief The enumeration value.
		unsigned int value;

		/// @brief Empty constructor.
		/// @note This will NOT auto-generate a value in the internal index.
		Enumeration();
		/// @brief Destructor.
		virtual ~Enumeration();

		/// @brief Gets the String name.
		String getName() const;

		/// @brief Checks if this Enumeration is greater than another Enumeration.
		/// @param[in] other Other Enumeration.
		/// @return True if this Enumeration is greater than another Enumeration.
		bool operator>(const Enumeration& other) const;
		/// @brief Checks if this Enumeration is less than another Enumeration.
		/// @param[in] other Other Enumeration.
		/// @return True if this Enumeration is less than another Enumeration.
		bool operator<(const Enumeration& other) const;
		/// @brief Checks if this Enumeration is greater than or equal to another Enumeration.
		/// @param[in] other Other Enumeration.
		/// @return True if this Enumeration is greater than or equal to another Enumeration.
		bool operator>=(const Enumeration& other) const;
		/// @brief Checks if this Enumeration is less than or equal to another Enumeration.
		/// @param[in] other Other Enumeration.
		/// @return True if this Enumeration is less than or equal to another Enumeration.
		bool operator<=(const Enumeration& other) const;
		/// @brief Checks if this Enumeration equals another Enumeration.
		/// @param[in] other Other Enumeration.
		/// @return True if this Enumeration equals another Enumeration.
		bool operator==(const Enumeration& other) const;
		/// @brief Checks if this Enumeration non-equals another Enumeration.
		/// @param[in] other Other Enumeration.
		/// @return True if this Enumeration non-equals another Enumeration.
		bool operator!=(const Enumeration& other) const;

	protected:
		/// @brief Basic constructor.
		/// @param[in] value The Enumeration value.
		/// @note This will NOT auto-generate a value in the internal index. It is used to convert ints to Enumeration instances.
		Enumeration(unsigned int value);

		/// @brief Gets the Map of Enumeration instances that can exist.
		/// @return The Map of Enumeration instances that can exist.
		virtual Map<unsigned int, String>& _getInstances() const { static Map<unsigned int, String> dummy; return dummy; };
		/// @brief Adds a new possible instance to the list.
		/// @param[in] className Name of the Enumeration.
		/// @param[in] name Name of the Enumeration value.
		/// @note This will add a new auto-generated value to the list.
		void _addNewInstance(const String& className, const String& name);
		/// @brief Adds a new possible instance to the list.
		/// @param[in] className Name of the Enumeration.
		/// @param[in] name Name of the Enumeration value.
		/// @param[in] value Value to add to the list.
		void _addNewInstance(const String& className, const String& name, unsigned int value);

	};

}

/// @brief Alias for simpler code.
typedef hltypes::Enumeration henum;

#endif
