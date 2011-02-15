/**
 * @file
 * @author  Kresimir Spes
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
 *
 * @section DESCRIPTION
 *
 * Defines macros for DLL exports/imports.
 */
#ifndef HLTYPES_EXPORT_H
#define HLTYPES_EXPORT_H

	#ifdef _STATICLIB
		#define hltypesExport
		#define hltypesFnExport
	#else
		#ifdef _WIN32
			#ifdef HLTYPES_EXPORTS
				#define hltypesExport __declspec(dllexport)
				#define hltypesFnExport __declspec(dllexport)
			#else
				#define hltypesExport __declspec(dllimport)
				#define hltypesFnExport __declspec(dllimport)
			#endif
		#else
			#define hltypesExport __attribute__ ((visibility("default")))
			#define hltypesFnExport
		#endif
	#endif

#endif

