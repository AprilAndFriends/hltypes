/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include "henum.h"
#include "hlog.h"

namespace hltypes
{
	Enum::Enum()
	{
		this->value = 0U;
	}

	Enum::Enum(unsigned int value)
	{
		this->value = value;
	}

	Enum::~Enum()
	{
	}

	String Enum::getName() const
	{
		Map<unsigned int, String>& instances = this->_getInstances();
		if (!instances.hasKey(value))
		{
			throw EnumValueNotExistsException(value);
		}
		return instances[this->value];
	}

	void Enum::_addNewInstance(const String& className, const String& name)
	{
		String newName = name;
		Map<unsigned int, String>& instances = this->_getInstances();
		if (instances.size() > 0)
		{
			this->value = instances.keys().max() + 1;
		}
		if (newName == "")
		{
			newName = this->value;
		}
		this->_addNewInstance(className, newName, this->value);
	}

	void Enum::_addNewInstance(const String& className, const String& name, unsigned int value)
	{
		Map<unsigned int, String>& instances = this->_getInstances();
		if (instances.hasKey(value))
		{
			throw EnumValueAlreadyExistsException(value);
		}
		instances[this->value] = name;
		// because of the order of global var initializations, mutexes get messed up
		printf("[%s] Adding enum '%s::%s' under value '%u'.\n", logTag.cStr(), className.cStr(), name.cStr(), this->value);
	}

	bool Enum::operator<(const Enum& other) const
	{
		return (this->value < other.value);
	}

	bool Enum::operator>(const Enum& other) const
	{
		return (this->value > other.value);
	}

	bool Enum::operator<=(const Enum& other) const
	{
		return (this->value <= other.value);
	}

	bool Enum::operator>=(const Enum& other) const
	{
		return (this->value >= other.value);
	}

	bool Enum::operator==(const Enum& other) const
	{
		return (this->value == other.value);
	}

	bool Enum::operator!=(const Enum& other) const
	{
		return (this->value != other.value);
	}

}
