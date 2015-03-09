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
/// Encapsulates all containers and adds high level methods.

#ifndef HLTYPES_CONTAINER_H
#define HLTYPES_CONTAINER_H

#include "hexception.h"
#include "hltypesUtil.h"
#include "hstring.h"

namespace hltypes
{
	/// @brief Encapsulates container functionality and adds high level methods.
	template <class STD, class T>
	class Container : public STD
	{
	private:
		typedef typename STD::iterator iterator_t;
		typedef typename STD::const_iterator const_iterator_t;
	public:
		/// @brief Empty constructor.
		inline Container() : STD()
		{
		}
		/// @brief Copy constructor.
		/// @param[in] other Container to copy.
		inline Container(const Container& other) : STD()
		{
			this->insert_at(0, other);
		}
		/// @brief Constructor from single element.
		/// @param[in] element Element to insert.
		inline Container(const T& element) : STD()
		{
			this->insert_at(0, element);
		}
		/// @brief Constructor from single element.
		/// @param[in] element Element to insert.
		/// @param[in] times Number of times to insert element.
		inline Container(const T& element, int times) : STD()
		{
			this->insert_at(0, element, times);
		}
		/// @brief Constructor from another Container.
		/// @param[in] other Container to copy.
		/// @param[in] count Number of elements to copy.
		inline Container(const Container& other, const int count) : STD()
		{
			this->insert_at(0, other, count);
		}
		/// @brief Constructor from another Container.
		/// @param[in] other Container to copy.
		/// @param[in] start Start index of elements to copy.
		/// @param[in] count Number of elements to copy.
		inline Container(const Container& other, const int start, const int count) : STD()
		{
			this->insert_at(0, other, start, count);
		}
		/// @brief Constructor from C-type array.
		/// @param[in] other C-type array to copy.
		/// @param[in] count Number of elements to copy.
		inline Container(const T other[], const int count) : STD()
		{
			this->insert_at(0, other, count);
		}
		/// @brief Constructor from C-type array.
		/// @param[in] other C-type array to copy.
		/// @param[in] start Start index of elements to copy.
		/// @param[in] count Number of elements to copy.
		inline Container(const T other[], const int start, const int count) : STD()
		{
			this->insert_at(0, other, start, count);
		}
		/// @brief Destructor.
		inline ~Container()
		{
		}
		/// @brief Returns the number of elements in the Container.
		/// @return The number of elements in the Container.
		inline int size() const
		{
			return (int)STD::size();
		}
		/// @brief Compares the contents of two Containers for being equal.
		/// @param[in] other Another Container.
		/// @return True if number of elements are equal and all pairs of elements at the same positions are equal.
		inline bool equals(const Container& other) const
		{
			int size = this->size();
			if (size != other.size())
			{
				return false;
			}
			for_iter (i, 0, size)
			{
				// making sure operator== is used, not !=
				if (!(this->at(i) == other.at(i)))
				{
					return false;
				}
			}
			return true;
		}
		/// @brief Compares the contents of two Containers for being not equal.
		/// @param[in] other Another Container.
		/// @return True if number of elements are not equal or at least one pair of elements at the same positions is not equal.
		inline bool nequals(const Container& other) const
		{
			int size = this->size();
			if (size != other.size())
			{
				return true;
			}
			for_iter (i, 0, size)
			{
				// making sure operator!= is used, not ==
				if (this->at(i) != other.at(i))
				{
					return true;
				}
			}
			return false;
		}
		/// @brief Returns element at specified position.
		/// @param[in] index Index of the element.
		/// @return Element at specified position.
		inline T& at(int index)
		{
			int size = this->size();
			if (index < 0)
			{
				index += size;
			}
			if (index < 0 || index >= size)
			{
				throw ContainerIndexException(index);
			}
			return *this->_itAdvance(STD::begin(), index);
		}
		/// @brief Returns element at specified position.
		/// @param[in] index Index of the element.
		/// @return Element at specified position.
		inline const T& at(int index) const
		{
			int size = this->size();
			if (index < 0)
			{
				index += size;
			}
			if (index < 0 || index >= size)
			{
				throw ContainerIndexException(index);
			}
			return *this->_itAdvance(STD::begin(), index);
		}
		/// @brief Gets index of the given element.
		/// @param[in] element Element to search for.
		/// @return Index of the given element or -1 if element could not be found.
		inline int index_of(T element) const
		{
			for_iter (i, 0, this->size())
			{
				if (element == this->at(i))
				{
					return i;
				}
			}
			return -1;
		}
		/// @brief Checks existence of element in Container.
		/// @param[in] element Element to search for.
		/// @return True if element is in Container.
		inline bool contains(const T& element) const
		{
			return (this->index_of(element) >= 0);
		}
		/// @brief Checks existence of elements in Container.
		/// @param[in] other Container with elements to search for.
		/// @return True if all elements are in Container.
		inline bool contains(const Container& other) const
		{
			int size = other.size();
			for_iter (i, 0, size)
			{
				if (this->index_of(other.at(i)) < 0)
				{
					return false;
				}
			}
			return true;
		}
		/// @brief Checks existence of elements in Container.
		/// @param[in] other C-type array with elements to search for.
		/// @param[in] count How many elements the C-type array has.
		/// @return True if all elements are in Container.
		inline bool contains(const T other[], int count) const
		{
			for_iter (i, 0, count)
			{
				if (this->index_of(other[i]) < 0)
				{
					return false;
				}
			}
			return true;
		}
		/// @brief Counts occurrences of element in Container.
		/// @param[in] element Element to search for.
		/// @return Number of occurrences of given element.
		inline int count(T element) const
		{
			int result = 0;
			int size = this->size();
			for_iter (i, 0, size)
			{
				if (element == this->at(i))
				{
					++result;
				}
			}
			return result;
		}
		/// @brief Inserts new element at specified position n times.
		/// @param[in] index Position where to insert the new element.
		/// @param[in] element Element to insert.
		/// @param[in] times Number of times to insert element.
		inline void insert_at(const int index, const T& element, const int times = 1)
		{
			if (index < 0 || index > this->size())
			{
				throw ContainerIndexException(index);
			}
			STD::insert(this->_itAdvance(STD::begin(), index), times, element);
		}
		/// @brief Inserts all elements of another Container into this one.
		/// @param[in] index Position where to insert the new elements.
		/// @param[in] other Container of elements to insert.
		inline void insert_at(const int index, const Container& other)
		{
			if (index < 0 || index > this->size())
			{
				throw ContainerIndexException(index);
			}
			STD::insert(this->_itAdvance(STD::begin(), index), other.begin(), other.end());
		}
		/// @brief Inserts all elements of another Container into this one.
		/// @param[in] index Position where to insert the new elements.
		/// @param[in] other Container of elements to insert.
		/// @param[in] count Number of elements to insert.
		inline void insert_at(const int index, const Container& other, const int count)
		{
			if (index < 0 || index > this->size())
			{
				throw ContainerIndexException(index);
			}
			if (count > other.size())
			{
				throw ContainerRangeException(0, count);
			}
			const_iterator_t it = other.begin();
			STD::insert(this->_itAdvance(STD::begin(), index), it, it + count);
		}
		/// @brief Inserts all elements of another Container into this one.
		/// @param[in] index Position where to insert the new elements.
		/// @param[in] other Container of elements to insert.
		/// @param[in] start Start index of the elements to insert.
		/// @param[in] count Number of elements to insert.
		inline void insert_at(const int index, const Container& other, const int start, const int count)
		{
			if (index < 0 || index > this->size())
			{
				throw ContainerIndexException(index);
			}
			if (start < 0 || start >= other.size() || start + count > other.size())
			{
				throw ContainerRangeException(start, count);
			}
			const_iterator_t it = this->_itAdvance(other.begin(), start);
			STD::insert(this->_itAdvance(STD::begin(), index), it, it + count);
		}
		/// @brief Inserts all elements of a C-type array into this Container.
		/// @param[in] index Position where to insert the new elements.
		/// @param[in] other C-type array of elements to insert.
		/// @param[in] count Number of elements to insert.
		inline void insert_at(const int index, const T other[], const int count)
		{
			STD::insert(this->_itAdvance(STD::begin(), index), other, other + count);
		}
		/// @brief Inserts all elements of a C-type array into this Container.
		/// @param[in] index Position where to insert the new elements.
		/// @param[in] other C-type array of elements to insert.
		/// @param[in] start Start index of the elements to insert.
		/// @param[in] count Number of elements to insert.
		inline void insert_at(const int index, const T other[], const int start, const int count)
		{
			STD::insert(this->_itAdvance(STD::begin(), index), other + start, other + (start + count));
		}
		/// @brief Removes element at given index.
		/// @param[in] index Index of element to remove.
		/// @return The removed element.
		inline T removeAt(int index)
		{
			int size = this->size();
			if (index < 0)
			{
				index -= size;
			}
			if (index < 0 || index >= size)
			{
				throw ContainerIndexException(index);
			}
			T result = this->at(index);
			STD::erase(this->_itAdvance(STD::begin(), index));
			return result;
		}
		/// @brief Removes first element of Array.
		/// @return The removed element.
		inline T removeFirst()
		{
			return this->removeAt(0);
		}
		/// @brief Removes last element of Array.
		/// @return The removed element.
		inline T removeLast()
		{
			if (this->size() == 0)
			{
				throw ContainerIndexException(0);
			}
			T element = STD::back();
			STD::pop_back();
			return element;
		}
		/// @brief Removes first occurrence of element in Container.
		/// @param[in] element Element to remove.
		inline void remove(T element)
		{
			int index = this->index_of(element);
			if (index < 0)
			{
				throw ContainerElementNotFoundException();
			}
			STD::erase(this->_itAdvance(STD::begin(), index));
		}
		/// @brief Removes first occurrence of each element in another Container from this one.
		/// @param[in] other Container of elements to remove.
		inline void remove(const Container& other)
		{
			int size = other.size();
			int index = 0;
			for_iter (i, 0, size)
			{
				index = this->index_of(other.at(i));
				if (index < 0)
				{
					throw ContainerElementNotFoundException();
				}
				STD::erase(this->_itAdvance(STD::begin(), index));
			}
		}
		/// @brief Removes all occurrences of element in Container.
		/// @param[in] element Element to remove.
		/// @return Number of elements removed.
		inline int removeAll(const T& element)
		{
			Container<std::vector<int>, int> indexes = this->_indexesOf<Container<std::vector<int>, int> >(element);
			iterator_t it = STD::begin();
			int size = indexes.size();
			for_iter_r (i, size, 0)
			{
				STD::erase(this->_itAdvance(it, indexes[i]));
			}
			return size;
		}
		/// @brief Removes all occurrences of each element in another Container from this one.
		/// @param[in] other Container of elements to remove.
		/// @return Number of elements removed.
		inline int removeAll(const Container& other)
		{
			Container<std::vector<int>, int> indexes;
			iterator_t it;
			int indexesSize = 0;
			int count = 0;
			for_iter(i, 0, other.size()) // has to stay other.size() here
			{
				indexes = this->_indexesOf<Container<std::vector<int>, int> >(other.at(i));
				it = STD::begin();
				indexesSize = indexes.size();
				for_iter_r (j, indexesSize, 0)
				{
					STD::erase(this->_itAdvance(it, indexes[j]));
				}
				count += indexesSize;
			}
			return count;
		}
		/// @brief Adds element at the end of Container.
		/// @param[in] element Element to add.
		inline void addLast(const T& element)
		{
			STD::push_back(element);
		}
		/// @brief Adds element at the end of Container n times.
		/// @param[in] element Element to add.
		/// @param[in] times Number of times to add the element.
		inline void addLast(const T& element, int times)
		{
			this->insert_at(this->size(), element, times);
		}
		/// @brief Adds all elements from another Container at the end of this one.
		/// @param[in] other Container of elements to add.
		inline void addLast(const Container& other)
		{
			this->insert_at(this->size(), other);
		}
		/// @brief Adds all elements from another Container at the end of this one.
		/// @param[in] other Container of elements to add.
		/// @param[in] count Number of elements to add.
		inline void addLast(const Container& other, const int count)
		{
			this->insert_at(this->size(), other, count);
		}
		/// @brief Adds all elements from another Container at the end of this one.
		/// @param[in] other Container of elements to add.
		/// @param[in] start Start index of the elements to add.
		/// @param[in] count Number of elements to add.
		inline void addLast(const Container& other, const int start, const int count)
		{
			this->insert_at(this->size(), other, start, count);
		}
		/// @brief Adds all elements from a C-type array at the end of Container.
		/// @param[in] other C-type array of elements to add.
		/// @param[in] count Number of elements to add.
		inline void addLast(const T other[], const int count)
		{
			this->insert_at(this->size(), other, count);
		}
		/// @brief Adds all elements from a C-type array at the end of Container.
		/// @param[in] other C-type array of elements to add.
		/// @param[in] start Start index of the elements to add.
		/// @param[in] count Number of elements to add.
		inline void addLast(const T other[], const int start, const int count)
		{
			this->insert_at(this->size(), other, start, count);
		}
		/// @brief Adds element at the beginning of Container n times.
		/// @param[in] element Element to add.
		/// @param[in] times Number of times to add the element.
		inline void addFirst(const T& element, int times = 1)
		{
			this->insert_at(0, element, times);
		}
		/// @brief Adds all elements from another Container at the beginning of this one.
		/// @param[in] other Container of elements to add.
		inline void addFirst(const Container& other)
		{
			this->insert_at(0, other);
		}
		/// @brief Adds all elements from another Container at the beginning of this one.
		/// @param[in] other Container of elements to add.
		/// @param[in] count Number of elements to add.
		inline void addFirst(const Container& other, const int count)
		{
			this->insert_at(0, other, count);
		}
		/// @brief Adds all elements from another Container at the beginning of this one.
		/// @param[in] other Container of elements to add.
		/// @param[in] start Start index of the elements to add.
		/// @param[in] count Number of elements to add.
		inline void addFirst(const Container& other, const int start, const int count)
		{
			this->insert_at(0, other, start, count);
		}
		/// @brief Adds all elements from a C-type array at the beginning of Container.
		/// @param[in] other C-type array of elements to add.
		/// @param[in] count Number of elements to add.
		inline void addFirst(const T other[], const int count)
		{
			this->insert_at(0, other, count);
		}
		/// @brief Adds all elements from a C-type array at the beginning of Container.
		/// @param[in] other C-type array of elements to add.
		/// @param[in] start Start index of the elements to add.
		/// @param[in] count Number of elements to add.
		inline void addFirst(const T other[], const int start, const int count)
		{
			this->insert_at(0, other, start, count);
		}
		/// @brief Unites elements of this Array with an element.
		/// @param[in] element Element to unite with.
		inline void unite(const T& element)
		{
			this->insert_at(this->size(), element);
			this->remove_duplicates();
		}
		/// @brief Unites elements of this Array with another one.
		/// @param[in] other Array to unite with.
		inline void unite(const Container& other)
		{
			this->insert_at(this->size(), other);
			this->remove_duplicates();
		}
		/// @brief Reverses order of elements.
		inline void reverse()
		{
			if (this->size() > 0)
			{
				std::reverse(STD::begin(), STD::end());
			}
		}
		/// @brief Removes duplicates in Array.
		inline void remove_duplicates()
		{
			Container<std::vector<int>, int> indexes;
			iterator_t it = STD::begin();
			int indexesSize = 0;
			for_iter (i, 0, this->size()) // has to stay this->size() here
			{
				indexes = this->_indexesOf<Container<std::vector<int>, int> >(this->at(i));
				indexesSize = indexes.size();
				for_iter_r (j, indexesSize, 1)
				{
					STD::erase(this->_itAdvance(it, indexes.at(j)));
				}
			}
		}
		/// @brief Intersects elements of this Array with another one.
		/// @param[in] other Array to intersect with.
		inline void intersect(const Container& other)
		{
			Container result;
			int size = this->size();
			for_iter(i, 0, size)
			{
				if (other.contains(this->at(i)))
				{
					result.addLast(this->at(i));
				}
			}
			STD::assign(result.begin(), result.end());
		}
		/// @brief Differentiates elements of this Array with an element.
		/// @param[in] other Element to differentiate with.
		/// @note Unlike remove, this method ignores if the element is not in this Array.
		inline void differentiate(const T& element)
		{
			int index = 0;
			while (true)
			{
				index = this->index_of(element);
				if (index < 0)
				{
					break;
				}
				STD::erase(this->_itAdvance(STD::begin(), index));
			}
		}
		/// @brief Differentiates elements of this Array with another one.
		/// @param[in] other Array to differentiate with.
		/// @note Unlike remove, this method ignores elements of other Array that are not in this one.
		inline void differentiate(const Container& other)
		{
			int index = 0;
			int size = other.size();
			for_iter (i, 0, size)
			{
				while (true)
				{
					index = this->index_of(other.at(i));
					if (index < 0)
					{
						break;
					}
					STD::erase(this->_itAdvance(STD::begin(), index));
				}
			}
		}

