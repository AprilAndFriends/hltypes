/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include "henum.h"
#include "hlog.h"

namespace hltypes
{
	Enumeration::Enumeration()
	{
		this->value = 0U;
	}

	Enumeration::Enumeration(unsigned int value)
	{
		this->value = value;
	}

	Enumeration::~Enumeration()
	{
	}

	String Enumeration::getName() const
	{
		Map<unsigned int, String>& instances = this->_getInstances();
		if (!instances.hasKey(value))
		{
			throw EnumerationValueNotExistsException(value);
		}
		return instances[this->value];
	}

	void Enumeration::_addNewInstance(const String& className, const String& name)
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

	void Enumeration::_addNewInstance(const String& className, const String& name, unsigned int value)
	{
		Map<unsigned int, String>& instances = this->_getInstances();
		if (instances.hasKey(value))
		{
			throw EnumerationValueAlreadyExistsException(value);
		}
		instances[this->value] = name;
#ifdef _DEBUG
		// because of the order of global var initializations, mutexes get messed up
		printf("[hltypes] Adding enum '%s::%s' under value '%u'.\n", className.cStr(), name.cStr(), this->value);
#endif
	}

	bool Enumeration::operator<(const Enumeration& other) const
	{
		return (this->value < other.value);
	}

	bool Enumeration::operator>(const Enumeration& other) const
	{
		return (this->value > other.value);
	}

	bool Enumeration::operator<=(const Enumeration& other) const
	{
		return (this->value <= other.value);
	}

	bool Enumeration::operator>=(const Enumeration& other) const
	{
		return (this->value >= other.value);
	}

	bool Enumeration::operator==(const Enumeration& other) const
	{
		return (this->value == other.value);
	}

	bool Enumeration::operator!=(const Enumeration& other) const
	{
		return (this->value != other.value);
	}

}
