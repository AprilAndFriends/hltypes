/// @file
/// @author  Boris Mikic
/// @version 1.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Encapsulates std::deque and adds high level methods.

#ifndef HLTYPES_DEQUE_H
#define HLTYPES_DEQUE_H

#include <deque>

/// @brief Provides a simpler syntax to iterate through a ldeque.
#define foreach_q(type, name, container) for (std::deque<type>::iterator name = (container).begin(); name != (container).end(); name++)
/// @brief Provides a simpler syntax to reverse iterate through a Deque.
#define foreach_qr(type, name, container) for (std::deque<type>::reverse_iterator name = (container).rbegin(); name != (container).rend(); name++)
/// @brief Alias for simpler code.
#define stddeque std::deque<T>

namespace hltypes
{
	/// @brief Encapsulates std::deque and adds high level methods.
	/// @author Boris Mikic
	template <class T> class Deque : public stddeque
	{
	public:
		/// @brief Empty constructor.
		Deque() : stddeque()
		{
		}
		/// @brief Copy constructor.
		/// @param[in] other Deque to copy.
		Deque(const Deque<T>& other) : stddeque(other)
		{
		}
		/// @brief Destructor.
		~Deque()
		{
		}
		/// @brief Returns the number of elements in the Deque.
		/// @return The number of elements in the Deque.
		int size() const
		{
			return (int)stddeque::size();
		}

	};
	
}

/// @brief Alias for simpler code.
#define hdeque hltypes::Deque

#endif
