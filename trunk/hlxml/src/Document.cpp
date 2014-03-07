/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/hresource.h>
#include <hltypes/exception.h>

#include "Document.h"
#include "Exception.h"
#include "TinyXml_Document.h"

namespace hlxml
{
	hstr logTag = "hlxml";

	Document* open(chstr filename)
	{
		return new TinyXml_Document(filename);
	}

	void close(Document* document)
	{
		delete document;
	}

	Document::Document(chstr filename)
	{
		this->filename = filename;
		if (!hresource::exists(this->filename))
		{
			throw resource_not_found(this->filename);
		}
	}

	Document::~Document()
	{
	}

}
