/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Represents a generic XML document.

#ifndef HLXML_DOCUMENT_H
#define HLXML_DOCUMENT_H

#include <hltypes/hltypesUtil.h>
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

		HL_DEFINE_GET(hstr, filename, Filename);

		virtual Node* root(chstr type = "") = 0;

	protected:
		hstr filename;
	
		Document(chstr filename);

	};
}

#endif
