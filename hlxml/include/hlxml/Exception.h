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
/// Defines exceptions.

#ifndef HLXML_EXCEPTION_H
#define HLXML_EXCEPTION_H

#include <hltypes/hexception.h>
#include <hltypes/hstring.h>

#include "hlxmlExport.h"

namespace hlxml
{
	class Node;

	/// @brief Defines a generic XML exception.
	class hlxmlExport _XMLException : public hexception
	{
	public:
		/// @brief Basic constructor.
		/// @param[in] message Error message.
		/// @param[in] node The affected Node.
		/// @param[in] source_file Name of the source file.
		/// @param[in] line_number Number of the line.
		_XMLException(chstr message, Node* node, const char* file, int line);
		/// @brief Gets the exception type.
		/// @return The exception type.
		inline hstr getType() const { return "XMLException"; }

	};

	/// @brief Alias for simpler code.
	#define XMLException(message, node) hlxml::_XMLException(message, node, __FILE__, __LINE__)
	/// @brief Alias for simpler code.
	#define XMLPropertyNotExistsException(element, node) hlxml::_XMLException(hstr("XML property doesn't exist: ") + element, node, __FILE__, __LINE__)
	/// @brief Alias for simpler code.
	#define XMLUnknownClassException(element, node) hlxml::_XMLException(hstr("Unknown class detected in XML file: ") + element, node, __FILE__, __LINE__)

}

#endif