		// DEPRECATED
		inline void push_front(const T& element, int times = 1)								{ this->addFirst(element, times); }
		inline void push_front(const Container& other)										{ this->addFirst(other); }
		inline void push_front(const Container& other, const int count)						{ this->addFirst(other, count); }
		inline void push_front(const Container& other, const int start, const int count)	{ this->addFirst(other, start, count); }
		inline void push_front(const T other[], const int count)							{ this->addFirst(other, count); }
		inline void push_front(const T other[], const int start, const int count)			{ this->addFirst(other, start, count); }
		inline void push_back(const T& element)												{ this->addLast(element); }
		inline void push_back(const T& element, int times)									{ this->addLast(element, times); }
		inline void push_back(const Container& other)										{ this->addLast(other); }
		inline void push_back(const Container& other, const int count)						{ this->addLast(other, count); }
		inline void push_back(const Container& other, const int start, const int count)		{ this->addLast(other, start, count); }
		inline void push_back(const T other[], const int count)								{ this->addLast(other, count); }
		inline void push_back(const T other[], const int start, const int count)			{ this->addLast(other, start, count); }
		inline void push_first(const T& element, int times = 1)								{ this->addFirst(element, times); }
		inline void push_first(const Container& other)										{ this->addFirst(other); }
		inline void push_first(const Container& other, const int count)						{ this->addFirst(other, count); }
		inline void push_first(const Container& other, const int start, const int count)	{ this->addFirst(other, start, count); }
		inline void push_first(const T other[], const int count)							{ this->addFirst(other, count); }
		inline void push_first(const T other[], const int start, const int count)			{ this->addFirst(other, start, count); }
		inline void push_last(const T& element)												{ this->addLast(element); }
		inline void push_last(const T& element, int times)									{ this->addLast(element, times); }
		inline void push_last(const Container& other)										{ this->addLast(other); }
		inline void push_last(const Container& other, const int count)						{ this->addLast(other, count); }
		inline void push_last(const Container& other, const int start, const int count)		{ this->addLast(other, start, count); }
		inline void push_last(const T other[], const int count)								{ this->addLast(other, count); }
		inline void push_last(const T other[], const int start, const int count)			{ this->addLast(other, start, count); }
		inline int remove_all(const T& element)												{ return this->removeAll(element); }
		inline int remove_all(const Container& other)										{ return this->removeAll(other); }

