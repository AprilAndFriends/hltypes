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
/// Encapsulates std::deque and adds high level methods.

#ifndef HLTYPES_DEQUE_H
#define HLTYPES_DEQUE_H

#include <algorithm>
#include <deque>

#include "hcontainer.h"
#include "hexception.h"
#include "hltypesUtil.h"
#include "hplatform.h"
#include "hstring.h"

/// @brief Provides a simpler syntax to iterate through a ldeque.
#define foreach_q(type, name, container) for (std::deque< type >::iterator name = (container).begin(); name != (container).end(); ++name)
#define foreachc_q(type, name, container) for (std::deque< type >::const_iterator name = (container).begin(); name != (container).end(); ++name)
/// @brief Provides a simpler syntax to reverse iterate through a Deque.
#define foreach_qr(type, name, container) for (std::deque< type >::reverse_iterator name = (container).rbegin(); name != (container).rend(); ++name)
#define foreachc_qr(type, name, container) for (std::deque< type >::reverse_const_iterator name = (container).rbegin(); name != (container).rend(); ++name)
/// @brief Alias for simpler code.
#define ContainerDeque Container<std::deque<T>, T>

namespace hltypes
{
	/// @brief Encapsulates std::deque and adds high level methods.
	template <class T>
	class Deque : public ContainerDeque
	{
	private:
		typedef typename std::deque<T>::iterator iterator_t;
		typedef typename std::deque<T>::const_iterator const_iterator_t;
	public:
		/// @brief Empty constructor.
		inline Deque() : ContainerDeque()
		{
		}
		/// @brief Copy constructor.
		/// @param[in] other Container to copy.
		inline Deque(const ContainerDeque& other) : ContainerDeque(other)
		{
		}
		/// @brief Constructor from single element.
		/// @param[in] element Element to insert.
		inline Deque(const T& element) : ContainerDeque(element)
		{
		}
		/// @brief Constructor from single element.
		/// @param[in] element Element to insert.
		/// @param[in] times Number of times to insert element.
		inline Deque(const T& element, int times) : ContainerDeque(element, times)
		{
		}
		/// @brief Constructor from another Container.
		/// @param[in] other Container to copy.
		/// @param[in] count Number of elements to copy.
		inline Deque(const ContainerDeque& other, const int count) : ContainerDeque(other, count)
		{
		}
		/// @brief Constructor from another Container.
		/// @param[in] other Container to copy.
		/// @param[in] start Start index of elements to copy.
		/// @param[in] count Number of elements to copy.
		inline Deque(const ContainerDeque& other, const int start, const int count) : ContainerDeque(other, start, count)
		{
		}
		/// @brief Constructor from C-type array.
		/// @param[in] other C-type array to copy.
		/// @param[in] count Number of elements to copy.
		inline Deque(const T other[], const int count) : ContainerDeque(other, count)
		{
		}
		/// @brief Constructor from C-type array.
		/// @param[in] other C-type array to copy.
		/// @param[in] start Start index of elements to copy.
		/// @param[in] count Number of elements to copy.
		inline Deque(const T other[], const int start, const int count) : ContainerDeque(other, start, count)
		{
		}
		/// @brief Destructor.
		inline ~Deque()
		{
		}
		/// @brief Gets all indexes of the given element.
		/// @param[in] element Element to search for.
		/// @return All indexes of the given element.
		inline Deque<int> indexesOf(const T& element) const
		{
			return this->template _indexesOf<Deque<int> >(element);
		}
		/// @brief Removes element at given index.
		/// @param[in] index Index of element to remove.
		/// @return The removed element.
		inline T removeAt(int index)
		{
			return ContainerDeque::removeAt(index);
		}
		/// @brief Removes n elements at given index of Deque.
		/// @param[in] index Start index of elements to remove.
		/// @param[in] count Number of elements to remove.
		/// @return Deque of all removed elements.
		/// @note Elements in the returned Deque are in the same order as in the orignal Deque.
		inline Deque<T> removeAt(int index, int count)
		{
			return this->template _removeAt<Deque<T> >(index, count);
		}
		/// @brief Removes first element of Deque.
		/// @return The removed element.
		inline T removeFirst()
		{
			return ContainerDeque::removeFirst();
		}
		/// @brief Removes n elements from the beginning of Deque.
		/// @param[in] count Number of elements to remove.
		/// @return Deque of all removed elements.
		/// @note Elements in the returned Deque are in the same order as in the orignal Deque.
		inline Deque<T> removeFirst(const int count)
		{
			return this->template _removeFirst<Deque<T> >(count);
		}
		/// @brief Removes last element of Deque.
		/// @return The removed element.
		inline T removeLast()
		{
			return ContainerDeque::removeLast();
		}
		/// @brief Removes n elements from the end of Deque.
		/// @param[in] count Number of elements to remove.
		/// @return Deque of all removed elements.
		/// @note Elements in the returned Deque are in the same order as in the orignal Deque.
		inline Deque<T> removeLast(const int count)
		{
			return this->template _removeLast<Deque<T> >(count);
		}
		/// @brief Creates new Deque with reversed order of elements.
		/// @return A new Deque.
		inline Deque<T> reversed() const
		{
			return this->template _reversed<Deque<T> >();
		}
		/// @brief Creates new Deque without duplicates.
		/// @return A new Deque.
		inline Deque<T> removedDuplicates() const
		{
			return this->template _removedDuplicates<Deque<T> >();
		}
		/// @brief Creates a new Deque as union of this Deque with an element.
		/// @param[in] element Element to unite with.
		/// @return A new Deque.
		inline Deque<T> united(const T& element) const
		{
			return this->template _united<Deque<T> >(element);
		}
		/// @brief Creates a new Deque as union of this Deque with another one.
		/// @param[in] other Deque to unite with.
		/// @return A new Deque.
		inline Deque<T> united(const Deque<T>& other) const
		{
			return this->template _united<Deque<T> >(other);
		}
		/// @brief Creates a new Deque as intersection of this Deque with another one.
		/// @param[in] other Deque to intersect with.
		/// @return A new Deque.
		inline Deque<T> intersected(const Deque<T>& other) const
		{
			return this->template _intersected<Deque<T> >(other);
		}
		/// @brief Creates a new Deque as difference of this Deque with an element.
		/// @param[in] other Element to differentiate with.
		/// @return A new Deque.
		/// @note Unlike remove, this method ignores if the element is not in this Deque.
		inline Deque<T> differentiated(const T& element) const
		{
			return this->template _differentiated<Deque<T> >(element);
		}
		/// @brief Creates a new Deque as difference of this Deque with another one.
		/// @param[in] other Deque to differentiate with.
		/// @return A new Deque.
		/// @note Unlike remove, this method ignore elements of other Deque that are not in this one.
		inline Deque<T> differentiated(const Deque<T>& other) const
		{
			return this->template _differentiated<Deque<T> >(other);
		}
		/// @brief Returns element at specified position.
		/// @param[in] index Index of the element.
		/// @return Element at specified position.
		/// @note Does not work with bool as T, use Deque::at directly instead.
		inline T& operator[](int index)
		{
			return this->at(index);
		}
		/// @brief Returns element at specified position.
		/// @param[in] index Index of the element.
		/// @return Element at specified position.
		/// @note Does not work with bool as T, use Deque::at directly instead.
		inline const T& operator[](int index) const
		{
			return this->at(index);
		}
		/// @brief Returns a subdeque.
		/// @param[in] start Start index of the elements to copy.
		/// @param[in] count Number of elements to copy.
		/// @return Subdeque created from the current Deque.
		inline Deque<T> operator()(int start, const int count) const
		{
			return this->template _sub<Deque<T> >(start, count);
		}
		/// @brief Same as equals.
		/// @see equals
		inline bool operator==(const Deque<T>& other) const
		{
			return this->equals(other);
		}
		/// @brief Same as nequals.
		/// @see nequals
		inline bool operator!=(const Deque<T>& other) const
		{
			return this->nequals(other);
		}
		/// @brief Same as push_back.
		/// @see push_back(const T& element)
		inline Deque<T>& operator<<(const T& element)
		{
			this->push_back(element);
			return (*this);
		}
		/// @brief Same as push_back.
		/// @see push_back(const Deque<T>& other)
		inline Deque<T>& operator<<(const Deque<T>& other)
		{
			this->push_back(other);
			return (*this);
		}
		/// @brief Same as push_back.
		/// @see push_back(const T& element)
		inline Deque<T>& operator+=(const T& element)
		{
			this->push_back(element);
			return (*this);
		}
		/// @brief Same as push_back.
		/// @see push_back(const Deque<T>& other)
		inline Deque<T>& operator+=(const Deque<T>& other)
		{
			this->push_back(other);
			return (*this);
		}
		/// @brief Same as remove.
		/// @see remove(T element)
		inline Deque<T>& operator-=(T element)
		{
			this->remove(element);
			return (*this);
		}
		/// @brief Same as remove.
		/// @see remove(const Deque<T>& other)
		inline Deque<T>& operator-=(const Deque<T>& other)
		{
			this->remove(other);
			return (*this);
		}
		/// @brief Same as unite.
		/// @see unite(const T& element)
		inline Deque<T>& operator|=(const T& element)
		{
			this->unite(element);
			return (*this);
		}
		/// @brief Same as unite.
		/// @see unite(const Deque<T>& other)
		inline Deque<T>& operator|=(const Deque<T>& other)
		{
			this->unite(other);
			return (*this);
		}
		/// @brief Same as intersect.
		/// @see intersect(const Deque<T>& other)
		inline Deque<T>& operator&=(const Deque<T>& other)
		{
			this->intersect(other);
			return (*this);
		}
		/// @brief Same as differentiate.
		/// @see differentiate(const T& element)
		inline Deque<T>& operator/=(const T& element)
		{
			this->differentiate(element);
			return (*this);
		}
		/// @brief Same as differentiate.
		/// @see differentiate(const Deque<T>& other)
		inline Deque<T>& operator/=(const Deque<T>& other)
		{
			this->differentiate(other);
			return (*this);
		}
		/// @brief Merges a Deque with an element.
		/// @param[in] element Element to merge with.
		/// @return New Deque with element added at the end of Deque.
		inline Deque<T> operator+(const T& element) const
		{
			Deque<T> result(*this);
			result += element;
			return result;
		}
		/// @brief Merges two Deques.
		/// @param[in] other Second Deque to merge with.
		/// @return New Deque with elements of second Deque added at the end of first Deque.
		inline Deque<T> operator+(const Deque<T>& other) const
		{
			Deque<T> result(*this);
			result += other;
			return result;
		}
		/// @brief Removes element from Deque.
		/// @param[in] element Element to remove.
		/// @return New Deque with elements of first Deque without given element.
		inline Deque<T> operator-(T element) const
		{
			Deque<T> result(*this);
			result -= element;
			return result;
		}
		/// @brief Removes second Deque from first Deque.
		/// @param[in] other Deque to remove.
		/// @return New Deque with elements of first Deque without the elements of second Deque.
		inline Deque<T> operator-(const Deque<T>& other) const
		{
			Deque<T> result(*this);
			result -= other;
			return result;
		}
		/// @brief Same as united.
		/// @see united(const T& element)
		inline Deque<T> operator|(const T& element) const
		{
			return this->united(element);
		}
		/// @brief Same as united.
		/// @see united(const Deque<T>& other)
		inline Deque<T> operator|(const Deque<T>& other) const
		{
			return this->united(other);
		}
		/// @brief Same as intersected.
		/// @see intersected(const Deque<T>& other)
		inline Deque<T> operator&(const Deque<T>& other) const
		{
			return this->intersected(other);
		}
		/// @brief Same as differentiated.
		/// @see differentiated(const T& element)
		inline Deque<T> operator/(const T& element) const
		{
			return this->differentiated(element);
		}
		/// @brief Same as differentiated.
		/// @see differentiated(const Deque<T>& other)
		inline Deque<T> operator/(const Deque<T>& other) const
		{
			return this->differentiated(other);
		}
		/// @brief Sorts elements in Deque.
		/// @note The sorting order is ascending.
		inline void sort()
		{
			if (this->size() > 0)
			{
				std::stable_sort(std::deque<T>::begin(), std::deque<T>::end());
			}
		}
		/// @brief Sorts elements in Deque.
		/// @param[in] compare_function Function pointer with comparison function that takes two elements of type T and returns bool.
		/// @note The sorting order is ascending.
		/// @note compare_function should return true if first element is less than the second element.
		inline void sort(bool (*compare_function)(T, T))
		{
			if (this->size() > 0)
			{
				std::stable_sort(std::deque<T>::begin(), std::deque<T>::end(), compare_function);
			}
		}
		/// @brief Creates new sorted Deque.
		/// @return A new Deque.
		/// @note The sorting order is ascending.
		inline Deque<T> sorted() const
		{
			Deque<T> result(*this);
			result.sort();
			return result;
		}
		/// @brief Creates new sorted Deque.
		/// @param[in] compare_function Function pointer with comparison function that takes two elements of type T and returns bool.
		/// @return A new Deque.
		/// @note The sorting order is ascending.
		/// @note compare_function should return true if first element is less than the second element.
		inline Deque<T> sorted(bool (*compare_function)(T, T)) const
		{
			Deque<T> result(*this);
			result.sort(compare_function);
			return result;
		}
		/// @brief Randomizes order of elements in Deque.
		inline void randomize()
		{
			std::random_shuffle(std::deque<T>::begin(), std::deque<T>::end());
		}
		/// @brief Creates a new Deque with randomized order of elements.
		/// @return A new Deque.
		inline Deque<T> randomized() const
		{
			Deque<T> result(*this);
			result.randomize();
			return result;
		}
		/// @brief Finds minimum element in Deque.
		/// @return Minimum Element.
		inline T min() const
		{
			if (this->size() == 0)
			{
				throw ContainerEmptyException("min()");
			}
			return (*std::min_element(std::deque<T>::begin(), std::deque<T>::end()));
		}
		/// @brief Finds minimum element in Deque.
		/// @param[in] compare_function Function pointer with comparison function that takes two elements of type T and returns bool.
		/// @return Minimum Element.
		/// @note compare_function should return true if first element is less than second element.
		inline T min(bool (*compare_function)(T, T)) const
		{
			if (this->size() == 0)
			{
				throw ContainerEmptyException("min()");
			}
			return (*std::min_element(std::deque<T>::begin(), std::deque<T>::end(), compare_function));
		}
		/// @brief Finds maximum element in Deque.
		/// @return Maximum Element.
		inline T max() const
		{
			if (this->size() == 0)
			{
				throw ContainerEmptyException("max()");
			}
			return (*std::max_element(std::deque<T>::begin(), std::deque<T>::end()));
		}
		/// @brief Finds maximum element in Deque.
		/// @param[in] compare_function Function pointer with comparison function that takes two elements of type T and returns bool.
		/// @return Maximum Element.
		/// @note compare_function should return true if first element is greater than second element.
		inline T max(bool (*compare_function)(T, T)) const
		{
			if (this->size() == 0)
			{
				throw ContainerEmptyException("max()");
			}
			return (*std::max_element(std::deque<T>::begin(), std::deque<T>::end(), compare_function));
		}
		/// @brief Gets a random element in Deque.
		/// @return Random element.
		inline T random() const
		{
			if (this->size() == 0)
			{
				throw ContainerEmptyException("random()");
			}
			return this->at(hrand(this->size()));
		}
		/// @brief Gets a Deque of random elements selected from this one.
		/// @param[in] count Number of random elements.
		/// @param[in] unique Whether to force all random values to be unique.
		/// @return Deque of random elements selected from this one.
		inline Deque<T> random(int count, bool unique = false) const
		{
			Deque<T> result;
			if (!unique)
			{
				for_iter (i, 0, count)
				{
					result.push_back(this->at(hrand(this->size())));
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
				Deque<int> indexes;
				for_iter (i, 0, this->size())
				{
					indexes.push_back(i);
				}
				for_iter (i, 0, count)
				{
					result.push_back(this->at(indexes.remove_at(hrand(indexes.size()))));
				}
			}
			return result;
		}
		/// @brief Gets a random element in Deque and removes it.
		/// @return Random element.
		inline T pop_random()
		{
			if (this->size() == 0)
			{
				throw ContainerEmptyException("pop_random()");
			}
			T result = this->at(hrand(this->size()));
			this->remove(result);
			return result;
		}
		/// @brief Gets a Deque of random elements selected from this one and removes them.
		/// @param[in] count Number of random elements.
		/// @param[in] unique Whether to force all random values to be unique.
		/// @return Deque of random elements selected from this one.
		inline Deque<T> pop_random(int count, bool unique = false)
		{
			Deque<T> result;
			if (!unique)
			{
				for_iter (i, 0, count)
				{
					result.push_back(this->at(hrand(this->size())));
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
				Deque<int> indexes;
				for_iter (i, 0, this->size())
				{
					indexes.push_back(i);
				}
				for_iter (i, 0, count)
				{
					result.push_back(this->at(indexes.remove_at(hrand(indexes.size()))));
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
				result += String(this->at(0));
				for_iter (i, 1, this->size())
				{
					result += separator + String(this->at(i));
				}
			}
			return result;
		}
		/// @brief Finds and returns new Deque of elements that match the condition.
		/// @param[in] condition_function Function pointer with condition function that takes one element of type T and returns bool.
		/// @return New Deque with all matching elements.
		inline Deque<T> find_all(bool (*condition_function)(T))
		{
			Deque<T> result;
			for_iter (i, 0, this->size())
			{
				if (condition_function(this->at(i)))
				{
					result.push_back(this->at(i));
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
				if (condition_function(this->at(i)))
				{
					return &this->at(i);
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
				if (condition_function(this->at(i)))
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
				if (!condition_function(this->at(i)))
				{
					return false;
				}
			}
			return true;
		}
		/// @brief Returns a new Deque with all elements cast into type S.
		/// @return A new Deque with all elements cast into type S.
		/// @note Make sure all elements in the Deque can be cast into type S.
		template <class S>
		inline Deque<S> cast()
		{
			Deque<S> result;
			for_iter (i, 0, this->size())
			{
				result.push_back((S)this->at(i));
			}
			return result;
		}
		/// @brief Returns a new Deque with all elements dynamically cast into type S.
		/// @param[in] include_nulls Whether to include NULLs that failed to cast.
		/// @return A new Deque with all elements cast into type S.
		/// @note Be careful not to use this function with non-pointers and classes that don't have virtual functions.
		template <class S>
		inline Deque<S> dyn_cast(bool include_nulls = false)
		{
			Deque<S> result;
			S value;
			for_iter (i, 0, this->size())
			{
				// when seeing "dynamic_cast", I always think of fireballs
				value = dynamic_cast<S>(this->at(i));
				if (value != NULL || include_nulls)
				{
					result.push_back(value);
				}
			}
			return result;
		}
		/// @brief Accesses first element of Deque.
		/// @return The first element.
		inline T& first()
		{
			return std::deque<T>::front();
		}
		/// @brief Accesses last element of Deque.
		/// @return The last element.
		inline T& last()
		{
			return std::deque<T>::back();
		}
		/// @brief Same as contains.
		/// @see contains(const T& element)
		inline bool includes(const T& element) const
		{
			return this->contains(element);
		}
		/// @brief Same as contains.
		/// @see contains(const Deque<T>& other)
		inline bool includes(const Deque<T>& other) const
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
		/// @see contains(const Deque<T>& other)
		inline bool has(const Deque<T>& other) const
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
		/// @see contains(const Deque<T>& other)
		inline bool has_element(const Deque<T>& other) const
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
		/// @see push_back(const Deque<T>& other)
		inline void add(const Deque<T>& other)
		{
			this->push_back(other);
		}
		/// @brief Same as push_back.
		/// @see push_back(const Deque<T>& other, const int count)
		inline void add(const Deque<T>& other, const int count)
		{
			this->push_back(other, count);
		}
		/// @brief Same as push_back.
		/// @see push_back(const Deque<T>& other, const int start, const int count)
		inline void add(const Deque<T>& other, const int start, const int count)
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
		/// @see push_back(const Deque<T>& other)
		inline void append(const Deque<T>& other)
		{
			this->push_back(other);
		}
		/// @brief Same as push_back.
		/// @see push_back(const Deque<T>& other, const int count)
		inline void append(const Deque<T>& other, const int count)
		{
			this->push_back(other, count);
		}
		/// @brief Same as push_back.
		/// @see push_back(const Deque<T>& other, const int start, const int count)
		inline void append(const Deque<T>& other, const int start, const int count)
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
		inline Deque<T> remove_random(const int count)
		{
			return this->pop_random(count);
		}

		// DEPRECATED
		inline Deque<int> indexes_of(const T& element) const	{ return this->indexesOf(element); }
		inline Deque<T> removed_duplicates() const				{ return this->removedDuplicates(); }
		inline T pop(int index)									{ return this->removeAt(index); }
		inline Deque<T> pop(int index, int count)				{ return this->removeAt(index, count); }
		inline T pop_at(int index)								{ return this->removeAt(index); }
		inline Deque<T> pop_at(int index, int count)			{ return this->removeAt(index, count); }
		inline T pop_front()									{ return this->removeFirst(); }
		inline Deque<T> pop_front(const int count)				{ return this->removeFirst(count); }
		inline T pop_first()									{ return this->removeFirst(); }
		inline Deque<T> pop_first(const int count)				{ return this->removeFirst(count); }
		inline T pop_back()										{ return this->removeLast(); }
		inline Deque<T> pop_back(const int count)				{ return this->removeLast(count); }
		inline T pop_last()										{ return this->removeLast(); }
		inline Deque<T> pop_last(const int count)				{ return this->removeLast(count); }
		inline T pop_all(T& element)							{ return this->removeAll(element); }
		inline Deque<T> pop_all(const Deque<T>& other)			{ return this->removeAll(other); }
		inline T remove_at(int index)							{ return this->removeAt(index); }
		inline Deque<T> remove_at(int index, int count)			{ return this->removeAt(index, count); }
		inline T remove_front()									{ return this->removeFirst(); }
		inline Deque<T> remove_front(const int count)			{ return this->removeFirst(count); }
		inline T remove_first()									{ return this->removeFirst(); }
		inline Deque<T> remove_first(const int count)			{ return this->removeFirst(count); }
		inline T remove_back()									{ return this->removeLast(); }
		inline Deque<T> remove_back(const int count)			{ return this->removeLast(count); }
		inline T remove_last()									{ return this->removeLast(); }
		inline Deque<T> remove_last(const int count)			{ return this->removeLast(count); }

	};
	
}

/// @brief Alias for simpler code.
#define hdeque hltypes::Deque

#endif
