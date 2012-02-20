/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/hresource.h>

#include "Document.h"
#include "Exception.h"
#ifdef USE_LIBXML2
#include "LibXml2_Document.h"
#endif
#ifdef USE_TINYXML
#include "TinyXml_Document.h"
#endif

namespace hlxml
{
	Document* open(chstr filename)
	{
#ifdef USE_LIBXML2
		return new LibXml2_Document(filename);
#endif
#ifdef USE_TINYXML
		return new TinyXml_Document(filename);
#endif
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
			throw XMLException("Unable to parse xml file '" + this->filename + "', document does not exist", NULL);
		}
	}

	Document::~Document()
	{
	}

}
