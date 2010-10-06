#ifdef _WIN32
#include <direct.h>
#elif defined(__APPLE__)
#include <sys/stat.h>
#define _mkdir(name) mkdir(name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)
#endif

#include "harray.h"
#include "hdir.h"

namespace hltypes
{
	dir::dir()
	{
	}
	
	dir::~dir()
	{
	}
		
	void dir::makedirs(chstr path)
	{
		harray<hstr> folders = path.split("/");
		if (folders.size() > 0)
		{
			hstr path = folders.pop_front();
			foreach (hstr, it, folders)
			{
				path += "/" + (*it);
				_mkdir(path.c_str());
			}
		}
	}
	
}
