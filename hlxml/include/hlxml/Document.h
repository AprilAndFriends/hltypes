/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Represents an XML document.

#ifndef HLXML_DOCUMENT_H
#define HLXML_DOCUMENT_H

#include <libxml/xmlmemory.h>

#include <hltypes/hstring.h>

#include "hlxmlExport.h"

namespace hlxml
{
	struct Node;

	struct hlxmlExport Document
	{
	public:
		Document(chstr filename);
		~Document();
		Node* root(chstr rootElementQuery = "");
	
	protected:
		xmlDocPtr xmlDocument;
		Node* rootNode;
	
	};

}

#endif
