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
		
		Array(const Array<T>& elements) : index(-1), std::vector<T>()
		{
			this->insert_at(0, elements, 0, elements.size());
		}
		
		Array(const Array<T>& elements, const int count) : index(-1), std::vector<T>()
		{
			this->insert_at(0, elements, 0, count);
		}
		
		Array(const Array<T>& elements, const int start, const int count) : index(-1), std::vector<T>()
		{
			this->insert_at(0, elements, start, count);
		}
		
		Array(const T elements[], const int count) : index(-1), std::vector<T>()
		{
			this->insert_at(0, elements, 0, count);
		}
		
		Array(const T elements[], const int start, const int count) : index(-1), std::vector<T>()
		{
			this->insert_at(0, elements, start, count);
		}
		
/******* OPERATORS *****************************************************/
		T& operator[](int index)
		{
			return this->at(index);
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
		T* iter(int start = 0) const
		{
			this->index = start - 1;
			return this->next();
		}
		
		T* next(int step = 1) const
		{
			this->index += step;
			if (this->index >= this->size())
			{
				return NULL;
			}
			return this[this->index];
		}

/******* HL METHODS ****************************************************/
		int index_of(const T& element) const
		{
			for (int i = 0; i < this->size(); i++)
			{
				if (this.at(i) == element)
				{
					return i;
				}
			}
			return -1;
		}
		
		bool contains(const T& element) const
		{
			return (find(this->begin(), this->end(), element) != this->end());
		}
		
		void insert_at(const int index, const T& element)
		{
			this->insert(this->begin() + index, element);
		}
		
		void insert_at(const int index, const Array<T>& elements, const int start, const int count)
		{
			for (int i = start; i < start + count; i++)
			{
				this->insert(this->begin() + index, elements.at(i));
			}
		}
		
		void insert_at(const int index, const T elements[], const int start, const int count)
		{
			for (int i = start; i < start + count; i++)
			{
				this->insert(this->begin() + index, elements[i]);
			}
		}
		
		T& remove_at(const int index)
		{
			T& result = this->at(index);
			this->erase(this->begin() + index);
			return result;
		}
		
		void remove(const Array<T>& elements)
		{
			for (int i = 0; i < elements.size(); i++)
			{
				this->remove(elements.at(i));
			}
		}
		
		void unite(const Array<T>& elements)
		{
			Array<T> result;
			for (int i = 0; i < elements.size(); i++)
			{
				if (this->contains(elements.at(i)))
				{
					result.push_back(elements.at(i));
				}
			}
			this->clear();
			this->insert_at(this->size(), result);
		}
		
		void intersect(const Array<T>& elements)
		{
			Array<T> result;
			for (int i = 0; i < elements.size(); i++)
			{
				if (!this->contains(elements.at(i)))
				{
					result.push_back(elements.at(i));
				}
			}
			this->insert_at(this->size(), result);
		}
		
		void remove_duplicates()
		{
			this->unite(this);
		}
		
		Array<T> united(const Array<T>& elements) const
		{
			Array<T> result;
			result.insert_at(0, this, 0, this->size());
			result.unite(elements);
			return result;
		}
		
		Array<T> intersected(const Array<T>& elements) const
		{
			Array<T> result;
			result.insert_at(0, this, 0, this->size());
			result.intersect(elements);
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
			if (this->size() != other.size())
			{
				return false;
			}
			for (int i = 0; i < this->size(); i++)
			{
				if (this->at(i) != other.at(i))
				{
					return false;
				}
			}
			return true;
		}
		
/******* HL METHOD ALIASES *********************************************/

		void includes(T& element)
		{
			return this->contains(element);
		}
		
		void has(T& element)
		{
			return this->contains(element);
		}
		
		void has_element(T& element)
		{
			return this->contains(element);
		}
		
		void insert_at(const int index, const Array<T>& elements)
		{
			this->insert_at(index, elements, 0, elements.size());
		}
		
		void insert_at(const int index, const Array<T>& elements, const int count)
		{
			this->insert_at(index, elements, 0, count);
		}
		
		void insert_at(const int index, const T elements[], const int count)
		{
			this->insert_at(index, elements, 0, count);
		}
		
		void append(const T& element)
		{
			this->insert_at(this->size(), element);
		}
		
		void append(const Array<T>& elements)
		{
			this->insert_at(this->size(), elements, 0, elements.size());
		}
		
		void append(const Array<T>& elements, const int count)
		{
			this->insert_at(this->size(), elements, 0, count);
		}
		
		void append(const Array<T>& elements, const int start, const int count)
		{
			this->insert_at(this->size(), elements, start, count);
		}
		
		void append(const T elements[], const int count)
		{
			this->insert_at(this->size(), elements, 0, count);
		}
		
		void append(const T elements[], const int start, const int count)
		{
			this->insert_at(this->size(), elements, start, count);
		}
		
		void push_back(const T& element)
		{
			this->insert_at(this->size(), element);
		}
		
		void push_back(const Array<T>& elements)
		{
			this->insert_at(this->size(), elements, 0, elements.size());
		}
		
		void push_back(const Array<T>& elements, const int count)
		{
			this->insert_at(this->size(), elements, 0, count);
		}
		
		void push_back(const Array<T>& elements, const int start, const int count)
		{
			this->insert_at(this->size(), elements, start, count);
		}
		
		void push_back(const T elements[], const int count)
		{
			this->insert_at(this->size(), elements, 0, count);
		}
		
		void push_back(const T elements[], const int start, const int count)
		{
			this->insert_at(this->size(), elements, start, count);
		}
		
		void push_front(const T& element)
		{
			this->insert_at(0, element);
		}
		
		void push_front(const Array<T>& elements)
		{
			this->insert_at(0, elements);
		}
		
		void push_front(const Array<T>& elements, const int count)
		{
			this->insert_at(0, elements, 0, count);
		}
		
		void push_front(const Array<T>& elements, const int start, const int count)
		{
			this->insert_at(0, elements, start, count);
		}
		
		void push_front(const T elements[], const int count)
		{
			this->insert_at(0, elements, 0, count);
		}
		
		void push_front(const T elements[], const int start, const int count)
		{
			this->insert_at(0, elements, start, count);
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
			this->insert_at(this->size(), element);
		}
		
		void operator<<(const Array<T>& elements)
		{
			this->insert_at(this->size(), elements, 0, elements.size());
		}
		
		void operator+=(const T& element)
		{
			this->insert_at(this->size(), element);
		}
		
		void operator+=(const Array<T>& elements)
		{
			this->insert_at(this->size(), elements, 0, elements.size());
		}
		
		void operator-=(const T& element)
		{
			this->remove(element);
		}
		
		void operator-=(const Array<T>& elements)
		{
			this->remove(elements);
		}
		
		void operator|=(const Array<T>& elements)
		{
			this->unite(elements);
		}
		
		void operator&=(const Array<T>& elements)
		{
			this->interset(elements);
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

template <class T> harray<T> operator-(const harray<T>& a, const harray<T>& b)
{
	harray<T> result(a);
	result -= b;
	return result;
}

template <class T> harray<T> operator|(const harray<T>& a, const harray<T>& b)
{
	return a->united(b);
}

template <class T> harray<T> operator&(const harray<T>& a, const harray<T>& b)
{
	return a->intersected(b);
}

#endif
