/// @file
/// @version 4.0
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

#include "Exception.h"
#include "hlxmlExport.h"

/// @brief Provides a simpler syntax to iterate through the nodes of a document.
#define foreach_xmlnode(nodeName, rootName) for (hlxml::Node* nodeName = rootName->iterChildren(); nodeName != NULL; nodeName = nodeName->next())

namespace hlxml
{
	class Document;

	/// @brief Represents an XML node.
	class hlxmlExport Node
	{
	public:
		friend class Document;

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

		/// @brief Name of the Node.
		hstr name;
		/// @brief Value of the Node.
		hstr value;
		/// @brief Type of the Node.
		Type type;
		/// @brief Properties within the Node.
		hmap<hstr, hstr> properties;

		/// @brief Gets the filename of the Document to which this Node belongs to.
		hstr getFilename();
		/// @brief Gets the line where the Node is located in the Document.
		int getLine();
		/// @brief Gets the child count.
		int getChildCount();

		/// @brief Gets a given property value as bool.
		/// @param[in] propertyName Name of the property.
		/// @return Bool value of the property.
		/// @note Throws an exception if not found.
		inline bool pbool(chstr propertyName)
		{
			if (!this->properties.hasKey(propertyName))
			{
				throw XMLPropertyNotExistsException(propertyName, this);
			}
			return (bool)this->properties[propertyName];
		}
		/// @brief Gets a given property value as bool.
		/// @param[in] propertyName Name of the property.
		/// @param[in] defaultValue Default value to return if property does not exist.
		/// @return Bool value of the property.
		inline bool pbool(chstr propertyName, bool defaultValue)
		{
			return (this->properties.hasKey(propertyName) ? (bool)this->properties[propertyName] : defaultValue);
		}
		/// @brief Gets a given property value as int.
		/// @param[in] propertyName Name of the property.
		/// @return Int value of the property.
		/// @note Throws an exception if not found.
		inline int pint(chstr propertyName)
		{
			if (!this->properties.hasKey(propertyName))
			{
				throw XMLPropertyNotExistsException(propertyName, this);
			}
			return (int)this->properties[propertyName];
		}
		/// @brief Gets a given property value as int.
		/// @param[in] propertyName Name of the property.
		/// @param[in] defaultValue Default value to return if property does not exist.
		/// @return Int value of the property.
		inline int pint(chstr propertyName, int defaultValue)
		{
			return (this->properties.hasKey(propertyName) ? (int)this->properties[propertyName] : defaultValue);
		}
		/// @brief Gets a given property value as float.
		/// @param[in] propertyName Name of the property.
		/// @return Float value of the property.
		/// @note Throws an exception if not found.
		inline float pfloat(chstr propertyName)
		{
			if (!this->properties.hasKey(propertyName))
			{
				throw XMLPropertyNotExistsException(propertyName, this);
			}
			return (float)this->properties[propertyName];
		}
		/// @brief Gets a given property value as float.
		/// @param[in] propertyName Name of the property.
		/// @param[in] defaultValue Default value to return if property does not exist.
		/// @return Float value of the property.
		inline float pfloat(chstr propertyName, float defaultValue)
		{
			return (this->properties.hasKey(propertyName) ? (float)this->properties[propertyName] : defaultValue);
		}
		/// @brief Gets a given property value as String.
		/// @param[in] propertyName Name of the property.
		/// @return String value of the property.
		/// @note Throws an exception if not found.
		inline hstr pstr(chstr propertyName)
		{
			if (!this->properties.hasKey(propertyName))
			{
				throw XMLPropertyNotExistsException(propertyName, this);
			}
			return this->properties[propertyName];
		}
		/// @brief Gets a given property value as String.
		/// @param[in] propertyName Name of the property.
		/// @param[in] defaultValue Default value to return if property does not exist.
		/// @return String value of the property.
		inline hstr pstr(chstr propertyName, chstr defaultValue)
		{
			return this->properties.tryGet(propertyName, defaultValue);
		}

		/// @brief Checks if a property exists.
		/// @param[in] propertyName Name of the property to check.
		/// @return True if the property exists.
		inline bool pexists(chstr propertyName)
		{
			return this->properties.hasKey(propertyName);
		}

		/// @brief Gets the next sibling Node in the current iteration.
		/// @return The next sibling Node of this Node or NULL if this is the last one.
		/// @note Use this only if you called iterChildren() on this Node's parent previously to continue the iteration.
		/// @see iterChildren()
		Node* next();
		/// @brief Starts an iteration over all child Nodes.
		/// @return The first child Node of this Node.
		Node* iterChildren();

	protected:
		/// @brief The document this Node belongs to.
		Document* document;
		/// @brief The Node of the underlying system.
		void* node;
		/// @brief Contains the child count.
		int childCount;

		/// @brief Constructor.
		/// @param[in] document The document this Node belongs to.
		/// @param[in] Node The TinyXML Node.
		Node(Document* document, void* node);

	};

}

#endif
