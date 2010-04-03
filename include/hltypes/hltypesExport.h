/************************************************************************************\
* This source file is part of the High Level C++ types library                       *
* For latest info, see http://libhltypes.sourceforge.net/                            *
**************************************************************************************
* Copyright (c) 2010 Kresimir Spes, Boris Mikic, Domagoj Cerjan                      *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef HLTYPES_EXPORT_H
#define HLTYPES_EXPORT_H

	#ifdef _STATICLIB
		#define hltypesExport
	#else
		#ifdef _WIN32
			#ifdef HLTYPES_EXPORTS
				#define hltypesExport __declspec(dllexport)
			#else
				#define hltypesExport __declspec(dllimport)
			#endif
		#else
			#define hltypesExport __attribute__ ((visibility("default")))
		#endif
	#endif

#endif

