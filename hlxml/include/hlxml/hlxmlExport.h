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
/// Defines macros for DLL exports/imports.

#ifndef HLXML_EXPORT_H
#define HLXML_EXPORT_H

	/// @def hlxmlExport
	/// @brief Macro for DLL exports/imports.
	/// @def hlxmlFnExport
	/// @brief Macro for function DLL exports/imports.
	#ifdef _LIB
		#define hlxmlExport
		#define hlxmlFnExport
	#else
		#ifdef _WIN32
			#ifdef HLXML_EXPORTS
				#define hlxmlExport __declspec(dllexport)
				#define hlxmlFnExport __declspec(dllexport)
			#else
				#define hlxmlExport __declspec(dllimport)
				#define hlxmlFnExport __declspec(dllimport)
			#endif
		#else
			#define hlxmlExport __attribute__ ((visibility("default")))
			#define hlxmlFnExport __attribute__ ((visibility("default")))
		#endif
	#endif
	#ifndef DEPRECATED_ATTRIBUTE
		#ifdef _MSC_VER
			#define DEPRECATED_ATTRIBUTE __declspec(deprecated("function is deprecated"))
		#else
			#define DEPRECATED_ATTRIBUTE __attribute__((deprecated))
		#endif
	#endif

#endif

