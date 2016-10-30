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
/// Provides high level system clipboard handling.

#ifndef HLTYPES_CLIPBOARD_H
#define HLTYPES_CLIPBOARD_H

#include "hstring.h"
#include "hltypesExport.h"

namespace hltypes
{
	/// @brief Provides high level system clipboard handling.
	class hltypesExport Clipboard
	{
	public:
		/// @brief Clears the cliboard contents.
		/// @brief True if successful.
		static bool clear();
		/// @brief Checks whether clipboard contains a string.
		/// @brief True if clipboard contains a string.
		static bool isString();
		/// @brief Gets the string from the clipboard.
		/// @param[out] string The result string.
		/// @brief True if successful.
		static bool getString(hstr& string);
		/// @brief Sets the string on the clipboard.
		/// @param[in] string The string to be set.
		/// @brief True if successful.
		static bool setString(chstr string);

	protected:
		/// @brief Basic constructor.
		/// @note Forces this to be a static class.
		inline Clipboard() { }
		/// @brief Basic constructor.
		/// @note Forces this to be a static class.
		inline ~Clipboard() { }

	};
}

/// @brief Alias for simpler code.
typedef hltypes::Clipboard hclipboard;

#endif

