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

/// @brief Provides a simpler syntax to iterate through the properties of a node.
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
		~Property();

		/// @return Name of the Property.
		hstr name();
		/// @return Value of the Property.
		hstr value();
		/// @brief Gets the next sibling Property in the current iteration.
		/// @return The next sibling Property of this Property or NULL if this is the last one.
		/// @note Use this only if you called iterProperties() on this Property's parent node previously to continue the iteration.
		/// @see Node::iterProperties()
		Property* next();

	protected:
		/// @brief The Node has this Property.
		Node* node;
		/// @brief The TinyXML property.
		TiXmlAttribute* prop;

		/// @brief Constructor.
		/// @param[in] node The Node this Property belongs to.
		/// @param[in] prop The TinyXML attribute.
		Property(Node* node, TiXmlAttribute* prop);

	};

}

#endif
