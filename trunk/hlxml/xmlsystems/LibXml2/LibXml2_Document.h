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
/// Represents a LibXML2 document.

#ifdef USE_LIBXML2
#ifndef HLXML_LIBXML2_DOCUMENT_H
#define HLXML_LIBXML2_DOCUMENT_H

struct _xmlDoc;
struct _xmlNode;

#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "Document.h"
#include "hlxmlExport.h"

namespace hlxml
{
	class LibXml2_Node;

	class hlxmlExport LibXml2_Document : public Document
	{
	public:
		LibXml2_Document(chstr filename);
		~LibXml2_Document();

		Node* root(chstr type = "");

		LibXml2_Node* node(_xmlNode* node);

	protected:
		_xmlDoc* document;
		LibXml2_Node* rootNode;
		hmap<_xmlNode*, LibXml2_Node*> nodes;
		
	};

}

#endif
#endif