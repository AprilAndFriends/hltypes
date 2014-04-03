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
/// Represents a generic XML node.

#ifndef HLXML_NODE_H
#define HLXML_NODE_H

#include <hltypes/hstring.h>

#include "hlxmlExport.h"

#define foreach_xmlnode(nodeName, rootName) for (hlxml::Node* nodeName = rootName->iterChildren(); nodeName != NULL; nodeName = nodeName->next())

namespace hlxml
{
	class Property;

	class hlxmlExport Node
	{
	public:
		enum Type
		{
			// TODO
			TYPE_ELEMENT,
			TYPE_COMMENT,
			TYPE_TEXT
		};

		Node();
		virtual ~Node();

		virtual hstr getFilename() = 0;
		virtual int getLine() = 0;
		virtual Type getType() = 0;

		bool pbool(chstr propertyName);
		bool pbool(chstr propertyName, bool defaultValue);
		int pint(chstr propertyName);
		int pint(chstr propertyName, int defaultValue);
		float pfloat(chstr propertyName);
		float pfloat(chstr propertyName, float defaultValue);
		hstr pstr(chstr propertyName);
		hstr pstr(chstr propertyName, chstr defaultValue);
		
		bool pexists(chstr propertyName);
		
		virtual void setProperty(chstr name, chstr value) = 0;
		virtual Node* next() = 0;
		virtual Node* iterChildren() = 0;
		virtual Property* iterProperties() = 0;
	
		bool operator==(const char* name);
		bool operator!=(const char* name);
		bool operator==(chstr name);
		bool operator!=(chstr name);
		
	protected:
		virtual const char* _findProperty(chstr propertyName, bool ignoreError = false);
		virtual bool _equals(const char* name) = 0;
	
	};

}

#endif
