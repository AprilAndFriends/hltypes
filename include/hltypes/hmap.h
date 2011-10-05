/// @file
/// @author  Boris Mikic
/// @version 1.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Encapsulates std::map and adds high level methods.

#ifndef HLTYPES_MAP_H
#define HLTYPES_MAP_H

#include <map>

#include "harray.h"
#include "hstring.h"

/// @brief Provides a simpler syntax to iterate through a Map.
#define foreach_map(type_key, type_value, name, container) for (std::map<type_key, type_value>::iterator name = container.begin(); name != container.end(); name++)
/// @brief Provides a simpler syntax to iterate through a Map with String as key.
#define foreach_m(type, name, container) for (std::map<hstr, type>::iterator name = container.begin(); name != container.end(); name++)
/// @brief Alias for simpler code.
#define stdmap std::map<K, V>

namespace hltypes
{
	/// @brief Encapsulates std::map and adds high level methods.
	/// @author Boris Mikic
	template <class K, class V> class Map : public stdmap
	{
	private:
        typedef typename std::map<K, V>::const_iterator iterator_map_t;
        typedef typename std::vector<K>::const_iterator iterator_map_key_t;
	public:
		/// @brief Empty constructor.
		Map() : stdmap()
		{
		}
		/// @brief Copy constructor.
		/// @param[in] other Map to copy.
		Map(const Map<K, V>& other) : stdmap(other)
		{
		}
		/// @brief Destructor.
		~Map()
		{
		}
		/// @brief Returns value with specified key.
		/// @param[in] key Key of the value.
		/// @return Value with specified key.
		V& operator[](const K& key)
		{
			return stdmap::operator[](key);
		}
		/// @brief Same as key_of.
		/// @see key_of
		K& operator()(const V& value) const
		{
			return this->key_of(value);
		}
		/// @brief Same as equals.
		/// @see equals
		bool operator==(const Map<K, V>& other)
		{
			return this->equals(other);
		}
		/// @brief Same as nequals.
		/// @see nequals
		bool operator!=(const Map<K, V>& other)
		{
			return this->nequals(other);
		}
		/// @brief Returns the number of values in the Map.
		/// @return The number of values in the Map.
		int size() const
		{
			return stdmap::size();
		}
		/// @brief Returns an Array with all keys.
		/// @return An Array with all keys.
		harray<K> keys() const
		{
			harray<K> result;
			for (iterator_map_t it = stdmap::begin(); it != stdmap::end(); it++)
			{
				result += it->first;
			}
			return result;
		}
		/// @brief Returns an Array with all values.
		/// @return An Array with all values.
		harray<V> values() const
		{
			harray<V> result;
			for (iterator_map_t it = stdmap::begin(); it != stdmap::end(); it++)
			{
				result += it->second;
			}
			return result;
		}
		/// @brief Returns an Array with all values in the same order as the given corresponding keys.
		/// @return An Array with all values in the same order as the given corresponding keys.
		harray<V> values(harray<K> keys) const
		{
			harray<V> result;
			for (iterator_map_key_t it = keys.begin(); it != keys.end(); it++)
			{
				result += stdmap::at(*it);
			}
			return result;
		}
		/// @brief Compares the contents of two Maps for being equal.
		/// @param[in] other Another Map.
		/// @return True if number of keys and values are equal and all pairs of keys and values are equal.
		bool equals(const Map<K, V>& other)
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
			for (iterator_map_t it = stdmap::begin(); it != stdmap::end(); it++)
			{
				// making sure operator== is used, not !=
				if (!(stdmap::at(it->first) == other.at(it->first)))
				{
					return false;
				}
			}
			return true;
		}
		/// @brief Compares the contents of two Maps for being not equal.
		/// @param[in] other Another Map.
		/// @return True if number of keys and values are not equal or at least one pair of keys and values is not equal.
		bool nequals(const Map<K, V>& other)
		{
			if (this->size() != other.size())
			{
				return true;
			}
			harray<K> keys = other.keys();
			if (!this->has_keys(keys))
			{
				return true;
			}
			for (iterator_map_t it = stdmap::begin(); it != stdmap::end(); it++)
			{
				// making sure operator!= is used, not ==
				if (stdmap::at(it->first) != other.at(it->first))
				{
					return true;
				}
			}
			return false;
		}
		/// @brief Returns key of specified value.
		/// @param[in] value Value with the given key.
		/// @return Key of specified value.
		K key_of(const V& value)
		{
			for (iterator_map_t it = stdmap::begin(); it != stdmap::end(); it++)
			{
				if (it->second == value)
				{
					return it->first;
				}
			}
			return stdmap::end()->first;
		}
		/// @brief Checks for existence of a key.
		/// @param[in] key Key to check.
		/// @return True if key is present.
		bool has_key(const K& key)
		{
			return (stdmap::find(key) != stdmap::end());
		}
		/// @brief Checks for existence of all keys.
		/// @param[in] keys Array of keys to check.
		/// @return True if all keys are present.
		bool has_keys(const harray<K>& keys)
		{
			for (int i = 0; i < keys.size(); i++)
			{
				if (!this->has_key(keys.at(i)))
				{
					return false;
				}
			}
			return true;
		}
		/// @brief Checks for existence of all keys.
		/// @param[in] keys C-type array of keys to check.
		/// @param[in] count Number of keys to check.
		/// @return True if all keys are present.
		bool has_keys(const K keys[], const int count)
		{
			for (int i = 0; i < count; i++)
			{
				if (!this->has_key(keys[i]))
				{
					return false;
				}
			}
			return true;
		}
		/// @brief Checks for existence of a value.
		/// @param[in] value Value to check.
		/// @return True if value is present.
		bool has_value(const V& value)
		{
			for (iterator_map_t it = stdmap::begin(); it != stdmap::end(); it++)
			{
				if (it->second == value)
				{
					return true;
				}
			}
			return false;
		}
		/// @brief Checks for existence of all values.
		/// @param[in] values Array of values to check.
		/// @return True if all values are present.
		bool has_values(const harray<V>& values)
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
		/// @brief Checks for existence of all values.
		/// @param[in] values C-type array of values to check.
		/// @param[in] count Number of values to check.
		/// @return True if all values are present.
		bool has_values(const V values[], const int count)
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
		/// @brief Adds a new pair of key and value into the Map.
		/// @param[in] key Key of the entry.
		/// @param[in] value Value of the entry.
		void insert(const K& key, const V& value)
		{
			stdmap::operator[](key) = value;
		}
		/// @brief Adds all pairs of keys and values from another Map into this one.
		/// @param[in] other Another Map.
		/// @note Entries with already existing keys will not be overwritten.
		void insert(const Map<K, V>& other)
		{
			stdmap::insert(other.begin, other.end());
		}
		/// @brief Adds all pairs of keys and values from another Map into this one.
		/// @param[in] other Another Map.
		/// @note Entries with already existing keys will be overwritten. In comparison to insert, this function is doing a barrel roll.
		void inject(const Map<K, V>& other)
		{
			for (iterator_map_t it = other.begin(); it != other.end(); it++)
			{
				stdmap::operator[](it->first) = it->second;
			}
		}
		/// @brief Removes a pair of key and value specified by a key.
		/// @param[in] key Key of the entry.
		void remove_key(const K& key)
		{
			if (this->has_key(key))
			{
				stdmap::erase(key);
			}
		}
		/// @brief Removes all pairs of key and value specified by an Array of keys.
		/// @param[in] keys Array of keys.
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
		/// @brief Removes a pair of key and value specified by a value.
		/// @param[in] value Value of the entry.
		void remove_value(const V& value)
		{
			if (this->has_value(value))
			{
				K result = this->key_of(value);
				stdmap::erase(result);
			}
		}
		/// @brief Removes all pairs of key and value specified by an Array of values.
		/// @param[in] values Array of values.
		void remove_values(const harray<V>& values)
		{
			for (int i = 0; i < values.size(); i++)
			{
				for (iterator_map_t it = stdmap::begin(); it != stdmap::end(); it++)
				{
					if (it->second == values.at(i))
					{
						stdmap::erase(it->first);
						break;
					}
				}
			}
		}
		/// @brief Gets a random element in Map.
		/// @param[out] value Value of selected random entry.
		/// @return Random element or NULL if Map is empty.
		K random(V* value = NULL)
		{
			if (this->size() == 0)
			{
				return NULL;
			}
			K key = this->keys()[hrand(this->size())];
			if (value != NULL)
			{
				*value = stdmap::operator[](key);
			}
			return key;
		}
		/// @brief Gets a Map of random elements selected from this one.
		/// @param[in] count Number of random elements.
		/// @return Map of random elements selected from this one.
		Map<K, V> random(int count)
		{
			if (count >= this->size())
			{
				return Map<K, V>(*this);
			}
			Map<K, V> result;
			if (count > 0)
			{
				Array<K> keys = this->keys();
				K key;
				for (int i = 0; i < count; i++)
				{
					key = keys.remove_at(hrand(keys.size()));
					result[key] = stdmap::operator[](key);
				}
			}
			return result;
		}
		/// @brief Finds and returns new Map with entries that match the condition.
		/// @param[in] condition_function Function pointer with condition function that takes a key of type K and a value of type V and returns bool.
		/// @return New Map with all matching elements.
		Map<K, V> find_all(bool (*condition_function)(K, V))
		{
			Map<K, V> result;
			for (iterator_map_t it = stdmap::begin(); it != stdmap::end(); it++)
			{
				if (condition_function(it->first, it->second))
				{
					result[it->first] = it->second;
				}
			}
			return result;
		}
		/// @brief Checks if at least one entry matches the condition.
		/// @param[in] condition_function Function pointer with condition function that takes a key of type K and a value of type V and returns bool.
		/// @return True if at least one entry matches the condition.
		bool matches_any(bool (*condition_function)(K, V))
		{
			for (iterator_map_t it = stdmap::begin(); it != stdmap::end(); it++)
			{
				if (condition_function(it->first, it->second))
				{
					return true;
				}
			}
			return false;
		}
		/// @brief Checks if all entries match the condition.
		/// @param[in] condition_function Function pointer with condition function that takes a key of type K and a value of type V and returns bool.
		/// @return True if all entries match the condition.
		bool matches_all(bool (*condition_function)(K, V))
		{
			for (iterator_map_t it = stdmap::begin(); it != stdmap::end(); it++)
			{
				if (!condition_function(it->first, it->second))
				{
					return false;
				}
			}
			return true;
		}
		/// @brief Returns a new Map with all keys and values cast into the type L and S.
		/// @return A new Map with all keys and values cast into the type L and S.
		/// @note Make sure all keys can be cast into type L and all values into type S.
		template <class L, class S>
		Map<L, S> cast()
		{
			Map<L, S> result;
			for (iterator_map_t it = stdmap::begin(); it != stdmap::end(); it++)
			{
				result[(L)it->first] = (S)it->second;
			}
			return result;
		}
		/// @brief Returns a new Map with all keys and values dynamically cast into the type L and S.
		/// @return A new Map with all keys and values cast into the type L and S.
		/// @note If dynamic casting fails, it won't be included in the result.
		template <class L, class S>
		Map<L, S> dyn_cast()
		{
			Map<L, S> result;
			L key;
			S value;
			for (iterator_map_t it = stdmap::begin(); it != stdmap::end(); it++)
			{
				key = dynamic_cast<L>(it->first);
				value = dynamic_cast<S>(it->second);
				if (key != NULL && value != NULL)
				{
					result[key] = value;
				}
			}
			return result;
		}
		/// @brief Returns a new Map with all keys and values dynamically cast into the type L and non-dynamically into S.
		/// @return A new Map with all keys and values cast into the type L and S.
		/// @note If dynamic casting fails, it won't be included in the result.
		template <class L, class S>
		Map<L, S> dyn_cast_key()
		{
			Map<L, S> result;
			L key;
			for (iterator_map_t it = stdmap::begin(); it != stdmap::end(); it++)
			{
				key = dynamic_cast<L>(it->first);
				if (key != NULL)
				{
					result[key] = (S)it->second;
				}
			}
			return result;
		}
		/// @brief Returns a new Map with all keys and values cast into the type L and dynamically into S.
		/// @return A new Map with all keys and values cast into the type L and S.
		/// @note If dynamic casting fails, it won't be included in the result.
		template <class L, class S>
		Map<L, S> dyn_cast_value()
		{
			Map<L, S> result;
			S value;
			for (iterator_map_t it = stdmap::begin(); it != stdmap::end(); it++)
			{
				value = dynamic_cast<S>(it->second);
				if (value != NULL)
				{
					result[(L)it->first] = value;
				}
			}
			return result;
		}
        /// @brief Finds and returns value stored at key. In case no value is found, returns default.
		/// @param[in] key Key which we want to retrieve.
        /// @param[in] def Default value to return if key does not exist.
		/// @return Value stored at key or given default value.
        V try_get_by_key(K key, V def)
        {
            if (this->has_key(key)) 
            {
                return operator[](key);
            }
			return def;
        }
	};
	
}

/// @brief Alias for simpler code.
#define hmap hltypes::Map

#endif