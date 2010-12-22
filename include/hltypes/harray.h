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

#include "hstring.h"

#ifdef _WIN32
	#undef min
	#undef max
#endif

#define foreach(type, name, container) for (std::vector<type>::iterator name = (container).begin(); name != (container).end(); name++)
#define foreach_r(type, name, container) for (std::vector<type>::reverse_iterator name = (container).rbegin(); name != (container).rend(); name++)

#define stdvector std::vector<T>

namespace hltypes
{
	template <class T> class Array : public stdvector
	{
	public:
		// constructors
		Array() : stdvector()
		{
		}
		
		Array(const Array<T>& other) : stdvector(other)
		{
		}
		
		Array(const Array<T>& other, const int count) : stdvector()
		{
			this->insert_at(0, other, 0, count);
		}
		
		Array(const Array<T>& other, const int start, const int count) : stdvector()
		{
			this->insert_at(0, other, start, count);
		}
		
		Array(const T other[], const int count) : stdvector()
		{
			this->insert_at(0, other, 0, count);
		}
		
		Array(const T other[], const int start, const int count) : stdvector()
		{
			this->insert_at(0, other, start, count);
		}
		
/******* OPERATORS *****************************************************/
		//NOTE: Does not work with bool, use Array::at directly instead.
		T& operator[](const int index)
		{
			if (index < 0)
			{
				return stdvector::at(this->size() + index);
			}
			return stdvector::at(index);
		}
		const T& operator[](const int index) const
		{
			if (index < 0)
			{
				return stdvector::at(this->size() + index);
			}
			return stdvector::at(index);
		}
	
		Array<T> operator()(const int start, const int count) const
		{
			Array<T> result;
			result.assign(stdvector::begin() + start, stdvector::begin() + start + count);
			return result;
		}
	
		bool operator==(const Array<T>& other) const
		{
			return this->equals(other);
		}
		
		bool operator!=(const Array<T>& other) const
		{
			return (!this->equals(other));
		}
		
/******* BASIC METHODS *************************************************/

		int size() const
		{
			return stdvector::size();
		}
		
		bool equals(const Array<T>& other)
		{
			if (this->size() != other.size())
			{
				return false;
			}
			for (int i = 0; i < this->size(); i++)
			{
				if (stdvector::at(i) != other.at(i))
				{
					return false;
				}
			}
			return true;
		}
		
		int index_of(T element)
		{
			for (int i = 0; i < this->size(); i++)
			{
				if (element == stdvector::at(i))
				{
					return i;
				}
			}
			return -1;
		}
		
		bool contains(const T& element)
		{
			return (this->index_of(element) >= 0);
		}
		
		bool contains(const Array<T>& other)
		{
			for (int i = 0; i < other.size(); i++)
			{
				if (!this->contains(other.at(i)))
				{
					return false;
				}
			}
			return true;
		}
		
		bool contains(const T other[], int count)
		{
			for (int i = 0; i < count; i++)
			{
				if (!this->contains(other[i]))
				{
					return false;
				}
			}
			return true;
		}
		
		int count(T element)
		{
			int result = 0;
			for (int i = 0; i < this->size(); i++)
			{
				if (element == stdvector::at(i))
				{
					result++;
				}
			}
			return result;
		}
		
		void insert_at(const int index, const T& element, int times = 1)
		{
			stdvector::insert(stdvector::begin() + index, times, element);
		}
		
		void insert_at(const int index, const Array<T>& other, const int start, const int count)
		{
			stdvector::insert(stdvector::begin() + index, other.begin() + start,
				other.begin() + start + count);
		}
		
		void insert_at(const int index, const T other[], const int start, const int count)
		{
			stdvector::insert(stdvector::begin() + index, other + start, other + start + count);
		}
		
		void insert_at(const int index, const Array<T>& other)
		{
			this->insert_at(index, other, 0, other.size());
		}
		
		void insert_at(const int index, const Array<T>& other, const int count)
		{
			this->insert_at(index, other, 0, count);
		}
		
		void insert_at(const int index, const T other[], const int count)
		{
			this->insert_at(index, other, 0, count);
		}
		
		T remove_at(const int index)
		{
			T result = stdvector::at(index);
			stdvector::erase(stdvector::begin() + index);
			return result;
		}
		
		void remove(T element)
		{
			stdvector::erase(stdvector::begin() + this->index_of(element));
		}
		
		void remove(const Array<T>& other)
		{
			for (int i = 0; i < other.size(); i++)
			{
				stdvector::erase(stdvector::begin() + this->index_of(other.at(i)));
			}
		}
		
		void remove_all(T& element)
		{
			int index = 0;
			while (true)
			{
				index = this->index_of(element);
				if (index < 0)
				{
					break;
				}
				stdvector::erase(stdvector::begin() + index);
			}
		}
		
		void remove_all(const Array<T>& other)
		{
			int index = 0;
			for (int i = 0; i < other.size(); i++)
			{
				while (true)
				{
					index = this->index_of(other.at(i));
					if (index < 0)
					{
						break;
					}
					stdvector::erase(stdvector::begin() + index);
				}
			}
		}
		
		void push_back(const T& element, int times)
		{
			for (int i = 0; i < times; i++)
			{
				stdvector::push_back(element);
			}
		}
		
		void push_back(const T& element)
		{
			stdvector::push_back(element);
		}
		
		void push_back(const Array<T>& other)
		{
			for (int i = 0; i < other.size(); i++)
			{
				stdvector::push_back(other.at(i));
			}
		}
		
		void push_back(const Array<T>& other, const int count)
		{
			for (int i = 0; i < count; i++)
			{
				stdvector::push_back(other.at(i));
			}
		}
		
		void push_back(const Array<T>& other, const int start, const int count)
		{
			for (int i = 0; i < count; i++)
			{
				stdvector::push_back(other.at(start + i));
			}
		}
		
		void push_back(const T other[], const int count)
		{
			for (int i = 0; i < count; i++)
			{
				stdvector::push_back(other[i]);
			}
		}
		
		void push_back(const T other[], const int start, const int count)
		{
			for (int i = 0; i < count; i++)
			{
				stdvector::push_back(other[start + i]);
			}
		}
		
		void push_front(const T& element, int times = 1)
		{
			this->insert_at(0, element, times);
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
		
		T pop_front()
		{
			return this->remove_at(0);
		}
		
		T pop_back()
		{
			T element = stdvector::back();
			stdvector::pop_back();
			return element;
		}
		
		T pop(int index)
		{
			return this->remove_at(index);
		}
		
		Array<T> pop_front(int count)
		{
			Array<T> result;
			result.assign(stdvector::begin(), stdvector::begin() + count);
			stdvector::erase(stdvector::begin(), stdvector::begin() + count);
			return result;
		}
		
		Array<T> pop_back(int count)
		{
			Array<T> result;
			result.assign(stdvector::end() - count, stdvector::end());
			stdvector::erase(stdvector::end() - count, stdvector::end());
			return result;
		}
		
/******* HL METHODS ****************************************************/

		void unite(const Array<T>& other)
		{
			this->insert_at(this->size(), other, 0, other.size());
			this->remove_duplicates();
		}
		
		Array<T> united(const Array<T>& other) const
		{
			Array<T> result(*this);
			result.unite(other);
			return result;
		}
		
		void intersect(const Array<T>& other)
		{
			Array<T> result;
			for (int i = 0; i < other.size(); i++)
			{
				if (this->contains(other.at(i)))
				{
					result.push_back(other.at(i));
				}
			}
			stdvector::assign(result.begin(), result.end());
		}
		
		Array<T> intersected(const Array<T>& other) const
		{
			Array<T> result(*this);
			result.intersect(other);
			return result;
		}
		
		void differentiate(const Array<T>& other)
		{
			int index;
			for (int i = 0; i < other.size(); i++)
			{
				index = this->index_of(other.at(i));
				if (index >= 0)
				{
					stdvector::erase(stdvector::begin() + index);
				}
			}
		}
		
		Array<T> differentiated(const Array<T>& other) const
		{
			Array<T> result(*this);
			result.differentiate(other);
			return result;
		}
		
		void reverse()
		{
			std::reverse(stdvector::begin(), stdvector::end());
		}
		
		Array<T> reversed()
		{
			Array<T> result(*this);
			result.reverse();
			return result;
		}
		
		void remove_duplicates()
		{
			Array<T> result;
			for (int i = 0; i < this->size(); i++)
			{
				if (!result.contains(stdvector::at(i)))
				{
					result.push_back(stdvector::at(i));
				}
			}
			stdvector::assign(result.begin(), result.end());
		}
		
		Array<T> removed_duplicates()
		{
			Array<T> result(*this);
			result.remove_duplicates();
			return result;
		}
		
		void sort()
		{
			std::stable_sort(stdvector::begin(), stdvector::end());
		}
		
		void sort(bool (*compare_function)(T, T))
		{
			std::stable_sort(stdvector::begin(), stdvector::end(), compare_function);
		}
		
		Array<T> sorted()
		{
			Array<T> result(*this);
			result.sort();
			return result;
		}
		
		Array<T> sorted(bool (*compare_function)(T, T))
		{
			Array<T> result(*this);
			result.sort(compare_function);
			return result;
		}
		
		void randomize()
		{
			std::random_shuffle(stdvector::begin(), stdvector::end());
		}
		
		Array<T> randomized()
		{
			Array<T> result(*this);
			result.randomize();
			return result;
		}
		
		T min()
		{
			if (this->size() == 0)
			{
				return NULL;
			}
			return (*std::min_element(stdvector::begin(), stdvector::end()));
		}
		
		T min(bool (*compare_function)(T, T))
		{
			if (this->size() == 0)
			{
				return NULL;
			}
			return (*std::min_element(stdvector::begin(), stdvector::end(), compare_function));
		}
		
		T max()
		{
			if (this->size() == 0)
			{
				return NULL;
			}
			return (*std::max_element(stdvector::begin(), stdvector::end()));
		}
		
		T max(bool (*compare_function)(T, T))
		{
			if (this->size() == 0)
			{
				return NULL;
			}
			return (*std::max_element(stdvector::begin(), stdvector::end(), compare_function));
		}
		
		hstr join(chstr separator)
		{
			hstr result;
			if (this->size() > 0)
			{
				result += stdvector::at(0);
				for (int i = 1; i < this->size(); i++)
				{
					result += separator;
					result += stdvector::at(i);
				}
			}
			return result;
		}
		
/******* HL FILTERS ****************************************************/

		Array<T> find_all(bool (*compare_function)(T))
		{
			Array<T> result;
			for (int i = 0; i < this->size(); i++)
			{
				if (compare_function(stdvector::at(i)))
				{
					result += stdvector::at(i);
				}
			}
			return result;
		}
		
		T* find_first(bool (*compare_function)(T))
		{
			for (int i = 0; i < this->size(); i++)
			{
				if (compare_function(stdvector::at(i)))
				{
					return &stdvector::at(i);
				}
			}
			return NULL;
		}
		
		bool matches_any(bool (*compare_function)(T))
		{
			for (int i = 0; i < this->size(); i++)
			{
				if (compare_function(stdvector::at(i)))
				{
					return true;
				}
			}
			return false;
		}
		
		bool matches_all(bool (*compare_function)(T))
		{
			for (int i = 0; i < this->size(); i++)
			{
				if (!compare_function(stdvector::at(i)))
				{
					return false;
				}
			}
			return true;
		}
		
		template <class S>
		Array<S> cast()
		{
			Array<S> result;
			for (int i = 0; i < this->size(); i++)
			{
				result += (S)stdvector::at(i);
			}
			return result;
		}
		
/******* HL METHOD ALIASES *********************************************/

		bool includes(const T& element)				{ return this->contains(element); }
		bool includes(const Array<T>& other)		{ return this->contains(other); }
		bool includes(const T other[], int count)	{ return this->contains(other, count); }
		
		bool has(const T& element)				{ return this->contains(element); }
		bool has(const Array<T>& other)			{ return this->contains(other); }
		bool has(const T other[], int count)	{ return this->contains(other, count); }
		
		bool has_element(const T& element)				{ return this->contains(element); }
		bool has_element(const Array<T>& other)			{ return this->contains(other); }
		bool has_element(const T other[], int count)	{ return this->contains(other, count); }
		
		void add(const T& element)											{ this->push_back(element); }
		void add(const T& element, int times)								{ this->push_back(element, times); }
		void add(const Array<T>& other)										{ this->push_back(other); }
		void add(const Array<T>& other, const int count)					{ this->push_back(other, count); }
		void add(const Array<T>& other, const int start, const int count)	{ this->push_back(other, start, count); }
		void add(const T other[], const int count)							{ this->push_back(other, count); }
		void add(const T other[], const int start, const int count)			{ this->push_back(other, start, count); }
		
		void append(const T& element)											{ this->push_back(element); }
		void append(const T& element, int times)								{ this->push_back(element, times); }
		void append(const Array<T>& other)										{ this->push_back(other); }
		void append(const Array<T>& other, const int count)						{ this->push_back(other, count); }
		void append(const Array<T>& other, const int start, const int count)	{ this->push_back(other, start, count); }
		void append(const T other[], const int count)							{ this->push_back(other, count); }
		void append(const T other[], const int start, const int count)			{ this->push_back(other, start, count); }
		
/******* HL OPERATORS **************************************************/

		Array<T>& operator<<(const T& element)
		{
			this->push_back(element);
			return *this;
		}
		
		Array<T>& operator<<(const Array<T>& other)
		{
			this->push_back(other);
			return *this;
		}
		
		Array<T>& operator+=(const T& element)
		{
			this->push_back(element);
			return *this;
		}
		
		Array<T>& operator+=(const Array<T>& other)
		{
			this->push_back(other);
			return *this;
		}
		
		Array<T>& operator-=(T element)
		{
			this->remove(element);
			return *this;
		}
		
		Array<T>& operator-=(const Array<T>& other)
		{
			this->remove(other);
			return *this;
		}
		
		Array<T>& operator|=(const Array<T>& other)
		{
			this->unite(other);
			return *this;
		}
		
		Array<T>& operator&=(const Array<T>& other)
		{
			this->intersect(other);
			return *this;
		}
		
		Array<T>& operator/=(const Array<T>& other)
		{
			this->differentiate(other);
			return *this;
		}
		
/******* HL NONMODIFYINGOPERATORS *****************************************/

		Array<T> operator+(const Array<T>& other) const
		{
			Array<T> result(*this);
			result += other;
			return result;
		}
		
		Array<T> operator+(const T& other) const
		{
			Array<T> result(*this);
			result += other;
			return result;
		}
		
		Array<T> operator-(const Array<T>& other) const
		{
			Array<T> result(*this);
			result -= other;
			return result;
		}
		
		Array<T> operator-(const T& other) const
		{
			Array<T> result(*this);
			result -= other;
			return result;
		}
		
		Array<T> operator|(const Array<T>& other) const
		{
			return united(other);
		}
		Array<T> operator&(const Array<T>& other) const
		{
			return intersected(other);
		}
		Array<T> operator/(const Array<T>& other) const
		{
			return differentiated(other);
		}
		
		
		
	};
	
}

#define harray hltypes::Array

#endif
