/************************************************************************************\
* This source file is part of the High Level C++ types library                       *
* For latest info, see http://libhltypes.sourceforge.net/                            *
**************************************************************************************
* Copyright (c) 2010 Kresimir Spes, Boris Mikic, Domagoj Cerjan                      *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef HLTYPES_MAP_H
#define HLTYPES_MAP_H

#include <map>

#include "harray.h"
#include "hstring.h"

#define foreach_m(type, name, container) for (std::map<hstr, type>::iterator name = container.begin(); name != container.end(); name++)

#define stdmap std::map<K, T>

namespace hltypes
{
	template <class K, class T> class Map : public stdmap
	{
	public:
        typedef typename std::map<K, T>::const_iterator iterator_t;
	
		// constructors
		Map() : stdmap()
		{
		}
		
		Map(const Map<K, T>& other) : stdmap()
		{
			for (iterator_t it = stdmap::begin(); it != stdmap::end(); it++)
			{
				this->operator[](it->first) = it->second;
			}
		}
		
/******* OPERATORS *****************************************************/
		
		T& operator[](const K& key)
		{
			return stdmap::operator[](key);
		}
		
		K& operator()(const T& value) const
		{
			return this->key_of(value);
		}
		
		bool operator==(const Map<K, T>& other)
		{
			return this->equals(other);
		}
		
		bool operator!=(const Map<K, T>& other)
		{
			return (!this->equals(other));
		}
		
/******* BASIC METHODS *************************************************/

		int size() const
		{
			return stdmap::size();
		}
		
		harray<K> keys() const
		{
			harray<K> result;
			for (iterator_t it = stdmap::begin(); it != stdmap::end(); it++)
			{
				result += it->first;
			}
			return result;
		}
	
		harray<T> values() const
		{
			harray<T> result;
			for (iterator_t it = stdmap::begin(); it != stdmap::end(); it++)
			{
				result += it->second;
			}
			return result;
		}
	
		bool equals(const Map<K, T>& other)
		{
			if (this->size() != other.size())
			{
				return false;
			}
			harray<K> keys = other.keys();
			if (!this->has_keys(keys))
			{
				return false;
			}
			for (iterator_t it = stdmap::begin(); it != stdmap::end(); it++)
			{
				if (stdmap::at(it->first) != other.at(it->first))
				{
					return false;
				}
			}
			return true;
		}
		
		K key_of(const T& value)
		{
			for (iterator_t it = stdmap::begin(); it != stdmap::end(); it++)
			{
				if (it->second == value)
				{
					return it->first;
				}
			}
			return stdmap::end()->first;
		}
		
		bool has_key(const K& key)
		{
			return (stdmap::find(key) != stdmap::end());
		}
		
		bool has_keys(const harray<K>& values)
		{
			for (int i = 0; i < values.size(); i++)
			{
				if (!this->has_key(values.at(i)))
				{
					return false;
				}
			}
			return true;
		}
		
		bool has_keys(const K values[], int count)
		{
			for (int i = 0; i < count; i++)
			{
				if (!this->has_key(values[i]))
				{
					return false;
				}
			}
			return true;
		}
		
		bool has_value(const T& value)
		{
			for (iterator_t it = stdmap::begin(); it != stdmap::end(); it++)
			{
				if (it->second == value)
				{
					return true;
				}
			}
			return false;
		}
		
		bool has_values(const harray<T>& values)
		{
			for (int i = 0; i < values.size(); i++)
			{
				if (!this->has_value(values.at(i)))
				{
					return false;
				}
			}
			return true;
		}
		
		bool has_values(const T values[], int count)
		{
			for (int i = 0; i < count; i++)
			{
				if (!this->has_value(values[i]))
				{
					return false;
				}
			}
			return true;
		}
		
		void insert(const K& key, const T& value)
		{
			stdmap::operator[](key) = value;
		}
		
		void insert(const Map<K, T>& other) // does not overwrite already existing values
		{
			stdmap::insert(other.begin, other.end());
		}
		
		void inject(const Map<K, T>& other) // overwrites already existing values
		{
			for (iterator_t it = other.begin(); it != other.end(); it++)
			{
				stdmap::operator[](it->first) = it->second;
			}
		}
		
		T remove_key(const K& key)
		{
			if (!this->has_key(key))
			{
				return NULL;
			}
			T result = this->operator[](key);
			stdmap::erase(key);
			return result;
		}
		
		void remove_keys(const harray<K>& keys)
		{
			for (int i = 0; i < keys.size(); i++)
			{
				if (this->has_key(keys.at(i)))
				{
					stdmap::erase(keys.at(i));
				}
			}
		}
		
		K remove_value(const T& value)
		{
			if (!this->has_value(value))
			{
				return NULL;
			}
			K result = this->key_of(value);
			stdmap::erase(result);
			return result;
		}
		
		void remove_values(const harray<T>& values)
		{
			for (int i = 0; i < values.size(); i++)
			{
				for (iterator_t it = stdmap::begin(); it != stdmap::end(); it++)
				{
					if (it->second == values.at(i))
					{
						stdmap::erase(it->first);
						break;
					}
				}
			}
		}
		
/******* HL FILTERS ****************************************************/

		Map<K, T> find_all(bool (*compare_function)(K, T))
		{
			Map<K, T> result;
			for (iterator_t it = stdmap::begin(); it != stdmap::end(); it++)
			{
				if (compare_function(it->first, it->second))
				{
					result[it->first] = it->second;
				}
			}
			return result;
		}
		
		bool matches_any(bool (*compare_function)(K, T))
		{
			for (iterator_t it = stdmap::begin(); it != stdmap::end(); it++)
			{
				if (compare_function(it->first, it->second))
				{
					return true;
				}
			}
			return false;
		}
		
		bool matches_all(bool (*compare_function)(K, T))
		{
			for (iterator_t it = stdmap::begin(); it != stdmap::end(); it++)
			{
				if (!compare_function(it->first, it->second))
				{
					return false;
				}
			}
			return true;
		}
		
		template <class L, class S>
		Map<L, S> cast()
		{
			Map<L, S> result;
			for (iterator_t it = stdmap::begin(); it != stdmap::end(); it++)
			{
				result[(L)it->first] = (S)it->second;
			}
			return result;
		}
		
	};
	
}

#define hmap hltypes::Map

#endif
