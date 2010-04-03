/************************************************************************************\
* This source file is part of the High Level C++ types library                       *
* For latest info, see http://libhltypes.sourceforge.net/                            *
**************************************************************************************
* Copyright (c) 2010 Kresimir Spes, Boris Mikic, Domagoj Cerjan                      *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
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
//	memcpy(this->data + (this->element_size * this->len++), input, this->element_size);
}

void hltypes::_Array::_getElement(int index, void* output)
{
    //memcpy(output, this->data + (this->element_size * index), this->element_size);
}

void hltypes::_Array::_setElement(int index, void* input)
{
}

