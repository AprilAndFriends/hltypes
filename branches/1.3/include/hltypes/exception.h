/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Provides high level exceptions.

#ifndef HLTYPES_EXCEPTION_H
#define HLTYPES_EXCEPTION_H

#include "hstring.h"

#include "hltypesExport.h"

namespace hltypes
{
	/// @brief Provides functionality of a basic exception.
	class hltypesExport exception
	{
	public:
		/// @brief Basic constructor.
		/// @param[in] message Exception message.
		/// @param[in] source_file Name of the source file.
		/// @param[in] line Number of the line.
		exception(chstr message, const char* source_file, int line);
		/// @brief Destructor.
		virtual ~exception();
		/// @brief Gets the exception message.
		/// @return The exception message.
		virtual hstr message() { return this->msg; }
		/// @brief Same as message.
		/// @see message
		virtual hstr getErrorText() { return message(); }
		
	protected:
		/// @brief Exception message.
		hstr msg;
		
	};
	/// @brief Alias for simpler code.
	#define hl_exception(msg) hltypes::exception(msg, __FILE__, __LINE__)
	
	/// @brief Defines a file-not-found exception.
	class hltypesExport _file_not_found : public exception
	{
	public:
		/// @brief Basic constructor.
		/// @param[in] filename Name of the file.
		/// @param[in] source_file Name of the source file.
		/// @param[in] line Number of the line.
		_file_not_found(chstr filename, const char* source_file, int line);
		
	};
	/// @brief Alias for simpler code.
	#define file_not_found(filename) hltypes::_file_not_found(filename, __FILE__, __LINE__)
	
	/// @brief Defines a file-not-open exception.
	class hltypesExport _file_not_open : public exception
	{
	public:
		/// @brief Basic constructor.
		/// @param[in] filename Name of the file.
		/// @param[in] source_file Name of the source file.
		/// @param[in] line Number of the line.
		_file_not_open(chstr filename, const char* source_file, int line);
		
	};
	/// @brief Alias for simpler code.
	#define file_not_open(filename) hltypes::_file_not_open(filename, __FILE__, __LINE__)
	
	/// @brief Defines an index-error exception.
	class hltypesExport _index_error : public exception
	{
	public:
		/// @brief Basic constructor.
		/// @param[in] index Index of the entry.
		/// @param[in] source_file Name of the source file.
		/// @param[in] line Number of the line.
		_index_error(int index, const char* source_file, int line);
		
	};
	/// @brief Alias for simpler code.
	#define index_error(index) hltypes::_index_error(index, __FILE__, __LINE__)
	
	/// @brief Defines a range-error exception.
	class hltypesExport _range_error : public exception
	{
	public:
		/// @brief Basic constructor.
		/// @param[in] start Start of the range.
		/// @param[in] count Number of elements in the range.
		/// @param[in] source_file Name of the source file.
		/// @param[in] line Number of the line.
		_range_error(int start, int count, const char* source_file, int line);
		
	};
	/// @brief Alias for simpler code.
	#define range_error(start, count) hltypes::_range_error(start, count, __FILE__, __LINE__)
	
	/// @brief Defines a key-error exception.
	class hltypesExport _key_error : public exception
	{
	public:
		/// @brief Basic constructor.
		/// @param[in] key Name of the key.
		/// @param[in] container Name of the container.
		/// @param[in] source_file Name of the source file.
		/// @param[in] line Number of the line.
		_key_error(chstr key, chstr container, const char* source_file, int line);
		
	};
	/// @brief Alias for simpler code.
	#define key_error(key, container) hltypes::_key_error(key, container, __FILE__, __LINE__)
	
	/// @brief Defines a resource-error exception.
	class hltypesExport _resource_error : public exception
	{
	public:
		/// @brief Basic constructor.
		/// @param[in] type Type of the resource.
		/// @param[in] name Name of the resource.
		/// @param[in] container Name of the container.
		/// @param[in] source_file Name of the source file.
		/// @param[in] line Number of the line.
		_resource_error(chstr type, chstr name, chstr container, const char* source_file, int line);
		
	};
	/// @brief Alias for simpler code.
	#define resource_error(type, name, container) hltypes::_resource_error(type, name, container, __FILE__, __LINE__)
	
}

#endif