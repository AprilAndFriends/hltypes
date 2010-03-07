#include <memory.h>
#include "Array.h"


hltypes::_Array::_Array(unsigned int element_size)
{
	this->element_size=element_size;
	data=new unsigned char[this->element_size * 100];
	len=0;
}

hltypes::_Array::~_Array()
{
	delete [] data;
}

void hltypes::_Array::_appendElement(void* input)
{
	memcpy(data+(this->element_size * this->len++),input,this->element_size);
}

void hltypes::_Array::_getElement(int index, void* output)
{
    memcpy(output,data+(this->element_size * index),this->element_size);
}

void hltypes::_Array::_setElement(int index, void* input)
{
}

