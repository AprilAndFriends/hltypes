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
/// Encapsulates all containers and adds high level methods.

#ifndef HLTYPES_CONTAINER_H
#define HLTYPES_CONTAINER_H

#include "hltypesUtil.h"
#include "hstring.h"

namespace hltypes
{
	/// @brief Encapsulates container functionality and adds high level methods.
	template <class STD, class T>
	class Container : public STD
	{
	private:
		typedef typename STD::iterator iterator_t;
		typedef typename STD::const_iterator const_iterator_t;
	public:
		/// @brief Empty constructor.
		inline Container() : STD()
		{
		}
		/// @brief Copy constructor.
		/// @param[in] other Container to copy.
		inline Container(const Container& other) : STD(other)
		{
		}
		/// @brief Constructor from single element.
		/// @param[in] element Element to insert.
		inline Container(const T& element) : STD()
		{
			//this->insert_at(0, element);
		}
		/// @brief Constructor from single element.
		/// @param[in] element Element to insert.
		/// @param[in] times Number of times to insert element.
		inline Container(const T& element, int times) : STD()
		{
			//this->insert_at(0, element, times);
		}
		/// @brief Constructor from another Container.
		/// @param[in] other Container to copy.
		/// @param[in] count Number of elements to copy.
		inline Container(const Container& other, const int count) : STD()
		{
			//this->insert_at(0, other, count);
		}
		/// @brief Constructor from another Container.
		/// @param[in] other Container to copy.
		/// @param[in] start Start index of elements to copy.
		/// @param[in] count Number of elements to copy.
		inline Container(const Container& other, const int start, const int count) : STD()
		{
			//this->insert_at(0, other, start, count);
		}
		/// @brief Constructor from C-type array.
		/// @param[in] other C-type array to copy.
		/// @param[in] count Number of elements to copy.
		inline Container(const T other[], const int count) : STD()
		{
			//this->insert_at(0, other, count);
		}
		/// @brief Constructor from C-type array.
		/// @param[in] other C-type array to copy.
		/// @param[in] start Start index of elements to copy.
		/// @param[in] count Number of elements to copy.
		inline Container(const T other[], const int start, const int count) : STD()
		{
			//this->insert_at(0, other, start, count);
		}
		/// @brief Destructor.
		inline ~Container()
		{
		}
		/// @brief Returns the number of elements in the Container.
		/// @return The number of elements in the Container.
		inline int size() const
		{
			return (int)STD::size();
		}
		/// @brief Compares the contents of two Containers for being equal.
		/// @param[in] other Another Container.
		/// @return True if number of elements are equal and all pairs of elements at the same positions are equal.
		inline bool equals(const Container& other) const
		{
			if (this->size() != other.size())
			{
				return false;
			}
			for_iter (i, 0, this->size())
			{
				// making sure operator== is used, not !=
				if (!(STD::at(i) == other.at(i)))
				{
					return false;
				}
			}
			return true;
		}
		/// @brief Compares the contents of two Containers for being not equal.
		/// @param[in] other Another Container.
		/// @return True if number of elements are not equal or at least one pair of elements at the same positions is not equal.
		inline bool nequals(const Container& other) const
		{
			if (this->size() != other.size())
			{
				return true;
			}
			for_iter (i, 0, this->size())
			{
				// making sure operator!= is used, not ==
				if (STD::at(i) != other.at(i))
				{
					return true;
				}
			}
			return false;
		}

	};
	
}

#endif
