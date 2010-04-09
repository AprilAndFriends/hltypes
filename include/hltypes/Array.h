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
#include <algorithm>
#include "hltypesExport.h"

namespace hltypes
{
	template <class T> class hltypesExport Array : public std::vector<T>
	{
	public:
		// constructors
		Array() : index(-1), std::vector<T>()
		{
		}
		
		Array(const Array<T>& other) : index(-1), std::vector<T>()
		{
			this->insert_at(0, other, 0, (int)other.size());
		}
		
		Array(const Array<T>& other, const int count) : index(-1), std::vector<T>()
		{
			this->insert_at(0, other, 0, count);
		}
		
		Array(const Array<T>& other, const int start, const int count) : index(-1), std::vector<T>()
		{
			this->insert_at(0, other, start, count);
		}
		
		Array(const T other[], const int count) : index(-1), std::vector<T>()
		{
			this->insert_at(0, other, 0, count);
		}
		
		Array(const T other[], const int start, const int count) : index(-1), std::vector<T>()
		{
			this->insert_at(0, other, start, count);
		}
		
/******* OPERATORS *****************************************************/
		//NOTE: Does not work with bool, use Array::at directly instead.
		T& operator[](int index)
		{
			if (index < 0)
			{
				return std::vector<T>::at((int)std::vector<T>::size() - index);
			}
			return std::vector<T>::at(index);
		}
	
		bool operator==(const Array<T>& other)
		{
			return this->equals(other);
		}
		
		bool operator!=(const Array<T>& other)
		{
			return (!this->equals(other));
		}
		
/******* ITERATOR METHODS **********************************************/
		T* iterate(int start = 0)
		{
			this->index = start - 1;
			return this->next();
		}
		
		T* next(int step = 1)
		{
			this->index += step;
			if (this->index >= std::vector<T>::size())
			{
				return NULL;
			}
			return &std::vector<T>::at(this->index);
		}

/******* HL METHODS ****************************************************/
		int index_of(T element)
		{
			for (int i = 0; i < std::vector<T>::size(); i++)
			{
				if (element == std::vector<T>::at(i))
				{
					return i;
				}
			}
			return -1;
		}
		
		bool contains(T element)
		{
			return (this->index_of(element) >= 0);
		}
		
		void insert_at(const int index, const T& element)
		{
			std::vector<T>::insert(std::vector<T>::begin() + index, element);
		}
		
		void insert_at(const int index, const Array<T>& other, const int start, const int count)
		{
			for (int i = start; i < start + count; i++)
			{
				std::vector<T>::insert(std::vector<T>::begin() + index, other.at(i));
			}
		}
		
		void insert_at(const int index, const T other[], const int start, const int count)
		{
			for (int i = start; i < start + count; i++)
			{
				std::vector<T>::insert(std::vector<T>::begin() + index, other[i]);
			}
		}
		
		T& remove_at(const int index)
		{
			T& result = std::vector<T>::at(index);
			std::vector<T>::erase(std::vector<T>::begin() + index);
			return result;
		}
		
		void remove(T& element)
		{
			std::vector<T>::erase(std::vector<T>::begin() + this->index_of(element));
		}
		
		void remove(const Array<T>& other)
		{
			for (int i = 0; i < other.size(); i++)
			{
				std::vector<T>::erase(std::vector<T>::begin() + this->index_of(other.at(i)));
			}
		}
		
		void unite(const Array<T>& other)
		{
			Array<T> result;
			for (int i = 0; i < other.size(); i++)
			{
				if (this->contains(other.at(i)))
				{
					result.push_back(other.at(i));
				}
			}
			std::vector<T>::clear();
			this->insert_at((int)std::vector<T>::size(), result, 0, (int)result.size());
		}
		
		void intersect(const Array<T>& other)
		{
			Array<T> result;
			for (int i = 0; i < other.size(); i++)
			{
				if (!this->contains(other.at(i)))
				{
					result.push_back(other.at(i));
				}
			}
			this->insert_at((int)std::vector<T>::size(), result, 0, (int)result.size());
		}
		
		void remove_duplicates()
		{
			this->unite(this);
		}
		
		Array<T> united(const Array<T>& other) const
		{
			Array<T> result;
			result.insert_at(0, *this, 0, (int)std::vector<T>::size());
			result.unite(other);
			return result;
		}
		
		Array<T> intersected(const Array<T>& other) const
		{
			Array<T> result;
			result.insert_at(0, *this, 0, (int)std::vector<T>::size());
			result.intersect(other);
			return result;
		}
		
		Array<T> removed_duplicates()
		{
			return this->united(this);
		}
		
		void reverse()
		{
			reverse(this);
		}
		
		Array<T> reversed()
		{
			return reverse_copy(this);
		}
		
		bool equals(const Array<T>& other)
		{
			if (std::vector<T>::size() != other.size())
			{
				return false;
			}
			for (int i = 0; i < std::vector<T>::size(); i++)
			{
				if (std::vector<T>::at(i) != other.at(i))
				{
					return false;
				}
			}
			return true;
		}
		
/******* ITERATOR METHOD ALIASES ***************************************/
		T* iter(int start = 0)
		{
			return this->iterate(start);
		}
		
/******* HL METHOD ALIASES *********************************************/

		void includes(const T& element)
		{
			return this->contains(element);
		}
		
		void has(const T& element)
		{
			return this->contains(element);
		}
		
		void has_element(const T& element)
		{
			return this->contains(element);
		}
		
		void insert_at(const int index, const Array<T>& other)
		{
			this->insert_at(index, other, 0, (int)other.size());
		}
		
		void insert_at(const int index, const Array<T>& other, const int count)
		{
			this->insert_at(index, other, 0, count);
		}
		
		void insert_at(const int index, const T other[], const int count)
		{
			this->insert_at(index, other, 0, count);
		}
		
		void append(const T& element)
		{
			this->insert_at((int)std::vector<T>::size(), element);
		}
		
		void append(const Array<T>& other)
		{
			this->insert_at((int)std::vector<T>::size(), other, 0, (int)other.size());
		}
		
		void append(const Array<T>& other, const int count)
		{
			this->insert_at((int)std::vector<T>::size(), other, 0, count);
		}
		
		void append(const Array<T>& other, const int start, const int count)
		{
			this->insert_at((int)std::vector<T>::size(), other, start, count);
		}
		
		void append(const T other[], const int count)
		{
			this->insert_at((int)std::vector<T>::size(), other, 0, count);
		}
		
		void append(const T other[], const int start, const int count)
		{
			this->insert_at((int)std::vector<T>::size(), other, start, count);
		}
		
		void push_back(const T& element)
		{
			this->insert_at((int)std::vector<T>::size(), element);
		}
		
		void push_back(const Array<T>& other)
		{
			this->insert_at((int)std::vector<T>::size(), other, 0, (int)other.size());
		}
		
		void push_back(const Array<T>& other, const int count)
		{
			this->insert_at((int)std::vector<T>::size(), other, 0, count);
		}
		
		void push_back(const Array<T>& other, const int start, const int count)
		{
			this->insert_at((int)std::vector<T>::size(), other, start, count);
		}
		
		void push_back(const T other[], const int count)
		{
			this->insert_at((int)std::vector<T>::size(), other, 0, count);
		}
		
		void push_back(const T other[], const int start, const int count)
		{
			this->insert_at((int)std::vector<T>::size(), other, start, count);
		}
		
		void push_front(const T& element)
		{
			this->insert_at(0, element);
		}
		
		void push_front(const Array<T>& other)
		{
			this->insert_at(0, other);
		}
		
		void push_front(const Array<T>& other, const int count)
		{
			this->insert_at(0, other, 0, count);
		}
		
		void push_front(const Array<T>& other, const int start, const int count)
		{
			this->insert_at(0, other, start, count);
		}
		
		void push_front(const T other[], const int count)
		{
			this->insert_at(0, other, 0, count);
		}
		
		void push_front(const T other[], const int start, const int count)
		{
			this->insert_at(0, other, start, count);
		}
		
		T& pop_front()
		{
			return this->remove_at(0);
		}
		
		T& pop(int index)
		{
			return this->remove_at(index);
		}
		
/******* HL OPERATORS **************************************************/

		void operator<<(const T& element)
		{
			this->insert_at((int)std::vector<T>::size(), element);
		}
		
		void operator<<(const Array<T>& other)
		{
			this->insert_at((int)std::vector<T>::size(), other, 0, (int)other.size());
		}
		
		void operator+=(const T& element)
		{
			this->insert_at((int)std::vector<T>::size(), element);
		}
		
		void operator+=(const Array<T>& other)
		{
			this->insert_at((int)std::vector<T>::size(), other, 0, (int)other.size());
		}
		
		void operator-=(T& element)
		{
			this->remove(element);
		}
		
		void operator-=(const Array<T>& other)
		{
			this->remove(other);
		}
		
		void operator|=(const Array<T>& other)
		{
			this->unite(other);
		}
		
		void operator&=(const Array<T>& other)
		{
			this->intersect(other);
		}
		
	private:
		int index;
		
	};
	
}

#define harray hltypes::Array

template <class T> harray<T> operator+(const harray<T>& a, const harray<T>& b)
{
	harray<T> result(a);
	result += b;
	return result;
}

template <class T> harray<T> operator+(const harray<T>& a, T b)
{
	harray<T> result(a);
	result += b;
	return result;
}

template <class T> harray<T> operator-(const harray<T>& a, const harray<T>& b)
{
	harray<T> result(a);
	result -= b;
	return result;
}

template <class T> harray<T> operator-(const harray<T>& a, T b)
{
	harray<T> result(a);
	result -= b;
	return result;
}

template <class T> harray<T> operator|(const harray<T>& a, const harray<T>& b)
{
	return a.united(b);
}

template <class T> harray<T> operator&(const harray<T>& a, const harray<T>& b)
{
	return a.intersected(b);
}

#endif
