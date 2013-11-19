/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Represents a TinyXML document.

#ifndef HLXML_TINYXML_DOCUMENT_H
#define HLXML_TINYXML_DOCUMENT_H

class TiXmlDocument;
class TiXmlNode;

#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "Document.h"
#include "hlxmlExport.h"

namespace hlxml
{
	class TinyXml_Node;

	class hlxmlExport TinyXml_Document : public Document
	{
	public:
		TinyXml_Document(chstr filename);
		~TinyXml_Document();

		Node* root(chstr type = "");

		TinyXml_Node* node(TiXmlNode* node);

	protected:
		TiXmlDocument* document;
		TinyXml_Node* rootNode;
		hmap<TiXmlNode*, TinyXml_Node*> nodes;
		
	};

}

#endif
