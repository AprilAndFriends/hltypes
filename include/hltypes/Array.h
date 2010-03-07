#ifndef HLTYPES_ARRAY_H
#define HLTYPES_ARRAY_H

namespace hltypes
{
	class _Array
	{
	protected:
		unsigned char* data;
		int element_size,len;
		
		void _getElement(int index,void* output);
		void _setElement(int index,void* input);
		void _appendElement(void* input);
	public:
		_Array(unsigned int element_size);
		~_Array();
		
	};

	template <class T>
	class Array : public _Array
	{
	public:
		Array() : _Array(sizeof(T)) { }
		
		T operator [](int index)
		{
			T element;
			_getElement(index,&element);
			return element;
		}
		
		void append(T element)
		{
			_appendElement(&element);
		}
	};
}


#endif
