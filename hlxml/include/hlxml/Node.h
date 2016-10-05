/// @file
/// @version 4.2
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
#define foreach_xmlnode(nodeName, rootName) foreach (hlxml::Node*, nodeName, rootName->children)

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
		/// @brief Filename when the Node's Document.
		hstr filename;
		/// @brief Line number where the node is located in the Document.
		int line;
		/// @brief Properties within the Node.
		/// @note Exposed for optimized access.
		hmap<hstr, hstr> properties;
		/// @brief Children of this Node.
		/// @note Exposed for optimized access.
		harray<Node*> children;

		/// @brief Gets the line where the Node is located in the Document.
		int getLine() const;

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
		/// @brief Gets a given property value as int8.
		/// @param[in] propertyName Name of the property.
		/// @return Int8 value of the property.
		/// @note Throws an exception if not found.
		inline char pint8(chstr propertyName)
		{
			if (!this->properties.hasKey(propertyName))
			{
				throw XMLPropertyNotExistsException(propertyName, this);
			}
			return (char)(int)this->properties[propertyName];
		}
		/// @brief Gets a given property value as int8.
		/// @param[in] propertyName Name of the property.
		/// @param[in] defaultValue Default value to return if property does not exist.
		/// @return Int8 value of the property.
		inline char pint8(chstr propertyName, char defaultValue)
		{
			return (this->properties.hasKey(propertyName) ? (char)(int)this->properties[propertyName] : defaultValue);
		}
		/// @brief Gets a given property value as uint8.
		/// @param[in] propertyName Name of the property.
		/// @return Uint8 value of the property.
		/// @note Throws an exception if not found.
		inline unsigned char puint8(chstr propertyName)
		{
			if (!this->properties.hasKey(propertyName))
			{
				throw XMLPropertyNotExistsException(propertyName, this);
			}
			return (unsigned char)(int)this->properties[propertyName];
		}
		/// @brief Gets a given property value as uint8.
		/// @param[in] propertyName Name of the property.
		/// @param[in] defaultValue Default value to return if property does not exist.
		/// @return Uint8 value of the property.
		inline unsigned char puint8(chstr propertyName, unsigned char defaultValue)
		{
			return (this->properties.hasKey(propertyName) ? (unsigned char)(int)this->properties[propertyName] : defaultValue);
		}
		/// @brief Gets a given property value as int16.
		/// @param[in] propertyName Name of the property.
		/// @return Int16 value of the property.
		/// @note Throws an exception if not found.
		inline short pint16(chstr propertyName)
		{
			if (!this->properties.hasKey(propertyName))
			{
				throw XMLPropertyNotExistsException(propertyName, this);
			}
			return (short)this->properties[propertyName];
		}
		/// @brief Gets a given property value as int16.
		/// @param[in] propertyName Name of the property.
		/// @param[in] defaultValue Default value to return if property does not exist.
		/// @return Int16 value of the property.
		inline short pint16(chstr propertyName, short defaultValue)
		{
			return (this->properties.hasKey(propertyName) ? (short)this->properties[propertyName] : defaultValue);
		}
		/// @brief Gets a given property value as uint16.
		/// @param[in] propertyName Name of the property.
		/// @return Uint16 value of the property.
		/// @note Throws an exception if not found.
		inline unsigned short puint16(chstr propertyName)
		{
			if (!this->properties.hasKey(propertyName))
			{
				throw XMLPropertyNotExistsException(propertyName, this);
			}
			return (unsigned short)this->properties[propertyName];
		}
		/// @brief Gets a given property value as uint16.
		/// @param[in] propertyName Name of the property.
		/// @param[in] defaultValue Default value to return if property does not exist.
		/// @return Uint16 value of the property.
		inline unsigned short puint16(chstr propertyName, unsigned short defaultValue)
		{
			return (this->properties.hasKey(propertyName) ? (unsigned short)this->properties[propertyName] : defaultValue);
		}
		/// @brief Gets a given property value as int32.
		/// @param[in] propertyName Name of the property.
		/// @return Int32 value of the property.
		/// @note Throws an exception if not found.
		inline int pint32(chstr propertyName)
		{
			if (!this->properties.hasKey(propertyName))
			{
				throw XMLPropertyNotExistsException(propertyName, this);
			}
			return (int)this->properties[propertyName];
		}
		/// @brief Gets a given property value as int32.
		/// @param[in] propertyName Name of the property.
		/// @param[in] defaultValue Default value to return if property does not exist.
		/// @return Int32 value of the property.
		inline int pint32(chstr propertyName, int defaultValue)
		{
			return (this->properties.hasKey(propertyName) ? (int)this->properties[propertyName] : defaultValue);
		}
		/// @brief Gets a given property value as uint32.
		/// @param[in] propertyName Name of the property.
		/// @return Uint32 value of the property.
		/// @note Throws an exception if not found.
		inline unsigned int puint32(chstr propertyName)
		{
			if (!this->properties.hasKey(propertyName))
			{
				throw XMLPropertyNotExistsException(propertyName, this);
			}
			return (unsigned int)this->properties[propertyName];
		}
		/// @brief Gets a given property value as uint32.
		/// @param[in] propertyName Name of the property.
		/// @param[in] defaultValue Default value to return if property does not exist.
		/// @return Uint32 value of the property.
		inline unsigned int puint32(chstr propertyName, unsigned int defaultValue)
		{
			return (this->properties.hasKey(propertyName) ? (unsigned int)this->properties[propertyName] : defaultValue);
		}
		/// @brief Gets a given property value as int64.
		/// @param[in] propertyName Name of the property.
		/// @return Int64 value of the property.
		/// @note Throws an exception if not found.
		inline int64_t pint64(chstr propertyName)
		{
			if (!this->properties.hasKey(propertyName))
			{
				throw XMLPropertyNotExistsException(propertyName, this);
			}
			return (int64_t)this->properties[propertyName];
		}
		/// @brief Gets a given property value as int64.
		/// @param[in] propertyName Name of the property.
		/// @param[in] defaultValue Default value to return if property does not exist.
		/// @return Int64 value of the property.
		inline int64_t pint64(chstr propertyName, int64_t defaultValue)
		{
			return (this->properties.hasKey(propertyName) ? (int64_t)this->properties[propertyName] : defaultValue);
		}
		/// @brief Gets a given property value as uint64.
		/// @param[in] propertyName Name of the property.
		/// @return Uint64 value of the property.
		/// @note Throws an exception if not found.
		inline uint64_t puint64(chstr propertyName)
		{
			if (!this->properties.hasKey(propertyName))
			{
				throw XMLPropertyNotExistsException(propertyName, this);
			}
			return (uint64_t)this->properties[propertyName];
		}
		/// @brief Gets a given property value as uint64.
		/// @param[in] propertyName Name of the property.
		/// @param[in] defaultValue Default value to return if property does not exist.
		/// @return Uint64 value of the property.
		inline uint64_t puint64(chstr propertyName, uint64_t defaultValue)
		{
			return (this->properties.hasKey(propertyName) ? (uint64_t)this->properties[propertyName] : defaultValue);
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
		/// @brief Gets a given property value as double.
		/// @param[in] propertyName Name of the property.
		/// @return Double value of the property.
		/// @note Throws an exception if not found.
		inline double pdouble(chstr propertyName)
		{
			if (!this->properties.hasKey(propertyName))
			{
				throw XMLPropertyNotExistsException(propertyName, this);
			}
			return (double)this->properties[propertyName];
		}
		/// @brief Gets a given property value as double.
		/// @param[in] propertyName Name of the property.
		/// @param[in] defaultValue Default value to return if property does not exist.
		/// @return Double value of the property.
		inline double pdouble(chstr propertyName, double defaultValue)
		{
			return (this->properties.hasKey(propertyName) ? (double)this->properties[propertyName] : defaultValue);
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

	protected:
		/// @brief Constructor.
		/// @param[in] node The underlying node object.
		Node(Document* document, void* node);
		/// @brief Destructor.
		~Node();

	};

}

#endif
