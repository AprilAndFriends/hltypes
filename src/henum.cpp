/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include "henum.h"

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
