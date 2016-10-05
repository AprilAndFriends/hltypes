/// @file
/// @version 4.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Represents an XML document.

#ifndef HLXML_DOCUMENT_H
#define HLXML_DOCUMENT_H

#include <hltypes/hltypesUtil.h>
#include <hltypes/hsbase.h>
#include <hltypes/hstring.h>
#include <hltypes/hmap.h>

#include "hlxmlExport.h"
#include "Node.h"

namespace hlxml
{
	hlxmlExport extern hstr logTag;

	class Node;
	
	/// @brief Represents an XML document.
	class hlxmlExport Document
	{
	public:
		friend class Node;

		/// @brief Constructor
		/// @param[in] filename Filename where to read from.
		/// @param[in] fromResource Whether the file is a resource or not.
		Document(chstr filename, bool fromResource = true);
		/// @brief Constructor
		/// @param[in] stream Stream where to read from.
		Document(hsbase& stream);
		/// @brief Destructor
		~Document();

		HL_DEFINE_GET(hstr, filename, Filename);

		/// @brief Gets the root Node of the Document.
		/// @param[in] name Optional node name check.
		/// @note Throws an exception if type is not an empty String and the root Node is not of that type.
		Node* root(chstr name = "");

	protected:
		/// @brief The TinyXML document.
		void* document;
		/// @brief Filename of the Document.
		hstr filename;
		/// @brief Real filename of the Document, used for exception prints.
		hstr realFilename;
		/// @brief Whether data is read from a resource or not.
		bool fromResource;
		/// @brief Raw data of the Document.
		char* data;
		/// @brief The root Node of the Document.
		Node* rootNode;
		/// @brief A list of all TinyXML nodes and their associated Nodes.
		hmap<void*, Node*> nodes;

		/// @brief Sets up data for later parsing.
		/// @param[in] data XML data.
		/// @param[in] realFilename The logical filename (used for error printing).
		void _setup(hsbase& stream, chstr realFilename);
		/// @brief Parses the XML.
		void _parse();
		/// @brief Gets the Node associated with the TinyXML node.
		/// @param[in] node The TinyXML node.
		/// @return The Node associated with the TinyXML node.
		inline Node* _node(void* node)
		{
			if (node == NULL)
			{
				return NULL;
			}
			Node* newNode = this->nodes.tryGet(node, NULL);
			if (newNode == NULL)
			{
				newNode = new Node(this, node);
				this->nodes[node] = newNode;
			}
			return newNode;
		}

	};
}

#endif
