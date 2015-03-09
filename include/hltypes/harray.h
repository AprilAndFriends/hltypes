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
/// Encapsulates std::vector and adds high level methods.

#ifndef HLTYPES_ARRAY_H
#define HLTYPES_ARRAY_H

#include <algorithm>
#include <vector>

#include "hcontainer.h"
#include "hexception.h"
#include "hltypesUtil.h"
#include "hplatform.h"
#include "hstring.h"

/// @brief Provides a simpler syntax to iterate through an Array.
#define foreach(type, name, container) for (std::vector< type >::iterator name = (container).begin(); name != (container).end(); ++name)
#define foreachc(type, name, container) for (std::vector< type >::const_iterator name = (container).begin(); name != (container).end(); ++name)
/// @brief Provides a simpler syntax to reverse iterate through an Array.
#define foreach_r(type, name, container) for (std::vector< type >::reverse_iterator name = (container).rbegin(); name != (container).rend(); ++name)
#define foreachc_r(type, name, container) for (std::vector< type >::reverse_const_iterator name = (container).rbegin(); name != (container).rend(); ++name)
/// @brief Alias for simpler code.
#define stdvector std::vector<T>

namespace hltypes
{
	/// @brief Encapsulates std::vector and adds high level methods.
	template <class T>
	class Array : public Container<stdvector, T>
	{
	private:
		typedef typename stdvector::iterator iterator_t;
		typedef typename stdvector::const_iterator const_iterator_t;
	public:
		/// @brief Empty constructor.
		inline Array() : Container()
		{
		}
		/// @brief Copy constructor.
		/// @param[in] other Container to copy.
		inline Array(const Container& other) : Container(other)
		{
		}
		/// @brief Constructor from single element.
		/// @param[in] element Element to insert.
		inline Array(const T& element) : Container(element)
		{
		}
		/// @brief Constructor from single element.
		/// @param[in] element Element to insert.
		/// @param[in] times Number of times to insert element.
		inline Array(const T& element, int times) : Container(element, times)
		{
		}
		/// @brief Constructor from another Container.
		/// @param[in] other Container to copy.
		/// @param[in] count Number of elements to copy.
		inline Array(const Container& other, const int count) : Container(other, count)
		{
		}
		/// @brief Constructor from another Container.
		/// @param[in] other Container to copy.
		/// @param[in] start Start index of elements to copy.
		/// @param[in] count Number of elements to copy.
		inline Array(const Container& other, const int start, const int count) : Container(other, start, count)
		{
		}
		/// @brief Constructor from C-type array.
		/// @param[in] other C-type array to copy.
		/// @param[in] count Number of elements to copy.
		inline Array(const T other[], const int count) : Container(other, count)
		{
		}
		/// @brief Constructor from C-type array.
		/// @param[in] other C-type array to copy.
		/// @param[in] start Start index of elements to copy.
		/// @param[in] count Number of elements to copy.
		inline Array(const T other[], const int start, const int count) : Container(other, start, count)
		{
		}
		/// @brief Destructor.
		inline ~Array()
		{
		}
		/// @brief Gets all indexes of the given element.
		/// @param[in] element Element to search for.
		/// @return All indexes of the given element.
		inline Array<int> indexes_of(T element) const
		{
			return this->_indexesOf<Array<int> >(element);
		}
		/// @brief Removes element at given index.
		/// @param[in] index Index of element to remove.
		/// @return The removed element.
		inline T removeAt(int index)
		{
			return Container::removeAt(index);
		}
		/// @brief Removes n elements at given index of Array.
		/// @param[in] index Start index of elements to remove.
		/// @param[in] count Number of elements to remove.
		/// @return Array of all removed elements.
		/// @note Elements in the returned Array are in the same order as in the orignal Array.
		inline Array<T> removeAt(int index, int count)
		{
			return this->_removeAt<Array<T> >(index, count);
		}
		/// @brief Removes first element of Array.
		/// @return The removed element.
		inline T removeFirst()
		{
			return Container::removeFirst();
		}
		/// @brief Removes n elements from the beginning of Array.
		/// @param[in] count Number of elements to remove.
		/// @return Array of all removed elements.
		/// @note Elements in the returned Array are in the same order as in the orignal Array.
		inline Array<T> removeFirst(const int count)
		{
			return this->_removeFirst<Array<T> >(count);
		}
		/// @brief Removes last element of Array.
		/// @return The removed element.
		inline T removeLast()
		{
			return Container::removeLast();
		}
		/// @brief Removes n elements from the end of Array.
		/// @param[in] count Number of elements to remove.
		/// @return Array of all removed elements.
		/// @note Elements in the returned Array are in the same order as in the orignal Array.
		inline Array<T> removeLast(const int count)
		{
			return this->_removeLast<Array<T> >(count);
		}
		/// @brief Creates new Array with reversed order of elements.
		/// @return A new Array.
		inline Array<T> reversed() const
		{
			return this->_reversed<Array<T> >();
		}
		/// @brief Creates new Array without duplicates.
		/// @return A new Array.
		inline Array<T> removed_duplicates() const
		{
			return this->_removedDuplicates<Array<T> >();
		}
		/// @brief Creates a new Array as union of this Array with an element.
		/// @param[in] element Element to unite with.
		/// @return A new Array.
		inline Array<T> united(const T& element) const
		{
			return this->_united<Array<T> >(element);
		}
		/// @brief Creates a new Array as union of this Array with another one.
		/// @param[in] other Array to unite with.
		/// @return A new Array.
		inline Array<T> united(const Array<T>& other) const
		{
			return this->_united<Array<T> >(other);
		}
		/// @brief Creates a new Array as intersection of this Array with another one.
		/// @param[in] other Array to intersect with.
		/// @return A new Array.
		inline Array<T> intersected(const Array<T>& other) const
		{
			return this->_intersected<Array<T> >(other);
		}
		/// @brief Creates a new Array as difference of this Array with an element.
		/// @param[in] other Element to differentiate with.
		/// @return A new Array.
		/// @note Unlike remove, this method ignores if the element is not in this Array.
		inline Array<T> differentiated(const T& element) const
		{
			return this->_differentiated<Array<T> >(element);
		}
		/// @brief Creates a new Array as difference of this Array with another one.
		/// @param[in] other Array to differentiate with.
		/// @return A new Array.
		/// @note Unlike remove, this method ignore elements of other Array that are not in this one.
		inline Array<T> differentiated(const Array<T>& other) const
		{
			return this->_differentiated<Array<T> >(other);
		}
		/// @brief Returns element at specified position.
		/// @param[in] index Index of the element.
		/// @return Element at specified position.
		/// @note Does not work with bool as T, use Array::at directly instead.
		inline T& operator[](int index)
		{
			return this->at(index);
		}
		/// @brief Returns element at specified position.
		/// @param[in] index Index of the element.
		/// @return Element at specified position.
		/// @note Does not work with bool as T, use Array::at directly instead.
		inline const T& operator[](int index) const
		{
			return this->at(index);
		}
		/// @brief Returns a subarray.
		/// @param[in] start Start index of the elements to copy.
		/// @param[in] count Number of elements to copy.
		/// @return Subarray created from the current Array.
		inline Array<T> operator()(int start, const int count) const
		{
			return this->_sub<Array<T> >(start, count);
		}
		/// @brief Same as equals.
		/// @see equals
		inline bool operator==(const Array<T>& other) const
		{
			return this->equals(other);
		}
		/// @brief Same as nequals.
		/// @see nequals
		inline bool operator!=(const Array<T>& other) const
		{
			return this->nequals(other);
		}
		/// @brief Same as push_back.
		/// @see push_back(const T& element)
		inline Array<T>& operator<<(const T& element)
		{
			this->push_back(element);
			return (*this);
		}
		/// @brief Same as push_back.
		/// @see push_back(const Array<T>& other)
		inline Array<T>& operator<<(const Array<T>& other)
		{
			this->push_back(other);
			return (*this);
		}
		/// @brief Same as push_back.
		/// @see push_back(const T& element)
		inline Array<T>& operator+=(const T& element)
		{
			this->push_back(element);
			return (*this);
		}
		/// @brief Same as push_back.
		/// @see push_back(const Array<T>& other)
		inline Array<T>& operator+=(const Array<T>& other)
		{
			this->push_back(other);
			return (*this);
		}
		/// @brief Same as remove.
		/// @see remove(T element)
		inline Array<T>& operator-=(T element)
		{
			this->remove(element);
			return (*this);
		}
		/// @brief Same as remove.
		/// @see remove(const Array<T>& other)
		inline Array<T>& operator-=(const Array<T>& other)
		{
			this->remove(other);
			return (*this);
		}
		/// @brief Same as unite.
		/// @see unite(const T& element)
		inline Array<T>& operator|=(const T& element)
		{
			this->unite(element);
			return (*this);
		}
		/// @brief Same as unite.
		/// @see unite(const Array<T>& other)
		inline Array<T>& operator|=(const Array<T>& other)
		{
			this->unite(other);
			return (*this);
		}
		/// @brief Same as intersect.
		/// @see intersect(const Array<T>& other)
		inline Array<T>& operator&=(const Array<T>& other)
		{
			this->intersect(other);
			return (*this);
		}
		/// @brief Same as differentiate.
		/// @see differentiate(const T& element)
		inline Array<T>& operator/=(const T& element)
		{
			this->differentiate(element);
			return (*this);
		}
		/// @brief Same as differentiate.
		/// @see differentiate(const Array<T>& other)
		inline Array<T>& operator/=(const Array<T>& other)
		{
			this->differentiate(other);
			return (*this);
		}
		/// @brief Merges an Array with an element.
		/// @param[in] element Element to merge with.
		/// @return New Array with element added at the end of Array.
		inline Array<T> operator+(const T& element) const
		{
			Array<T> result(*this);
			result += element;
			return result;
		}
		/// @brief Merges two Arrays.
		/// @param[in] other Second Array to merge with.
		/// @return New Array with elements of second Array added at the end of first Array.
		inline Array<T> operator+(const Array<T>& other) const
		{
			Array<T> result(*this);
			result += other;
			return result;
		}
		/// @brief Removes element from Array.
		/// @param[in] element Element to remove.
		/// @return New Array with elements of first Array without given element.
		inline Array<T> operator-(T element) const
		{
			Array<T> result(*this);
			result -= element;
			return result;
		}
		/// @brief Removes second Array from first Array.
		/// @param[in] other Array to remove.
		/// @return New Array with elements of first Array without the elements of second Array.
		inline Array<T> operator-(const Array<T>& other) const
		{
			Array<T> result(*this);
			result -= other;
			return result;
		}
		/// @brief Same as united.
		/// @see united(const T& element)
		inline Array<T> operator|(const T& element) const
		{
			return this->united(element);
		}
		/// @brief Same as united.
		/// @see united(const Array<T>& other)
		inline Array<T> operator|(const Array<T>& other) const
		{
			return this->united(other);
		}
		/// @brief Same as intersected.
		/// @see intersected(const Array<T>& other)
		inline Array<T> operator&(const Array<T>& other) const
		{
			return this->intersected(other);
		}
		/// @brief Same as differentiated.
		/// @see differentiated(const T& element)
		inline Array<T> operator/(const T& element) const
		{
			return this->differentiated(element);
		}
		/// @brief Same as differentiated.
		/// @see differentiated(const Array<T>& other)
		inline Array<T> operator/(const Array<T>& other) const
		{
			return this->differentiated(other);
		}
		/// @brief Sorts elements in Array.
		/// @note The sorting order is ascending.
		inline void sort()
		{
			if (this->size() > 0)
			{
				std::stable_sort(stdvector::begin(), stdvector::end());
			}
		}
		/// @brief Sorts elements in Array.
		/// @param[in] compare_function Function pointer with comparison function that takes two elements of type T and returns bool.
		/// @note The sorting order is ascending.
		/// @note compare_function should return true if first element is less than the second element.
		inline void sort(bool (*compare_function)(T, T))
		{
			if (this->size() > 0)
			{
				std::stable_sort(stdvector::begin(), stdvector::end(), compare_function);
			}
		}
		/// @brief Creates new sorted Array.
		/// @return A new Array.
		/// @note The sorting order is ascending.
		inline Array<T> sorted() const
		{
			Array<T> result(*this);
			result.sort();
			return result;
		}
		/// @brief Creates new sorted Array.
		/// @param[in] compare_function Function pointer with comparison function that takes two elements of type T and returns bool.
		/// @return A new Array.
		/// @note The sorting order is ascending.
		/// @note compare_function should return true if first element is less than the second element.
		inline Array<T> sorted(bool (*compare_function)(T, T)) const
		{
			Array<T> result(*this);
			result.sort(compare_function);
			return result;
		}
		/// @brief Randomizes order of elements in Array.
		inline void randomize()
		{
			std::random_shuffle(stdvector::begin(), stdvector::end());
		}
		/// @brief Creates a new Array with randomized order of elements.
		/// @return A new Array.
		inline Array<T> randomized() const
		{
			Array<T> result(*this);
			result.randomize();
			return result;
		}
		/// @brief Finds minimum element in Array.
		/// @return Minimum Element.
		inline T min() const
		{
			if (this->size() == 0)
			{
				throw ContainerEmptyException("min()");
			}
			return (*std::min_element(stdvector::begin(), stdvector::end()));
		}
		/// @brief Finds minimum element in Array.
		/// @param[in] compare_function Function pointer with comparison function that takes two elements of type T and returns bool.
		/// @return Minimum Element.
		/// @note compare_function should return true if first element is less than second element.
		inline T min(bool (*compare_function)(T, T)) const
		{
			if (this->size() == 0)
			{
				throw ContainerEmptyException("min()");
			}
			return (*std::min_element(stdvector::begin(), stdvector::end(), compare_function));
		}
		/// @brief Finds maximum element in Array.
		/// @return Maximum Element.
		inline T max() const
		{
			if (this->size() == 0)
			{
				throw ContainerEmptyException("max()");
			}
			return (*std::max_element(stdvector::begin(), stdvector::end()));
		}
		/// @brief Finds maximum element in Array.
		/// @param[in] compare_function Function pointer with comparison function that takes two elements of type T and returns bool.
		/// @return Maximum Element.
		/// @note compare_function should return true if first element is greater than second element.
		inline T max(bool (*compare_function)(T, T)) const
		{
			if (this->size() == 0)
			{
				throw ContainerEmptyException("max()");
			}
			return (*std::max_element(stdvector::begin(), stdvector::end(), compare_function));
		}
		/// @brief Gets a random element in Array.
		/// @return Random element.
		inline T random() const
		{
			if (this->size() == 0)
			{
				throw ContainerEmptyException("random()");
			}
			return stdvector::at(hrand(this->size()));
		}
		/// @brief Gets an Array of random elements selected from this one.
		/// @param[in] count Number of random elements.
		/// @param[in] unique Whether to force all random values to be unique.
		/// @return Array of random elements selected from this one.
		inline Array<T> random(int count, bool unique = false) const
		{
			Array<T> result;
			if (!unique)
			{
				for_iter (i, 0, count)
				{
					result.push_back(stdvector::at(hrand(this->size())));
				}
			}
			else if (count > 0)
			{
				if (count > this->size())
				{
					throw ContainerRangeException(0, count);
				}
				if (count == this->size())
				{
					return this->randomized();
				}
				Array<int> indexes;
				for_iter (i, 0, this->size())
				{
					indexes.push_back(i);
				}
				for_iter (i, 0, count)
				{
					result.push_back(stdvector::at(indexes.remove_at(hrand(indexes.size()))));
				}
			}
			return result;
		}
		/// @brief Gets a random element in Array and removes it.
		/// @return Random element.
		inline T pop_random()
		{
			if (this->size() == 0)
			{
				throw ContainerEmptyException("pop_random()");
			}
			T result = stdvector::at(hrand(this->size()));
			this->remove(result);
			return result;
		}
		/// @brief Gets an Array of random elements selected from this one and removes them.
		/// @param[in] count Number of random elements.
		/// @param[in] unique Whether to force all random values to be unique.
		/// @return Array of random elements selected from this one.
		inline Array<T> pop_random(int count, bool unique = false)
		{
			Array<T> result;
			if (!unique)
			{
				for_iter (i, 0, count)
				{
					result.push_back(stdvector::at(hrand(this->size())));
				}
			}
			else if (count > 0)
			{
				if (count > this->size())
				{
					throw ContainerRangeException(0, count);
				}
				if (count == this->size())
				{
					return this->randomized();
				}
				Array<int> indexes;
				for_iter (i, 0, this->size())
				{
					indexes.push_back(i);
				}
				for_iter (i, 0, count)
				{
					result.push_back(stdvector::at(indexes.remove_at(hrand(indexes.size()))));
				}
			}
			this->remove(result);
			return result;
		}
		/// @brief Joins all elements into a string.
		/// @param[in] separator Separator string between elements.
		/// @return String or joined elements separater by separator string.
		/// @note Make sure your elements can be cast into String or are already String.
		inline String join(const String& separator) const
		{
			String result;
			if (this->size() > 0)
			{
				result += String(stdvector::at(0));
				for_iter (i, 1, this->size())
				{
					result += separator + String(stdvector::at(i));
				}
			}
			return result;
		}
		/// @brief Finds and returns new Array of elements that match the condition.
		/// @param[in] condition_function Function pointer with condition function that takes one element of type T and returns bool.
		/// @return New Array with all matching elements.
		inline Array<T> find_all(bool (*condition_function)(T))
		{
			Array<T> result;
			for_iter (i, 0, this->size())
			{
				if (condition_function(stdvector::at(i)))
				{
					result.push_back(stdvector::at(i));
				}
			}
			return result;
		}
		/// @brief Finds and returns first occurrence of element that matches the condition.
		/// @param[in] condition_function Function pointer with condition function that takes one element of type T and returns bool.
		/// @return Pointer to element that matches the condition or NULL if no element was found.
		inline T* find_first(bool (*condition_function)(T))
		{
			for_iter (i, 0, this->size())
			{
				if (condition_function(stdvector::at(i)))
				{
					return &stdvector::at(i);
				}
			}
			return NULL;
		}
		/// @brief Checks if at least one element matches the condition.
		/// @param[in] condition_function Function pointer with condition function that takes one element of type T and returns bool.
		/// @return True if at least one element matches the condition.
		inline bool matches_any(bool (*condition_function)(T))
		{
			for_iter (i, 0, this->size())
			{
				if (condition_function(stdvector::at(i)))
				{
					return true;
				}
			}
			return false;
		}
		/// @brief Checks if all elements match the condition.
		/// @param[in] condition_function Function pointer with condition function that takes one element of type T and returns bool.
		/// @return True if all elements match the condition.
		inline bool matches_all(bool (*condition_function)(T))
		{
			for_iter (i, 0, this->size())
			{
				if (!condition_function(stdvector::at(i)))
				{
					return false;
				}
			}
			return true;
		}
		/// @brief Returns a new Array with all elements cast into type S.
		/// @return A new Array with all elements cast into type S.
		/// @note Make sure all elements in the Array can be cast into type S.
		template <class S>
		inline Array<S> cast()
		{
			Array<S> result;
			for_iter (i, 0, this->size())
			{
				result.push_back((S)stdvector::at(i));
			}
			return result;
		}
		/// @brief Returns a new Array with all elements dynamically cast into type S.
		/// @param[in] include_nulls Whether to include NULLs that failed to cast.
		/// @return A new Array with all elements cast into type S.
		/// @note Be careful not to use this function with non-pointers and classes that don't have virtual functions.
		template <class S>
		inline Array<S> dyn_cast(bool include_nulls = false)
		{
			Array<S> result;
			S value;
			for_iter (i, 0, this->size())
			{
				// when seeing "dynamic_cast", I always think of fireballs
				value = dynamic_cast<S>(stdvector::at(i));
				if (value != NULL || include_nulls)
				{
					result.push_back(value);
				}
			}
			return result;
		}
		/// @brief Accesses first element of Array.
		/// @return The first element.
		inline T& first()
		{
			return stdvector::front();
		}
		/// @brief Accesses last element of Array.
		/// @return The last element.
		inline T& last()
		{
			return stdvector::back();
		}
		/// @brief Same as contains.
		/// @see contains(const T& element)
		inline bool includes(const T& element) const
		{
			return this->contains(element);
		}
		/// @brief Same as contains.
		/// @see contains(const Array<T>& other)
		inline bool includes(const Array<T>& other) const
		{
			return this->contains(other);
		}
		/// @brief Same as contains.
		/// @see contains(const T other[], int count)
		inline bool includes(const T other[], int count) const
		{
			return this->contains(other, count);
		}
		/// @brief Same as contains.
		/// @see contains(const T& element)
		inline bool has(const T& element) const
		{
			return this->contains(element);
		}
		/// @brief Same as contains.
		/// @see contains(const Array<T>& other)
		inline bool has(const Array<T>& other) const
		{
			return this->contains(other);
		}
		/// @brief Same as contains.
		/// @see contains(const T other[], int count)
		inline bool has(const T other[], int count) const
		{
			return this->contains(other, count);
		}
		/// @brief Same as contains.
		/// @see contains(const T& element)
		inline bool has_element(const T& element) const
		{
			return this->contains(element);
		}
		/// @brief Same as contains.
		/// @see contains(const Array<T>& other)
		inline bool has_element(const Array<T>& other) const
		{
			return this->contains(other);
		}
		/// @brief Same as contains.
		/// @see contains(const T other[], int count)
		inline bool has_element(const T other[], int count) const
		{
			return this->contains(other, count);
		}
		/// @brief Same as push_back.
		/// @see push_back(const T& element)
		inline void add(const T& element)
		{
			this->push_back(element);
		}
		/// @brief Same as push_back.
		/// @see push_back(const T& element, int times)
		inline void add(const T& element, int times)
		{
			this->push_back(element, times);
		}
		/// @brief Same as push_back.
		/// @see push_back(const Array<T>& other)
		inline void add(const Array<T>& other)
		{
			this->push_back(other);
		}
		/// @brief Same as push_back.
		/// @see push_back(const Array<T>& other, const int count)
		inline void add(const Array<T>& other, const int count)
		{
			this->push_back(other, count);
		}
		/// @brief Same as push_back.
		/// @see push_back(const Array<T>& other, const int start, const int count)
		inline void add(const Array<T>& other, const int start, const int count)
		{
			this->push_back(other, start, count);
		}
		/// @brief Same as push_back.
		/// @see push_back(const T other[], const int count)
		inline void add(const T other[], const int count)
		{
			this->push_back(other, count);
		}
		/// @brief Same as push_back.
		/// @see push_back(const T other[], const int start, const int count)
		inline void add(const T other[], const int start, const int count)
		{
			this->push_back(other, start, count);
		}
		/// @brief Same as push_back.
		/// @see push_back(const T& element)
		inline void append(const T& element)
		{
			this->push_back(element);
		}
		/// @brief Same as push_back.
		/// @see push_back(const T& element, int times)
		inline void append(const T& element, int times)
		{
			this->push_back(element, times);
		}
		/// @brief Same as push_back.
		/// @see push_back(const Array<T>& other)
		inline void append(const Array<T>& other)
		{
			this->push_back(other);
		}
		/// @brief Same as push_back.
		/// @see push_back(const Array<T>& other, const int count)
		inline void append(const Array<T>& other, const int count)
		{
			this->push_back(other, count);
		}
		/// @brief Same as push_back.
		/// @see push_back(const Array<T>& other, const int start, const int count)
		inline void append(const Array<T>& other, const int start, const int count)
		{
			this->push_back(other, start, count);
		}
		/// @brief Same as push_back.
		/// @see push_back(const T other[], const int count)
		inline void append(const T other[], const int count)
		{
			this->push_back(other, count);
		}
		/// @brief Same as push_back.
		/// @see push_back(const T other[], const int start, const int count)
		inline void append(const T other[], const int start, const int count)
		{
			this->push_back(other, start, count);
		}
		/// @brief Same as pop_random.
		/// @see pop_random().
		inline T remove_random()
		{
			return this->pop_random();
		}
		/// @brief Same as pop_random.
		/// @see pop_random(const int count).
		inline Array<T> remove_random(const int count)
		{
			return this->pop_random(count);
		}

