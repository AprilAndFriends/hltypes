/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Represents a generic XML document.

#ifndef HLXML_DOCUMENT_H
#define HLXML_DOCUMENT_H

#include <hltypes/hstring.h>

#include "hlxmlExport.h"

namespace hlxml
{
	extern hstr logTag;

	class Document;
	class Node;
	
	hlxmlFnExport Document* open(chstr filename);
	hlxmlFnExport void close(Document* document);

	class hlxmlExport Document
	{
	public:
		virtual ~Document();

		hstr getFilename() { return this->filename; }

		virtual Node* root(chstr type = "") = 0;

	protected:
		hstr filename;
	
		Document(chstr filename);

	};
}

#endif
