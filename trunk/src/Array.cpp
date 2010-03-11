#include <memory.h>
#include "Array.h"

hltypes::_Array::_Array(unsigned int element_size)
{
	this->element_size = element_size;
	this->data = new unsigned char[this->element_size * 100];
	this->len = 0;
}

hltypes::_Array::~_Array()
{
	delete [] this->data;
}

void hltypes::_Array::_appendElement(void* input)
{
	memcpy(this->data + (this->element_size * this->len++), input, this->element_size);
}

void hltypes::_Array::_getElement(int index, void* output)
{
    memcpy(output, this->data + (this->element_size * index), this->element_size);
}

void hltypes::_Array::_setElement(int index, void* input)
{
}