		// DEPRECATED
		inline T pop(int index)							{ return this->removeAt(index); }
		inline Array<T> pop(int index, int count)		{ return this->removeAt(index, count); }
		inline T pop_at(int index)						{ return this->removeAt(index); }
		inline Array<T> pop_at(int index, int count)	{ return this->removeAt(index, count); }
		inline T pop_front()							{ return this->removeFirst(); }
		inline Array<T> pop_front(const int count)		{ return this->removeFirst(count); }
		inline T pop_first()							{ return this->removeFirst(); }
		inline Array<T> pop_first(const int count)		{ return this->removeFirst(count); }
		inline T pop_back()								{ return this->removeLast(); }
		inline Array<T> pop_back(const int count)		{ return this->removeLast(count); }
		inline T pop_last()								{ return this->removeLast(); }
		inline Array<T> pop_last(const int count)		{ return this->removeLast(count); }
		inline T pop_all(T& element)					{ return this->removeAll(element); }
		inline Array<T> pop_all(const Array<T>& other)	{ return this->removeAll(other); }
		inline T remove_at(int index)					{ return this->removeAt(index); }
		inline Array<T> remove_at(int index, int count)	{ return this->removeAt(index, count); }
		inline T remove_front()							{ return this->removeFirst(); }
		inline Array<T> remove_front(const int count)	{ return this->removeFirst(count); }
		inline T remove_first()							{ return this->removeFirst(); }
		inline Array<T> remove_first(const int count)	{ return this->removeFirst(count); }
		inline T remove_back()							{ return this->removeLast(); }
		inline Array<T> remove_back(const int count)	{ return this->removeLast(count); }
		inline T remove_last()							{ return this->removeLast(); }
		inline Array<T> remove_last(const int count)	{ return this->removeLast(count); }

	};
	
}

/// @brief Alias for simpler code.
#define harray hltypes::Array

#endif