	protected:
		/// @brief Gets all indexes of the given element.
		/// @param[in] element Element to search for.
		/// @return All indexes of the given element.
		template <class D>
		inline D _indexesOf(T element) const
		{
			D result;
			int size = this->size();
			for_iter (i, 0, size)
			{
				if (element == this->at(i))
				{
					result.addLast(i);
				}
			}
			return result;
		}
		/// @brief Returns a subarray of elements.
		/// @param[in] start Start index of the elements to copy.
		/// @param[in] count Number of elements to copy.
		/// @return Subarray created from the current Container.
		template <class D>
		inline D _sub(int start, int count) const
		{
			D result;
			if (count > 0)
			{
				int size = this->size();
				if (start < 0)
				{
					start += size;
				}
				if (start < 0 || start >= size || start + count > size)
				{
					throw ContainerRangeException(start, count);
				}
				const_iterator_t it = this->_itAdvance(STD::begin(), start);
				result.assign(it, this->_itAdvance(it, count));
			}
			return result;
		}
		/// @brief Removes n elements at given index of Container.
		/// @param[in] index Start index of elements to remove.
		/// @param[in] count Number of elements to remove.
		/// @return Container of all removed elements.
		/// @note Elements in the returned Container are in the same order as in the orignal Container.
		template <class D>
		inline D _removeAt(int index, int count)
		{
			int size = this->size();
			if (index < 0)
			{
				index -= size;
			}
			if (index < 0 || index >= size || index + count > size)
			{
				throw ContainerRangeException(index, count);
			}
			D result;
			iterator_t it = STD::begin();
			iterator_t begin = this->_itAdvance(it, index);
			iterator_t end = this->_itAdvance(it, index + count);
			result.assign(begin, end);
			STD::erase(begin, end);
			return result;
		}
		/// @brief Removes n elements from the beginning of Array.
		/// @param[in] count Number of elements to remove.
		/// @return Array of all removed elements.
		/// @note Elements in the returned Array are in the same order as in the orignal Array.
		template <class D>
		inline D _removeFirst(const int count)
		{
			if (count > this->size())
			{
				throw ContainerRangeException(0, count);
			}
			D result;
			iterator_t begin = STD::begin();
			iterator_t end = this->_itAdvance(begin, count);
			result.assign(begin, end);
			STD::erase(begin, end);
			return result;
		}
		/// @brief Removes n elements from the end of Array.
		/// @param[in] count Number of elements to remove.
		/// @return Array of all removed elements.
		/// @note Elements in the returned Array are in the same order as in the orignal Array.
		template <class D>
		inline D _removeLast(const int count)
		{
			if (count > this->size())
			{
				throw ContainerRangeException(0, count);
			}
			D result;
			iterator_t end = STD::end();
			iterator_t begin = this->_itAdvance(end, -count);
			result.assign(begin, end);
			STD::erase(begin, end);
			return result;
		}
		/// @brief Creates new Array with reversed order of elements.
		/// @return A new Array.
		template <class D>
		inline D _reversed() const
		{
			D result(*this);
			result.reverse();
			return result;
		}
		/// @brief Creates new Array without duplicates.
		/// @return A new Array.
		template <class D>
		inline D _removedDuplicates() const
		{
			D result(*this);
			result.remove_duplicates();
			return result;
		}
		/// @brief Creates a new Array as union of this Array with an element.
		/// @param[in] element Element to unite with.
		/// @return A new Array.
		template <class D>
		inline D _united(const T& element) const
		{
			D result(*this);
			result.unite(element);
			return result;
		}
		/// @brief Creates a new Array as union of this Array with another one.
		/// @param[in] other Array to unite with.
		/// @return A new Array.
		template <class D>
		inline D _united(const D& other) const
		{
			D result(*this);
			result.unite(other);
			return result;
		}
		/// @brief Creates a new Array as intersection of this Array with another one.
		/// @param[in] other Array to intersect with.
		/// @return A new Array.
		template <class D>
		inline D _intersected(const D& other) const
		{
			D result(*this);
			result.intersect(other);
			return result;
		}
		/// @brief Creates a new Array as difference of this Array with an element.
		/// @param[in] other Element to differentiate with.
		/// @return A new Array.
		/// @note Unlike remove, this method ignores if the element is not in this Array.
		template <class D>
		inline D _differentiated(const T& element) const
		{
			D result(*this);
			result.differentiate(element);
			return result;
		}
		/// @brief Creates a new Array as difference of this Array with another one.
		/// @param[in] other Array to differentiate with.
		/// @return A new Array.
		/// @note Unlike remove, this method ignore elements of other Array that are not in this one.
		template <class D>
		inline D _differentiated(const D& other) const
		{
			D result(*this);
			result.differentiate(other);
			return result;
		}

	private:
		/// @brief Moves iterator forward/backward by a number of elements.
		/// @param[in] it Current iterator.
		/// @param[in] count Number of elements to move.
		/// @return Moved iterator.
		/// @note This is an internal utility function to make code easier to read.
		inline iterator_t _itAdvance(iterator_t it, int count)
		{
			std::advance(it, count);
			return it;
		}
		/// @brief Moves const iterator forward/backward by a number of elements.
		/// @param[in] it Current const iterator.
		/// @param[in] count Number of elements to move.
		/// @return Moved const iterator.
		/// @note This is an internal utility function to make code easier to read.
		inline const_iterator_t _itAdvance(const_iterator_t it, int count) const
		{
			std::advance(it, count);
			return it;
		}

	};
	
}

#endif
