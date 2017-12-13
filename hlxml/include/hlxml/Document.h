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
	/// @brief Used for logging display.
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

		/// @brief The filename.
		HL_DEFINE_GET(hstr, filename, Filename);

		/// @brief Gets the root Node of the Document.
		/// @param[in] name Optional node name check.
		/// @note Throws an exception if type is not an empty String and the root Node is not of that type.
		Node* root(chstr name = "");

	protected:
		/// @brief Raw data of the Document.
		char* data;
		/// @brief Line number of the Document.
		int line;
		/// @brief Filename of the Document.
		hstr filename;
		/// @brief Real filename of the Document, used for exception prints.
		hstr realFilename;
		/// @brief Whether data is read from a resource or not.
		bool fromResource;
		/// @brief The underlying document object.
		void* document;
		/// @brief The root Node of the Document.
		Node* rootNode;

		/// @brief Sets up data for later parsing.
		/// @param[in] stream XML data stream.
		/// @param[in] realFilename The logical filename (used for error printing).
		void _setup(hsbase& stream, chstr realFilename);
		/// @brief Parses the XML.
		void _parse();

	};
}

#endif
