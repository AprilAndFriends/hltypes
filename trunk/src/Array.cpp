/************************************************************************************\
* This source file is part of the High Level C++ types library                       *
* For latest info, see http://libhltypes.sourceforge.net/                            *
**************************************************************************************
* Copyright (c) 2010 Kresimir Spes, Boris Mikic, Domagoj Cerjan                      *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#define _CRT_SECURE_NO_WARNINGS // damn visual studio warnings

#include <vector>
#include <algorithm>
#include "Array.h"

namespace hltypes
{
/******* OPERATORS *****************************************************/
	template <class T> T& Array<T>::operator[](int index) const
	{
		return std::vector<T>::operator[](index);
	}
	
	/******* HL METHODS ****************************************************/
		template <class T> int Array<T>::index_of(const T& element) const
		{
			for (int i = 0; i < this->size(); i++)
			{
				if (this[i] == element)
				{
					return i;
				}
			}
			return -1;
		}
		
		template <class T> bool Array<T>::contains(const T& element) const
		{
			//std::vector<T>::iterator it = this->begin();
			iterator it = this->begin();
			return (find(this->begin(), this->end(), element) != this->end());
		}
		
		template <class T> void Array<T>::append(const T& element)
		{
			this->push_back(element);
		}
		
		template <class T> void Array<T>::push_front(const T& element)
		{
			this->insert(this->begin(), element);
		}
		
		template <class T> T& Array<T>::pop_front()
		{
			T& result = this->front();
			this->erase(this->begin());
			return result;
		}
		
		template <class T> void Array<T>::insert_at(int index, const T& element)
		{
			this->insert(this->begin() + index, element);
		}
		
		template <class T> T& Array<T>::remove_at(int index)
		{
			//2DO - how to use an iterator in a derived template class so "begin() + index" isn't there twice
			
			T& result = *(this->begin() + index);
			this->erase(this->begin() + index);
			return result;
		}
		
		template <class T> void Array<T>::reverse()
		{
			reverse(this);
		}
		
		template <class T> Array<T> Array<T>::reversed()
		{
			return reverse_copy(this);
		}
	
}
