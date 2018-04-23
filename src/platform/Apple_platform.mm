/// @file
/// @version 4.0
///
/// @section LICENSE
///
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifdef __APPLE__
#import <Foundation/Foundation.h>

#include "hlog.h"
#include "hplatform.h"
#include "hstring.h"
#include "platform_internal.h"

#ifdef _DEBUG

#define TAG_MAX 35
#define TAG_MAX_FMT "35"

#include <Foundation/Foundation.h>
#include <sys/sysctl.h>

bool isDebuggerAttached()
{
	static BOOL debuggerIsAttached = NO;
	
	static dispatch_once_t debuggerPredicate;
	dispatch_once(&debuggerPredicate,
	^{
		struct kinfo_proc info;
		size_t info_size = sizeof(info);
		int name[4];
		
		name[0] = CTL_KERN;
		name[1] = KERN_PROC;
		name[2] = KERN_PROC_PID;
		name[3] = getpid(); // from unistd.h, included by Foundation
		
		if (sysctl(name, 4, &info, &info_size, NULL, 0) == -1)
		{
			NSLog(@"ERROR: Checking for a running debugger via sysctl() failed: %s", strerror(errno));
			debuggerIsAttached = false;
		}
		
		if (!debuggerIsAttached && (info.kp_proc.p_flag & P_TRACED) != 0)
		{
			debuggerIsAttached = true;
		}
	});
	
	return debuggerIsAttached;
}
#else
bool isDebuggerAttached()
{
	return false;
}
#endif

namespace hltypes
{
	void _platformPrint(const String& tag, const String& message, int level)
	{
		// detect if we're running the app in xcode and if so, format prints to look better. debug only.
		static int xcode = -1;
		
		if (xcode == 0)
		{
			if (tag != "")
			{
				NSLog(@"[%s] %s", tag.cStr(), message.cStr());
			}
			else
			{
				NSLog(@"%s", message.cStr());
			}
		}
		else
		{
			if (xcode == -1) // put here for performance reasons
			{
				xcode = isDebuggerAttached();
				_platformPrint(tag, message, level);
				return;
			}
			
			if (tag != "")
			{
				char cTag[TAG_MAX + 1];
				memset(cTag, 0, TAG_MAX);
				snprintf(cTag, TAG_MAX + 1, "%" TAG_MAX_FMT "s", tag.cStr());
				if (message.contains("\n"))
				{
					harray<hstr> lines = message.split("\n");
					hstr first = lines.removeFirst();
					printf("%s | %s\n", cTag, first.cStr());
					foreach (hstr, it, lines)
					{
						printf("%" TAG_MAX_FMT "s | %s\n", " ", it->cStr());
					}
				}
				else
				{
					printf("%s | %s\n", cTag, message.cStr());
				}
			}
			else
			{
				printf("%s\n", message.cStr());
			}
		}
	}
}
#endif
