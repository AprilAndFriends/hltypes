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
/// Legacy header.

#ifndef HLTYPES_EXCEPTION_LEGACY_H
#define HLTYPES_EXCEPTION_LEGACY_H

#pragma message("'hltypes/exception.h' is deprecated. Include 'hltypes/hexception.h' instead.")
#include "hexception.h"

namespace hltypes
{
	DEPRECATED_ATTRIBUTE typedef _Exception exception;
	DEPRECATED_ATTRIBUTE typedef _FileCouldNotOpenException _file_could_not_open;
	DEPRECATED_ATTRIBUTE typedef _FileNotOpenException _file_not_open;
	DEPRECATED_ATTRIBUTE typedef _FileNotWriteableException _file_not_writeable;
	DEPRECATED_ATTRIBUTE typedef _ResourceNotExistsException _resource_not_exists;
	DEPRECATED_ATTRIBUTE typedef _ResourceAlreadyExistsException _resource_already_exists;
	DEPRECATED_ATTRIBUTE typedef _ContainerIndexException _container_index_error;
	DEPRECATED_ATTRIBUTE typedef _ContainerEmptyException _container_empty_error;
	DEPRECATED_ATTRIBUTE typedef _ContainerElementNotFoundException _container_element_not_found;
	DEPRECATED_ATTRIBUTE typedef _ContainerRangeException _container_range_error;
	DEPRECATED_ATTRIBUTE typedef _ContainerKeyException _container_key_error;

	// DEPRECATED
	#define hl_exception(message) hltypes::exception(message, __FILE__, __LINE__)
	#define file_could_not_open(filename) hltypes::_file_could_not_open(filename, false, __FILE__, __LINE__)
	#define resource_not_found(filename) hltypes::_file_could_not_open(filename, true, __FILE__, __LINE__)
	#define file_not_open(filename) hltypes::_file_not_open(filename, __FILE__, __LINE__)
	#define file_not_writeable(filename) hltypes::_file_not_writeable(filename, __FILE__, __LINE__)
	#define resource_not_exists(type, name, container) hltypes::_resource_not_exists(type, name, container, __FILE__, __LINE__)
	#define resource_already_exists(type, name, container) hltypes::_resource_not_exists(type, name, container, __FILE__, __LINE__)
	#define container_index_error(index) hltypes::_container_index_error(index, __FILE__, __LINE__)
	#define container_empty_error(function_name) hltypes::_container_empty_error(function_name, __FILE__, __LINE__)
	#define container_element_not_found() hltypes::_container_element_not_found(__FILE__, __LINE__)
	#define container_range_error(start, count) hltypes::_container_range_error(start, count, __FILE__, __LINE__)
	#define container_key_error(key, container) hltypes::_container_key_error(key, container, __FILE__, __LINE__)

}

#endif
