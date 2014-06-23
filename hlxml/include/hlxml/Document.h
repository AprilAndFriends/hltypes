/// @file
/// @version 3.0
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
#include <hltypes/hmap.h>

#include "hlxmlExport.h"

class TiXmlDocument;
class TiXmlNode;

namespace hlxml
{
	extern hstr logTag;

	class Node;
	
	class hlxmlExport Document
	{
	public:
		friend class Node;

		Document(chstr filename);
		virtual ~Document();

		HL_DEFINE_GET(hstr, filename, Filename);

		Node* root(chstr type = "");

	protected:
		TiXmlDocument* document;
		hstr filename;
		Node* rootNode;
		hmap<TiXmlNode*, Node*> nodes;

		Node* _node(TiXmlNode* node);

	};
}

#endif
