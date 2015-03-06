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

#include "hstring.h"

namespace hltypes
{
	/// @brief Encapsulates container functionality and adds high level methods.
	template <class STD, class T>
	class Container
	{
	public:
		/// @brief Empty constructor.
		inline Container()
		{
		}
		/// @brief Copy constructor.
		/// @param[in] other Array to copy.
		inline Container(const Container& other)
		{
		}
		/// @brief Constructor from single element.
		/// @param[in] element Element to insert.
		inline Container(const T& element)
		{
			//this->insert_at(0, element);
		}
		/// @brief Constructor from single element.
		/// @param[in] element Element to insert.
		/// @param[in] times Number of times to insert element.
		inline Container(const T& element, int times)
		{
			//this->insert_at(0, element, times);
		}
		/// @brief Constructor from another Array.
		/// @param[in] other Array to copy.
		/// @param[in] count Number of elements to copy.
		inline Container(const Container& other, const int count)
		{
			//this->insert_at(0, other, count);
		}
		/// @brief Constructor from another Array.
		/// @param[in] other Array to copy.
		/// @param[in] start Start index of elements to copy.
		/// @param[in] count Number of elements to copy.
		inline Container(const Container& other, const int start, const int count)
		{
			//this->insert_at(0, other, start, count);
		}
		/// @brief Constructor from C-type array.
		/// @param[in] other C-type array to copy.
		/// @param[in] count Number of elements to copy.
		inline Container(const T other[], const int count)
		{
			//this->insert_at(0, other, count);
		}
		/// @brief Constructor from C-type array.
		/// @param[in] other C-type array to copy.
		/// @param[in] start Start index of elements to copy.
		/// @param[in] count Number of elements to copy.
		inline Container(const T other[], const int start, const int count)
		{
			//this->insert_at(0, other, start, count);
		}
		/// @brief Destructor.
		virtual inline ~Container()
		{
		}
		
	};
	
}

#endif
