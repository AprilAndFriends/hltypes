/**
 * @file
 * @author  Boris Mikic
 * @author  Ivan Vucica
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
 *
 * @section DESCRIPTION
 *
 * Encapsulates std::vector and adds high level methods.
 */
#ifndef HLTYPES_ARRAY_H
#define HLTYPES_ARRAY_H

#include <vector>
#include <algorithm>

#include "hstring.h"

#ifdef _WIN32
	#undef min
	#undef max
#endif

/**
 * @brief Provides a simpler syntax to iterate through an Array.
 */
#define foreach(type, name, container) for (std::vector<type>::iterator name = (container).begin(); name != (container).end(); name++)
/**
 * @brief Provides a simpler syntax to reverse iterate through an Array.
 */
#define foreach_r(type, name, container) for (std::vector<type>::reverse_iterator name = (container).rbegin(); name != (container).rend(); name++)
/**
 * @brief Alias for simpler code.
 */
#define stdvector std::vector<T>

namespace hltypes
{
	/**
	 * @brief Encapsulates std::vector and adds high level methods.
	 * @author Boris Mikic
	 * @author Ivan Vucica
	 */
	template <class T> class Array : public stdvector
	{
	public:
		/**
		 * @brief Empty constructor.
		 */
		Array() : stdvector()
		{
		}
		/**
		 * @brief Copy constructor.
		 * @param[in] other Array to copy.
		 */
		Array(const Array<T>& other) : stdvector(other)
		{
		}
		/**
		 * @brief Copy constructor.
		 * @param[in] other Array to copy.
		 * @param[in] count Number of elements to copy.
		 */
		Array(const Array<T>& other, const int count) : stdvector()
		{
			this->insert_at(0, other, 0, count);
		}
		/**
		 * @brief Copy constructor.
		 * @param[in] other Array to copy.
		 * @param[in] start Start index of elements to copy.
		 * @param[in] count Number of elements to copy.
		 */
		Array(const Array<T>& other, const int start, const int count) : stdvector()
		{
			this->insert_at(0, other, start, count);
		}
		/**
		 * @brief Constructor from C-type array.
		 * @param[in] other C-type array to copy.
		 * @param[in] count Number of elements to copy.
		 */
		Array(const T other[], const int count) : stdvector()
		{
			this->insert_at(0, other, 0, count);
		}
		/**
		 * @brief Constructor from C-type array.
		 * @param[in] other C-type array to copy.
		 * @param[in] start Start index of elements to copy.
		 * @param[in] count Number of elements to copy.
		 */
		Array(const T other[], const int start, const int count) : stdvector()
		{
			this->insert_at(0, other, start, count);
		}
		/**
		 * @brief Returns element at specified position.
		 * @param[in] index Index of the element.
		 * @return Element at specified position.
		 * @note Does not work with bool as T, use Array::at directly instead.
		 */
		T& operator[](const int index)
		{
			if (index < 0)
			{
				return stdvector::at(this->size() + index);
			}
			return stdvector::at(index);
		}
		/**
		 * @brief Returns element at specified position.
		 * @param[in] index Index of the element.
		 * @return Element at specified position.
		 * @note Does not work with bool as T, use Array::at directly instead.
		 */
		const T& operator[](const int index) const
		{
			if (index < 0)
			{
				return stdvector::at(this->size() + index);
			}
			return stdvector::at(index);
		}
		/**
		 * @brief Returns a subarray.
		 * @param[in] start Start index of the elements to copy.
		 * @param[in] count Number of elements to copy.
		 * @return Subarray created from the current Array.
		 */
		Array<T> operator()(const int start, const int count) const
		{
			Array<T> result;
			result.assign(stdvector::begin() + start, stdvector::begin() + start + count);
			return result;
		}
		/**
		 * @brief Same as equals.
		 * @see equals
		 */
		bool operator==(const Array<T>& other) const
		{
			return this->equals(other);
		}
		/**
		 * @brief Same as nequals.
		 * @see nequals
		 */
		bool operator!=(const Array<T>& other) const
		{
			return this->nequals(other);
		}
		/**
		 * @brief Returns the number of elements in the Array.
		 * @return The number of elements in the Array.
		 */
		int size() const
		{
			return (int)stdvector::size();
		}
		/**
		 * @brief Compares the contents of two Arrays for being equal.
		 * @param[in] other Another Array.
		 * @return True if number of elements are equal and all pairs of elements at the same positions are equal.
		 */
		bool equals(const Array<T>& other) const
		{
			if (this->size() != other.size())
			{
				return false;
			}
			for (int i = 0; i < this->size(); i++)
			{
				// making sure operator== is used, not !=
				if (!(stdvector::at(i) == other.at(i)))
				{
					return false;
				}
			}
			return true;
		}
		/**
		 * @brief Compares the contents of two Arrays for being not equal.
		 * @param[in] other Another Array.
		 * @return True if number of elements are not equal or at least one pair of elements at the same positions is not equal.
		 */
		bool nequals(const Array<T>& other) const
		{
			if (this->size() != other.size())
			{
				return true;
			}
			for (int i = 0; i < this->size(); i++)
			{
				// making sure operator!= is used, not ==
				if (stdvector::at(i) != other.at(i))
				{
					return true;
				}
			}
			return false;
		}
		/**
		 * @brief Gets index of the given element.
		 * @param[in] element Element to search for.
		 * @return Index of the given element or -1 if element could not be found.
		 */
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
		/**
		 * @brief Checks existence of element in Array.
		 * @param[in] element Element to search for.
		 * @return True if element is in Array.
		 */
		bool contains(const T& element)
		{
			return (this->index_of(element) >= 0);
		}
		/**
		 * @brief Checks existence of elements in Array.
		 * @param[in] other Array with elements to search for.
		 * @return True if all elements are in Array.
		 */
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
		/**
		 * @brief Checks existence of elements in Array.
		 * @param[in] other C-type array with elements to search for.
		 * @param[in] count How many elements the C-type array has.
		 * @return True if all elements are in Array.
		 */
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
		/**
		 * @brief Counts occurences of element in Array.
		 * @param[in] element Element to search for.
		 * @return Number of occurences of given element.
		 */
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
		/**
		 * @brief Inserts new element at specified position n times.
		 * @param[in] index Position where to insert the new element.
		 * @param[in] element Element to insert.
		 * @param[in] times Number of times to insert element.
		 */
		void insert_at(const int index, const T& element, const int times = 1)
		{
			stdvector::insert(stdvector::begin() + index, times, element);
		}
		/**
		 * @brief Inserts all elements of another Array into this one.
		 * @param[in] index Position where to insert the new elements.
		 * @param[in] other Array of elements to insert.
		 */
		void insert_at(const int index, const Array<T>& other)
		{
			this->insert_at(index, other, 0, other.size());
		}
		/**
		 * @brief Inserts all elements of another Array into this one.
		 * @param[in] index Position where to insert the new elements.
		 * @param[in] other Array of elements to insert.
		 * @param[in] count Number of elements to insert.
		 */
		void insert_at(const int index, const Array<T>& other, const int count)
		{
			this->insert_at(index, other, 0, count);
		}
		/**
		 * @brief Inserts all elements of another Array into this one.
		 * @param[in] index Position where to insert the new elements.
		 * @param[in] other Array of elements to insert.
		 * @param[in] start Start index of the elements to insert.
		 * @param[in] count Number of elements to insert.
		 */
		void insert_at(const int index, const Array<T>& other, const int start, const int count)
		{
			stdvector::insert(stdvector::begin() + index, other.begin() + start,
				other.begin() + start + count);
		}
		/**
		 * @brief Inserts all elements of a C-type array into this Array.
		 * @param[in] index Position where to insert the new elements.
		 * @param[in] other C-type array of elements to insert.
		 * @param[in] count Number of elements to insert.
		 */
		void insert_at(const int index, const T other[], const int count)
		{
			this->insert_at(index, other, 0, count);
		}
		/**
		 * @brief Inserts all elements of a C-type array into this Array.
		 * @param[in] index Position where to insert the new elements.
		 * @param[in] other C-type array of elements to insert.
		 * @param[in] start Start index of the elements to insert.
		 * @param[in] count Number of elements to insert.
		 */
		void insert_at(const int index, const T other[], const int start, const int count)
		{
			stdvector::insert(stdvector::begin() + index, other + start, other + start + count);
		}
		/**
		 * @brief Removes element at given index.
		 * @param[in] index Index of element to remove.
		 * @return The removed element.
		 */
		T remove_at(const int index)
		{
			T result = stdvector::at(index);
			stdvector::erase(stdvector::begin() + index);
			return result;
		}
		/**
		 * @brief Removes n elements at given index of Array.
		 * @param[in] index Start index of elements to remove.
		 * @param[in] count Number of elements to remove.
		 * @return Array of all removed elements.
		 * @note Elements in the returned Array are in the same order as in the orignal Array.
		 */
		Array<T> remove_at(const int index, const int count)
		{
			Array<T> result;
			result.assign(stdvector::begin() + index, stdvector::begin() + index + count);
			stdvector::erase(stdvector::begin() + index, stdvector::begin() + index + count);
			return result;
		}
		/**
		 * @brief Removes first occurence of element in Array.
		 * @param[in] element Element to remove.
		 */
		void remove(T element)
		{
			stdvector::erase(stdvector::begin() + this->index_of(element));
		}
		/**
		 * @brief Removes first occurence of each element in another Array from this one.
		 * @param[in] other Array of elements to remove.
		 */
		void remove(const Array<T>& other)
		{
			for (int i = 0; i < other.size(); i++)
			{
				stdvector::erase(stdvector::begin() + this->index_of(other.at(i)));
			}
		}
		/**
		 * @brief Removes all occurences of element in Array.
		 * @param[in] element Element to remove.
		 */
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
		/**
		 * @brief Removes all occurences of each element in another Array from this one.
		 * @param[in] other Array of elements to remove.
		 */
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
		/**
		 * @brief Adds element at the end of Array.
		 * @param[in] element Element to add.
		 */
		void push_back(const T& element)
		{
			stdvector::push_back(element);
		}
		/**
		 * @brief Adds element at the end of Array n times.
		 * @param[in] element Element to add.
		 * @param[in] times Number of times to add the element.
		 */
		void push_back(const T& element, int times)
		{
			for (int i = 0; i < times; i++)
			{
				stdvector::push_back(element);
			}
		}
		/**
		 * @brief Adds all elements from another Array at the end of this one.
		 * @param[in] other Array of elements to add.
		 */
		void push_back(const Array<T>& other)
		{
			for (int i = 0; i < other.size(); i++)
			{
				stdvector::push_back(other.at(i));
			}
		}
		/**
		 * @brief Adds all elements from another Array at the end of this one.
		 * @param[in] other Array of elements to add.
		 * @param[in] count Number of elements to add.
		 */
		void push_back(const Array<T>& other, const int count)
		{
			for (int i = 0; i < count; i++)
			{
				stdvector::push_back(other.at(i));
			}
		}
		/**
		 * @brief Adds all elements from another Array at the end of this one.
		 * @param[in] other Array of elements to add.
		 * @param[in] start Start index of the elements to add.
		 * @param[in] count Number of elements to add.
		 */
		void push_back(const Array<T>& other, const int start, const int count)
		{
			for (int i = 0; i < count; i++)
			{
				stdvector::push_back(other.at(start + i));
			}
		}
		/**
		 * @brief Adds all elements from a C-type array at the end of Array.
		 * @param[in] other C-type array of elements to add.
		 * @param[in] count Number of elements to add.
		 */
		void push_back(const T other[], const int count)
		{
			for (int i = 0; i < count; i++)
			{
				stdvector::push_back(other[i]);
			}
		}
		/**
		 * @brief Adds all elements from a C-type array at the end of Array.
		 * @param[in] other C-type array of elements to add.
		 * @param[in] start Start index of the elements to add.
		 * @param[in] count Number of elements to add.
		 */
		void push_back(const T other[], const int start, const int count)
		{
			for (int i = 0; i < count; i++)
			{
				stdvector::push_back(other[start + i]);
			}
		}
		/**
		 * @brief Adds element at the beginning of Array n times.
		 * @param[in] element Element to add.
		 * @param[in] times Number of times to add the element.
		 */
		void push_front(const T& element, int times = 1)
		{
			this->insert_at(0, element, times);
		}
		/**
		 * @brief Adds all elements from another Array at the beginning of this one.
		 * @param[in] other Array of elements to add.
		 */
		void push_front(const Array<T>& other)
		{
			this->insert_at(0, other);
		}
		/**
		 * @brief Adds all elements from another Array at the beginning of this one.
		 * @param[in] other Array of elements to add.
		 * @param[in] count Number of elements to add.
		 */
		void push_front(const Array<T>& other, const int count)
		{
			this->insert_at(0, other, 0, count);
		}
		/**
		 * @brief Adds all elements from another Array at the beginning of this one.
		 * @param[in] other Array of elements to add.
		 * @param[in] start Start index of the elements to add.
		 * @param[in] count Number of elements to add.
		 */
		void push_front(const Array<T>& other, const int start, const int count)
		{
			this->insert_at(0, other, start, count);
		}
		/**
		 * @brief Adds all elements from a C-type array at the beginning of Array.
		 * @param[in] other C-type array of elements to add.
		 * @param[in] count Number of elements to add.
		 */
		void push_front(const T other[], const int count)
		{
			this->insert_at(0, other, 0, count);
		}
		/**
		 * @brief Adds all elements from a C-type array at the beginning of Array.
		 * @param[in] other C-type array of elements to add.
		 * @param[in] start Start index of the elements to add.
		 * @param[in] count Number of elements to add.
		 */
		void push_front(const T other[], const int start, const int count)
		{
			this->insert_at(0, other, start, count);
		}
		/**
		 * @brief Removes first element of Array.
		 * @return The removed element.
		 */
		T pop_front()
		{
			return this->remove_at(0);
		}
		/**
		 * @brief Removes n elements from the beginning of Array.
		 * @param[in] count Number of elements to remove.
		 * @return Array of all removed elements.
		 * @note Elements in the returned Array are in the same order as in the orignal Array.
		 */
		Array<T> pop_front(const int count)
		{
			Array<T> result;
			result.assign(stdvector::begin(), stdvector::begin() + count);
			stdvector::erase(stdvector::begin(), stdvector::begin() + count);
			return result;
		}
		/**
		 * @brief Removes last element of Array.
		 * @return The removed element.
		 */
		T pop_back()
		{
			T element = stdvector::back();
			stdvector::pop_back();
			return element;
		}
		/**
		 * @brief Removes n elements from the end of Array.
		 * @param[in] count Number of elements to remove.
		 * @return Array of all removed elements.
		 * @note Elements in the returned Array are in the same order as in the orignal Array.
		 */
		Array<T> pop_back(const int count)
		{
			Array<T> result;
			result.assign(stdvector::end() - count, stdvector::end());
			stdvector::erase(stdvector::end() - count, stdvector::end());
			return result;
		}
		/**
		 * @brief Same as remove_at.
		 * @see remove_at(const int index)
		 */
		T pop(const int index)
		{
			return this->remove_at(index);
		}
		/**
		 * @brief Same as remove_at.
		 * @see remove_at(const int index, const int count)
		 */
		Array<T> pop(const int index, const int count)
		{
			return this->remove_at(index, count);
		}
		/**
		 * @brief Same as remove_all.
		 * @see remove_all(T& element)
		 */
		void pop_all(T& element)
		{
			this->remove_all(element);
		}
		/**
		 * @brief Same as remove_all.
		 * @see remove_all(const Array<T>& other)
		 */
		void pop_all(const Array<T>& other)
		{
			this->remove_all(other);
		}
		/**
		 * @brief Unites elements of this Array with another one.
		 * @param[in] other Array to unite with.
		 */
		void unite(const Array<T>& other)
		{
			this->insert_at(this->size(), other, 0, other.size());
			this->remove_duplicates();
		}
		/**
		 * @brief Creates a new Array as union of this Array with another one.
		 * @param[in] other Array to unite with.
		 * @return A new Array.
		 */
		Array<T> united(const Array<T>& other) const
		{
			Array<T> result(*this);
			result.unite(other);
			return result;
		}
		/**
		 * @brief Intersects elements of this Array with another one.
		 * @param[in] other Array to intersect with.
		 */
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
		/**
		 * @brief Creates a new Array as intersection of this Array with another one.
		 * @param[in] other Array to intersect with.
		 * @return A new Array.
		 */
		Array<T> intersected(const Array<T>& other) const
		{
			Array<T> result(*this);
			result.intersect(other);
			return result;
		}
		/**
		 * @brief Differentiates elements of this Array with another one.
		 * @param[in] other Array to differentiate with.
		 * @note Unlike remove, this method ignore elements of other Array that are not in this one.
		 */
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
		/**
		 * @brief Creates a new Array as difference of this Array with another one.
		 * @param[in] other Array to differentiate with.
		 * @return A new Array.
		 * @note Unlike remove, this method ignore elements of other Array that are not in this one.
		 */
		Array<T> differentiated(const Array<T>& other) const
		{
			Array<T> result(*this);
			result.differentiate(other);
			return result;
		}
		/**
		 * @brief Reverses order of elements.
		 */
		void reverse()
		{
			std::reverse(stdvector::begin(), stdvector::end());
		}
		/**
		 * @brief Creates new Array with reversed order of elements.
		 * @return A new Array.
		 */
		Array<T> reversed() const
		{
			Array<T> result(*this);
			result.reverse();
			return result;
		}
		/**
		 * @brief Removes duplicates in Array.
		 */
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
		/**
		 * @brief Creates new Array without duplicates.
		 * @return A new Array.
		 */
		Array<T> removed_duplicates() const
		{
			Array<T> result(*this);
			result.remove_duplicates();
			return result;
		}
		/**
		 * @brief Sorts elements in Array.
		 * @note The sorting order is ascending.
		 */
		void sort()
		{
			if (this->size() > 0)
			{
				std::stable_sort(stdvector::begin(), stdvector::end());
			}
		}
		/**
		 * @brief Sorts elements in Array.
		 * @param[in] compare_function Function pointer with comparison function that takes two elements of type T and returns bool.
		 * @note The sorting order is ascending.
		 * @note compare_function should return true if first element is less than the second element.
		 */
		void sort(bool (*compare_function)(T, T))
		{
			if (this->size() > 0)
			{
				std::stable_sort(stdvector::begin(), stdvector::end(), compare_function);
			}
		}
		/**
		 * @brief Creates new sorted Array.
		 * @return A new Array.
		 * @note The sorting order is ascending.
		 */
		Array<T> sorted() const
		{
			Array<T> result(*this);
			result.sort();
			return result;
		}
		/**
		 * @brief Creates new sorted Array.
		 * @param[in] compare_function Function pointer with comparison function that takes two elements of type T and returns bool.
		 * @return A new Array.
		 * @note The sorting order is ascending.
		 * @note compare_function should return true if first element is less than the second element.
		 */
		Array<T> sorted(bool (*compare_function)(T, T)) const
		{
			Array<T> result(*this);
			result.sort(compare_function);
			return result;
		}
		/**
		 * @brief Randomizes order of elements in Array.
		 */
		void randomize()
		{
			std::random_shuffle(stdvector::begin(), stdvector::end());
		}
		/**
		 * @brief Creates a new Array with randomized order of elements.
		 * @return A new Array.
		 */
		Array<T> randomized() const
		{
			Array<T> result(*this);
			result.randomize();
			return result;
		}
		/**
		 * @brief Finds minimum element in Array.
		 * @return Minimum Element or NULL if Array is empty.
		 */
		T min()
		{
			if (this->size() == 0)
			{
				return NULL;
			}
			return (*std::min_element(stdvector::begin(), stdvector::end()));
		}
		/**
		 * @brief Finds minimum element in Array.
		 * @param[in] compare_function Function pointer with comparison function that takes two elements of type T and returns bool.
		 * @return Minimum Element or NULL if Array is empty.
		 * @note compare_function should return true if first element is less than second element.
		 */
		T min(bool (*compare_function)(T, T))
		{
			if (this->size() == 0)
			{
				return NULL;
			}
			return (*std::min_element(stdvector::begin(), stdvector::end(), compare_function));
		}
		/**
		 * @brief Finds maximum element in Array.
		 * @return Maximum Element or NULL if Array is empty.
		 */
		T max()
		{
			if (this->size() == 0)
			{
				return NULL;
			}
			return (*std::max_element(stdvector::begin(), stdvector::end()));
		}
		/**
		 * @brief Finds maximum element in Array.
		 * @param[in] compare_function Function pointer with comparison function that takes two elements of type T and returns bool.
		 * @return Maximum Element or NULL if Array is empty.
		 * @note compare_function should return true if first element is greater than second element.
		 */
		T max(bool (*compare_function)(T, T))
		{
			if (this->size() == 0)
			{
				return NULL;
			}
			return (*std::max_element(stdvector::begin(), stdvector::end(), compare_function));
		}
		/**
		 * @brief Joins all elements into a string.
		 * @param[in] separator Separator string between elements.
		 * @return String or joined elements separater by separator string.
		 * @note Make sure your elements can be cast into String or are already String.
		 */
		hstr join(chstr separator)
		{
			hstr result;
			if (this->size() > 0)
			{
				result += hstr(stdvector::at(0));
				for (int i = 1; i < this->size(); i++)
				{
					result += separator + hstr(stdvector::at(i));
				}
			}
			return result;
		}
		/**
		 * @brief Finds and returns new Array of elements that match the condition.
		 * @param[in] condition_function Function pointer with condition function that takes one element of type T and returns bool.
		 * @return New Array with all matching elements.
		 */
		Array<T> find_all(bool (*condition_function)(T))
		{
			Array<T> result;
			for (int i = 0; i < this->size(); i++)
			{
				if (condition_function(stdvector::at(i)))
				{
					result += stdvector::at(i);
				}
			}
			return result;
		}
		/**
		 * @brief Finds and returns first occurence of element that matches the condition.
		 * @param[in] condition_function Function pointer with condition function that takes one element of type T and returns bool.
		 * @return Pointer to element that matches the condition or NULL if no element was found.
		 */
		T* find_first(bool (*condition_function)(T))
		{
			for (int i = 0; i < this->size(); i++)
			{
				if (condition_function(stdvector::at(i)))
				{
					return &stdvector::at(i);
				}
			}
			return NULL;
		}
		/**
		 * @brief Checks if at least one element matches the condition.
		 * @param[in] condition_function Function pointer with condition function that takes one element of type T and returns bool.
		 * @return True if at least one element matches the condition.
		 */
		bool matches_any(bool (*condition_function)(T))
		{
			for (int i = 0; i < this->size(); i++)
			{
				if (condition_function(stdvector::at(i)))
				{
					return true;
				}
			}
			return false;
		}
		/**
		 * @brief Checks if all elements match the condition.
		 * @param[in] condition_function Function pointer with condition function that takes one element of type T and returns bool.
		 * @return True if all elements match the condition.
		 */
		bool matches_all(bool (*condition_function)(T))
		{
			for (int i = 0; i < this->size(); i++)
			{
				if (!condition_function(stdvector::at(i)))
				{
					return false;
				}
			}
			return true;
		}
		/**
		 * @brief Returns a new Array with all elements cast into type S.
		 * @return A new Array with all elements cast into type S.
		 * @note Make sure all elements in the Array can be cast into type S.
		 */
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
		/**
		 * @brief Same as contains.
		 * @see contains(const T& element)
		 */
		bool includes(const T& element)
		{
			return this->contains(element);
		}
		/**
		 * @brief Same as contains.
		 * @see contains(const Array<T>& other)
		 */
		bool includes(const Array<T>& other)
		{
			return this->contains(other);
		}
		/**
		 * @brief Same as contains.
		 * @see contains(const T other[], int count)
		 */
		bool includes(const T other[], int count)
		{
			return this->contains(other, count);
		}
		/**
		 * @brief Same as contains.
		 * @see contains(const T& element)
		 */
		bool has(const T& element)
		{
			return this->contains(element);
		}
		/**
		 * @brief Same as contains.
		 * @see contains(const Array<T>& other)
		 */
		bool has(const Array<T>& other)
		{
			return this->contains(other);
		}
		/**
		 * @brief Same as contains.
		 * @see contains(const T other[], int count)
		 */
		bool has(const T other[], int count)
		{
			return this->contains(other, count);
		}
		/**
		 * @brief Same as contains.
		 * @see contains(const T& element)
		 */
		bool has_element(const T& element)
		{
			return this->contains(element);
		}
		/**
		 * @brief Same as contains.
		 * @see contains(const Array<T>& other)
		 */
		bool has_element(const Array<T>& other)
		{
			return this->contains(other);
		}
		/**
		 * @brief Same as contains.
		 * @see contains(const T other[], int count)
		 */
		bool has_element(const T other[], int count)
		{
			return this->contains(other, count);
		}
		/**
		 * @brief Same as push_back.
		 * @see push_back(const T& element)
		 */
		void add(const T& element)
		{
			this->push_back(element);
		}
		/**
		 * @brief Same as push_back.
		 * @see push_back(const T& element, int times)
		 */
		void add(const T& element, int times)
		{
			this->push_back(element, times);
		}
		/**
		 * @brief Same as push_back.
		 * @see push_back(const Array<T>& other)
		 */
		void add(const Array<T>& other)
		{
			this->push_back(other);
		}
		/**
		 * @brief Same as push_back.
		 * @see push_back(const Array<T>& other, const int count)
		 */
		void add(const Array<T>& other, const int count)
		{
			this->push_back(other, count);
		}
		/**
		 * @brief Same as push_back.
		 * @see push_back(const Array<T>& other, const int start, const int count)
		 */
		void add(const Array<T>& other, const int start, const int count)
		{
			this->push_back(other, start, count);
		}
		/**
		 * @brief Same as push_back.
		 * @see push_back(const T other[], const int count)
		 */
		void add(const T other[], const int count)
		{
			this->push_back(other, count);
		}
		/**
		 * @brief Same as push_back.
		 * @see push_back(const T other[], const int start, const int count)
		 */
		void add(const T other[], const int start, const int count)
		{
			this->push_back(other, start, count);
		}
		/**
		 * @brief Same as push_back.
		 * @see push_back(const T& element)
		 */
		void append(const T& element)
		{
			this->push_back(element);
		}
		/**
		 * @brief Same as push_back.
		 * @see push_back(const T& element, int times)
		 */
		void append(const T& element, int times)
		{
			this->push_back(element, times);
		}
		/**
		 * @brief Same as push_back.
		 * @see push_back(const Array<T>& other)
		 */
		void append(const Array<T>& other)
		{
			this->push_back(other);
		}
		/**
		 * @brief Same as push_back.
		 * @see push_back(const Array<T>& other, const int count)
		 */
		void append(const Array<T>& other, const int count)
		{
			this->push_back(other, count);
		}
		/**
		 * @brief Same as push_back.
		 * @see push_back(const Array<T>& other, const int start, const int count)
		 */
		void append(const Array<T>& other, const int start, const int count)
		{
			this->push_back(other, start, count);
		}
		/**
		 * @brief Same as push_back.
		 * @see push_back(const T other[], const int count)
		 */
		void append(const T other[], const int count)
		{
			this->push_back(other, count);
		}
		/**
		 * @brief Same as push_back.
		 * @see push_back(const T other[], const int start, const int count)
		 */
		void append(const T other[], const int start, const int count)
		{
			this->push_back(other, start, count);
		}
		/**
		 * @brief Same as push_back.
		 * @see push_back(const T& element)
		 */
		Array<T>& operator<<(const T& element)
		{
			this->push_back(element);
			return *this;
		}
		/**
		 * @brief Same as push_back.
		 * @see push_back(const Array<T>& other)
		 */
		Array<T>& operator<<(const Array<T>& other)
		{
			this->push_back(other);
			return *this;
		}
		/**
		 * @brief Same as push_back.
		 * @see push_back(const T& element)
		 */
		Array<T>& operator+=(const T& element)
		{
			this->push_back(element);
			return *this;
		}
		/**
		 * @brief Same as push_back.
		 * @see push_back(const Array<T>& other)
		 */
		Array<T>& operator+=(const Array<T>& other)
		{
			this->push_back(other);
			return *this;
		}
		/**
		 * @brief Same as remove.
		 * @see remove(T element)
		 */
		Array<T>& operator-=(T element)
		{
			this->remove(element);
			return *this;
		}
		/**
		 * @brief Same as remove.
		 * @see remove(const Array<T>& other)
		 */
		Array<T>& operator-=(const Array<T>& other)
		{
			this->remove(other);
			return *this;
		}
		/**
		 * @brief Same as unite.
		 * @see unite
		 */
		Array<T>& operator|=(const Array<T>& other)
		{
			this->unite(other);
			return *this;
		}
		/**
		 * @brief Same as intersect.
		 * @see intersect
		 */
		Array<T>& operator&=(const Array<T>& other)
		{
			this->intersect(other);
			return *this;
		}
		/**
		 * @brief Same as differentiate.
		 * @see differentiate
		 */
		Array<T>& operator/=(const Array<T>& other)
		{
			this->differentiate(other);
			return *this;
		}
		/**
		 * @brief Merges two Arrays.
		 * @param[in] other Second Array to merge with.
		 * @return New Array with elements of second Array added at the end of first Array.
		 */
		Array<T> operator+(const Array<T>& other) const
		{
			Array<T> result(*this);
			result += other;
			return result;
		}
		/**
		 * @brief Merges an Array with an element.
		 * @param[in] element Element to merge with.
		 * @return New Array with element added at the end of Array.
		 */
		Array<T> operator+(const T& element) const
		{
			Array<T> result(*this);
			result += element;
			return result;
		}
		/**
		 * @brief Removes second Array from first Array.
		 * @param[in] other Array to remove.
		 * @return New Array with elements of first Array without the elements of second Array.
		 */
		Array<T> operator-(const Array<T>& other) const
		{
			Array<T> result(*this);
			result -= other;
			return result;
		}
		/**
		 * @brief Removes element from Array.
		 * @param[in] element Element to remove.
		 * @return New Array with elements of first Array without given element.
		 */
		Array<T> operator-(const T& element) const
		{
			Array<T> result(*this);
			result -= element;
			return result;
		}
		/**
		 * @brief Same as united.
		 * @see united
		 */
		Array<T> operator|(const Array<T>& other) const
		{
			return this->united(other);
		}
		/**
		 * @brief Same as intersected.
		 * @see intersected
		 */
		Array<T> operator&(const Array<T>& other) const
		{
			return this->intersected(other);
		}
		/**
		 * @brief Same as differentiated.
		 * @see differentiated
		 */
		Array<T> operator/(const Array<T>& other) const
		{
			return this->differentiated(other);
		}
		
	};
	
}

/**
 * @brief Alias for simpler code.
 */
#define harray hltypes::Array

#endif
