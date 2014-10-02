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
/// Represents an XML node.

#ifndef HLXML_NODE_H
#define HLXML_NODE_H

#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "hlxmlExport.h"

class TiXmlNode;
class TiXmlAttribute;

/// @brief Provides a simpler syntax to iterate through the nodes of a document.
#define foreach_xmlnode(nodeName, rootName) for (hlxml::Node* nodeName = rootName->iterChildren(); nodeName != NULL; nodeName = nodeName->next())

namespace hlxml
{
	class Document;
	class Property;

	/// @brief Represents an XML node.
	class hlxmlExport Node
	{
	public:
		friend class Document;
		friend class Property;

		/// @brief Type of the Node.
		enum Type
		{
			/// @brief Normal node.
			TYPE_ELEMENT,
			/// @brief Comment node.
			TYPE_COMMENT,
			/// @brief Text node.
			TYPE_TEXT
		};

		/// @brief Destructor.
		~Node();

		/// @brief Gets the filename of the Document to which this Node belongs to.
		hstr getFilename();
		/// @brief Gets the line where the Node is located in the Document.
		int getLine();
		/// @brief Gets the type of the Node.
		Type getType();

		/// @brief Gets a given property value as bool.
		/// @param[in] propertyName Name of the property.
		/// @return Bool value of the property.
		/// @note Throws an exception if not found.
		bool pbool(chstr propertyName);
		/// @brief Gets a given property value as bool.
		/// @param[in] propertyName Name of the property.
		/// @param[in] defaultValue Default value to return if property does not exist.
		/// @return Bool value of the property.
		bool pbool(chstr propertyName, bool defaultValue);
		/// @brief Gets a given property value as int.
		/// @param[in] propertyName Name of the property.
		/// @return Int value of the property.
		/// @note Throws an exception if not found.
		int pint(chstr propertyName);
		/// @brief Gets a given property value as int.
		/// @param[in] propertyName Name of the property.
		/// @param[in] defaultValue Default value to return if property does not exist.
		/// @return Int value of the property.
		int pint(chstr propertyName, int defaultValue);
		/// @brief Gets a given property value as float.
		/// @param[in] propertyName Name of the property.
		/// @return Float value of the property.
		/// @note Throws an exception if not found.
		float pfloat(chstr propertyName);
		/// @brief Gets a given property value as float.
		/// @param[in] propertyName Name of the property.
		/// @param[in] defaultValue Default value to return if property does not exist.
		/// @return Float value of the property.
		float pfloat(chstr propertyName, float defaultValue);
		/// @brief Gets a given property value as String.
		/// @param[in] propertyName Name of the property.
		/// @return String value of the property.
		/// @note Throws an exception if not found.
		hstr pstr(chstr propertyName);
		/// @brief Gets a given property value as String.
		/// @param[in] propertyName Name of the property.
		/// @param[in] defaultValue Default value to return if property does not exist.
		/// @return String value of the property.
		hstr pstr(chstr propertyName, chstr defaultValue);
		
		/// @brief Checks if a property exists.
		/// @param[in] propertyName Name of the property to check.
		/// @return True if the property exists.
		bool pexists(chstr propertyName);
		
		/// @brief Gets the next sibling Node in the current iteration.
		/// @return The next sibling Node of this Node or NULL if this is the last one.
		/// @note Use this only if you called iterChildren() on this Node's parent previously to continue the iteration.
		/// @see iterChildren()
		Node* next();
		/// @brief Starts an iteration over all child Nodes.
		/// @return The first child Node of this Node.
		Node* iterChildren();
		/// @brief Starts an iteration over all properties.
		/// @return The first Property of this Node.
		Property* iterProperties();

		/// @brief Checks whether Node's name corresponds to a given name.
		bool operator==(const char* name);
		/// @brief Checks whether Node's name corresponds to a given name.
		bool operator!=(const char* name);
		/// @brief Checks whether Node's name corresponds to a given name.
		bool operator==(chstr name);
		/// @brief Checks whether Node's name corresponds to a given name.
		bool operator!=(chstr name);
		
	protected:
		/// @brief The document this Node belongs to.
		Document* document;
		/// @brief The Node of the underlying system.
		TiXmlNode* node;
		/// @brief All properties.
		hmap<TiXmlAttribute*, Property*> props;

		/// @brief Constructor.
		/// @param[in] document The document this Node belongs to.
		/// @param[in] Node The TinyXML Node.
		Node(Document* document, TiXmlNode* node);

		/// @brief Gets the Property associated with the TinyXML atrribute.
		/// @param[in] prop The TinyXML atrribute.
		/// @return The Property associated with the TinyXML atrribute.
		Property* _prop(TiXmlAttribute* prop);
		/// @brief Gets the value (as C-string) for a Property name.
		/// @param[in] propertyName Name of the Property.
		/// @param[in] ignoreError Whether to ignore any errors.
		/// @return The value (as C-string) for a property name.
		const char* _findProperty(chstr propertyName, bool ignoreError = false);

	};

}

#endif
