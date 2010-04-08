/************************************************************************************\
* This source file is part of the High Level C++ types library                       *
* For latest info, see http://libhltypes.sourceforge.net/                            *
**************************************************************************************
* Copyright (c) 2010 Kresimir Spes, Boris Mikic, Domagoj Cerjan                      *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef HLTYPES_ARRAY_H
#define HLTYPES_ARRAY_H

#include <vector>

namespace hltypes
{
	template <class T> class Array : std::vector<T>
	{
	public:
		Array();
		
		// operators
		T& operator[](int index) const;
		
		// HL methods
		int index_of(const T& element) const;
		bool contains(const T& element) const;
		void append(const T& element);
		void push_front(const T& element);
		T& pop_front();
		void insert_at(int index, const T& element);
		T& remove_at(int index);
		void reverse();
		Array<T> reversed();
		
		// HL operators
		//template <class T> void Array<T>::operator<<(const T& element); // same as push_back
		//template <class T> void Array<T>::operator+=(const T& element); // same as push_back
		//template <class T> T& Array<T>::operator-=(const T& element); // same as remove
		//template <class T> void Array<T>::operator+=(other Array<T>); // appends elements of other
		//template <class T> void Array<T>::operator-=(other Array<T>); // removes elements of other
		//template <class T> Array<T> Array<T>::operator+(other Array<T>); // merges with other
		//template <class T> Array<T> Array<T>::operator-(other Array<T>); // difference with other
		//template <class T> void Array<T>::operator&=(other Array<T>); // intersection with other
		//template <class T> void Array<T>::operator|=(other Array<T>); // union with other
		//template <class T> Array<T> Array<T>::operator&(other Array<T>); // intersects with other
		//template <class T> Array<T> Array<T>::operator|(other Array<T>); // unites with other
		
	};
}

#endif
