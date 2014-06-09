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
/// Represents an XML node's property.

#ifndef HLXML_PROPERTY_H
#define HLXML_PROPERTY_H

#include <hltypes/hstring.h>

#include "hlxmlExport.h"

#define foreach_xmlproperty(propertyName, nodeName) for (hlxml::Property* propertyName = nodeName->iterProperties(); propertyName != NULL; propertyName = propertyName->next())

class TiXmlAttribute;

namespace hlxml
{
	class Node;

	/// @brief Represents an XML node's property.
	class hlxmlExport Property
	{
	public:
		friend class Node;

		/// @brief Destructor.
		virtual ~Property();

		/// @return Name of the Property.
		hstr name();
		/// @return Value of the Property.
		hstr value();
		/// @return The next sibling Property within the Node.
		/// @note This returns NULL if there are no more Properties.
		Property* next();

	protected:
		/// @brief The Node has this Property.
		Node* node;
		/// @brief The TinyXML property.
		TiXmlAttribute* prop;

		/// @brief Constructor.
		/// @param[in] node The Node has this Property.
		/// @param[in] prop The TinyXML property.
		Property(Node* node, TiXmlAttribute* prop);

	};

}

#endif
