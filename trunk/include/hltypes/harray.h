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


namespace hltypes
{
#define stdvector std::vector<T>
	
	template <class T> class Array : public stdvector
	{
	public:
		// constructors
		Array() : index(-1), stdvector()
		{
		}
		
		Array(const Array<T>& other) : index(-1), stdvector()
		{
			this->insert_at(0, other, 0, (int)other.size());
		}
		
		Array(const Array<T>& other, const int count) : index(-1), stdvector()
		{
			this->insert_at(0, other, 0, count);
		}
		
		Array(const Array<T>& other, const int start, const int count) : index(-1), stdvector()
		{
			this->insert_at(0, other, start, count);
		}
		
		Array(const T other[], const int count) : index(-1), stdvector()
		{
			this->insert_at(0, other, 0, count);
		}
		
		Array(const T other[], const int start, const int count) : index(-1), stdvector()
		{
			this->insert_at(0, other, start, count);
		}
		
/******* OPERATORS *****************************************************/
		//NOTE: Does not work with bool, use Array::at directly instead.
		T& operator[](int index)
		{
			if (index < 0)
			{
				return stdvector::at((int)stdvector::size() - index);
			}
			return stdvector::at(index);
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
			if (this->index >= stdvector::size())
			{
				return NULL;
			}
			return &stdvector::at(this->index);
		}

		T* riterate(int start = 0)
		{
			this->index = start - 1;
			return this->rnext();
		}
		
		T* rnext(int step = 1)
		{
			this->index += step;
			if (this->index >= stdvector::size())
			{
				return NULL;
			}
			return &stdvector::at(stdvector::size() - 1 - this->index);
		}

		int iterator_index()
		{
			return this->index;
		}
		
/******* ITERATOR METHOD ALIASES ***************************************/
		T* iter(int start = 0)
		{
			return this->iterate(start);
		}
		
		T* riter(int start = 0)
		{
			return this->riterate(start);
		}
		
/******* HL METHODS ****************************************************/
		int index_of(T element)
		{
			for (int i = 0; i < stdvector::size(); i++)
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
		
		void insert_at(const int index, const T& element)
		{
			stdvector::insert(stdvector::begin() + index, element);
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
		
		T remove_at(const int index)
		{
			T result = stdvector::at(index);
			stdvector::erase(stdvector::begin() + index);
			return result;
		}
		
		void remove(T& element)
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
		
		void unite(const Array<T>& other)
		{
			this->insert_at((int)stdvector::size(), other, 0, other.size());
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
		
		void differenciate(const Array<T>& other)
		{
			Array<T> result;
			for (int i = 0; i < other.size(); i++)
			{
				if (this->contains(other.at(i)))
				{
					result.push_back(other.at(i));
				}
			}
			this->remove(result);
		}
		
		Array<T> differenciated(const Array<T>& other) const
		{
			Array<T> result(*this);
			result.differenciate(other);
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
			for (int i = 0; i < stdvector::size(); i++)
			{
				if (!result.contains(stdvector::at(i)))
				{
					result.push_back(stdvector::at(i));
				}
			}
			this->assign(result.begin(), result.end());
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
			if (stdvector::size() == 0)
			{
				return NULL;
			}
			return (*std::min_element(stdvector::begin(), stdvector::end()));
		}
		
		T max()
		{
			if (stdvector::size() == 0)
			{
				return NULL;
			}
			return (*std::max_element(stdvector::begin(), stdvector::end()));
		}
		
		bool equals(const Array<T>& other)
		{
			if (stdvector::size() != other.size())
			{
				return false;
			}
			for (int i = 0; i < stdvector::size(); i++)
			{
				if (stdvector::at(i) != other.at(i))
				{
					return false;
				}
			}
			return true;
		}
		
		hstr join(chstr separator)
		{
			hstr result;
			if (stdvector::size() > 0)
			{
				result += stdvector::at(0);
				for (int i = 1; i < stdvector::size(); i++)
				{
					result += separator;
					result += stdvector::at(i);
				}
			}
			return result;
		}
		
/******* HL METHOD ALIASES *********************************************/

		bool includes(const T& element)
		{
			return this->contains(element);
		}
		
		bool has(const T& element)
		{
			return this->contains(element);
		}
		
		bool has_element(const T& element)
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
			this->insert_at((int)stdvector::size(), element);
		}
		
		void append(const Array<T>& other)
		{
			this->insert_at((int)stdvector::size(), other, 0, (int)other.size());
		}
		
		void append(const Array<T>& other, const int count)
		{
			this->insert_at((int)stdvector::size(), other, 0, count);
		}
		
		void append(const Array<T>& other, const int start, const int count)
		{
			this->insert_at((int)stdvector::size(), other, start, count);
		}
		
		void append(const T other[], const int count)
		{
			this->insert_at((int)stdvector::size(), other, 0, count);
		}
		
		void append(const T other[], const int start, const int count)
		{
			this->insert_at((int)stdvector::size(), other, start, count);
		}
		
		void push_back(const T& element)
		{
			this->insert_at((int)stdvector::size(), element);
		}
		
		void push_back(const Array<T>& other)
		{
			this->insert_at((int)stdvector::size(), other, 0, (int)other.size());
		}
		
		void push_back(const Array<T>& other, const int count)
		{
			this->insert_at((int)stdvector::size(), other, 0, count);
		}
		
		void push_back(const Array<T>& other, const int start, const int count)
		{
			this->insert_at((int)stdvector::size(), other, start, count);
		}
		
		void push_back(const T other[], const int count)
		{
			this->insert_at((int)stdvector::size(), other, 0, count);
		}
		
		void push_back(const T other[], const int start, const int count)
		{
			this->insert_at((int)stdvector::size(), other, start, count);
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
		
		T pop_front()
		{
			return this->remove_at(0);
		}
		
		T pop_back()
		{
			return this->remove_at((int)stdvector::size() - 1);
		}
		
		T pop(int index)
		{
			return this->remove_at(index);
		}
		
/******* HL OPERATORS **************************************************/

		void operator<<(const T& element)
		{
			this->insert_at((int)stdvector::size(), element);
		}
		
		void operator<<(const Array<T>& other)
		{
			this->insert_at((int)stdvector::size(), other, 0, (int)other.size());
		}
		
		void operator+=(const T& element)
		{
			this->insert_at((int)stdvector::size(), element);
		}
		
		void operator+=(const Array<T>& other)
		{
			this->insert_at((int)stdvector::size(), other, 0, (int)other.size());
		}
		
		void operator-=(T element)
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
		
		void operator/=(const Array<T>& other)
		{
			this->differenciate(other);
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

template <class T> harray<T> operator/(const harray<T>& a, const harray<T>& b)
{
	return a.differenciated(b);
}

#endif
