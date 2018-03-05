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
/// Provides high level enumeration functionality.

#ifndef HLTYPES_ENUMERATION_H
#define HLTYPES_ENUMERATION_H

#include "harray.h"
#include "hltypesUtil.h"
#include "hltypesExport.h"
#include "hmap.h"

/// @brief Helper macro for declaring an enum class.
/// @param[in] exportDefinition Export definition.
/// @param[in] classe Name of the enum class.
/// @param[in] code Additional code to process (usually value declarations).
#define HL_ENUM_CLASS_PREFIX_DECLARE(exportDefinition, classe, code) \
class exportDefinition classe : public henum \
{ \
public: \
	inline classe() : henum() { } \
	inline classe(chstr name) : henum() { this->_addNewInstance(#classe, name); } \
	inline classe(chstr name, unsigned int value) : henum(value) { this->_addNewInstance(#classe, name, value); } \
	inline classe next() \
	{ \
		harray<classe> values = classe::getValues(); \
		values.sort(); \
		return values[(values.indexOf(*this) + 1) % values.size()]; \
	} \
	inline classe previous() \
	{ \
		harray<classe> values = classe::getValues(); \
		values.sort(); \
		return values[(values.indexOf(*this) + values.size() - 1) % values.size()]; \
	} \
	inline bool operator<(const classe& other) const { return (this->value < other.value); } \
	inline bool operator>(const classe& other) const { return (this->value > other.value); } \
	inline bool operator<=(const classe& other) const { return (this->value <= other.value); } \
	inline bool operator>=(const classe& other) const { return (this->value >= other.value); } \
	inline bool operator==(const classe& other) const { return (this->value == other.value); } \
	inline bool operator!=(const classe& other) const { return (this->value != other.value); } \
	inline static classe fromInt(int value) \
	{ \
		return fromUint((unsigned int)value); \
	} \
	inline static classe fromUint(unsigned int value) \
	{ \
		if (!_instances.hasKey(value)) \
		{ \
			if (!_useDefaultLooseValue) \
			{ \
				throw EnumerationValueNotExistsException(value); \
			} \
			return classe(_defaultLooseValue); \
		} \
		return classe(value); \
	} \
	inline static classe fromName(chstr name) \
	{ \
		if (!_instances.hasValue(name)) \
		{ \
			if (!_useDefaultLooseValue) \
			{ \
				throw EnumerationValueNotExistsException(name); \
			} \
			return classe(_defaultLooseValue); \
		} \
		return classe(_instances(name)); \
	} \
	inline static bool hasValue(chstr name) \
	{ \
		return _instances.hasValue(name); \
	} \
	inline static bool hasValueFor(int value) \
	{ \
		return _instances.hasKey((unsigned int)value); \
	} \
	inline static bool hasValueFor(unsigned int value) \
	{ \
		return _instances.hasKey(value); \
	} \
	inline static harray<classe> getValues() \
	{ \
		harray<classe> result; \
		foreach_map (unsigned int, hstr, it, _instances) \
		{ \
			result += classe::fromUint(it->first); \
		} \
		return result; \
	} \
	inline static hstr getEnumName() { return #classe; } \
	inline static hmap<unsigned int, hstr> getAsMap() { return _instances; } \
	__HL_EXPAND_MACRO code \
protected: \
	inline classe(unsigned int value) : henum(value) { } \
	inline hmap<unsigned int, hstr>& _getInstances() const { return _instances; } \
private: \
	static hmap<unsigned int, hstr> _instances; \
	static bool _useDefaultLooseValue; \
	static unsigned int _defaultLooseValue; \
};
/// @see HL_ENUM_CLASS_PREFIX_DECLARE
#define HL_ENUM_CLASS_DECLARE(classe, code) HL_ENUM_CLASS_PREFIX_DECLARE(, classe, code)
/// @brief Helper macro for defining an enum class.
/// @param[in] classe Name of the enum class.
/// @param[in] code Additional code to process (usually value definitions).
#define HL_ENUM_CLASS_DEFINE(classe, code) \
	hmap<unsigned int, hstr> classe::_instances; \
	bool classe::_useDefaultLooseValue = false; \
	unsigned int classe::_defaultLooseValue = 0; \
	__HL_EXPAND_MACRO code;
/// @brief Helper macro for defining an enum class.
/// @param[in] classe Name of the enum class.
/// @param[in] defaultLooseValue Which value to use if the value is missing in the fromInt() and fromUint() functions.
/// @param[in] code Additional code to process (usually value definitions).
/// @see fromInt()
/// @see fromUint()
#define HL_ENUM_CLASS_DEFINE_LOOSE(classe, defaultLooseValue, code) \
	hmap<unsigned int, hstr> classe::_instances; \
	bool classe::_useDefaultLooseValue = true; \
	unsigned int classe::_defaultLooseValue = defaultLooseValue; \
	__HL_EXPAND_MACRO code;

/// @brief Helper macro for declaring an enum value within a class.
/// @param[in] classe Name of the enum class.
/// @param[in] name Value name.
#define HL_ENUM_DECLARE(classe, name) static const classe name;
/// @brief Helper macro for defining an enum value within a class.
/// @param[in] classe Name of the enum class.
/// @param[in] name Value name.
#define HL_ENUM_DEFINE(classe, name) const classe classe::name(#name);
/// @brief Helper macro for defining an enum value within a class with a certain value.
/// @param[in] classe Name of the enum class.
/// @param[in] name Value name.
/// @param[in] value Int value.
#define HL_ENUM_DEFINE_VALUE(classe, name, value) const classe classe::name(#name, value);
/// @brief Helper macro for defining an enum value within a class with a custom internal string name.
/// @param[in] classe Name of the enum class.
/// @param[in] name Value name.
/// @param[in] stringName String name used for display.
#define HL_ENUM_DEFINE_NAME(classe, name, stringName) const classe classe::name(stringName);
/// @brief Helper macro for defining an enum value within a class with a custom internal string name and a certain value.
/// @param[in] classe Name of the enum class.
/// @param[in] name Value name.
/// @param[in] stringName String name used for display.
/// @param[in] value Int value.
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
