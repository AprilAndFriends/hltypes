/// @file
/// @version 3.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef _MSC_VER // required for memset on non-MSVC compilers
#include <string.h>
#endif
#ifndef _WIN32
#include <sys/time.h>
#endif
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define __HL_INCLUDE_PLATFORM_HEADERS
#include "harray.h"
#include "hlog.h"
#include "hltypesUtil.h"
#include "hplatform.h"
#include "hresource.h"
#include "hstream.h"
#include "hstring.h"
#include "platform_internal.h"

#if defined(_WIN32) && !defined(_WINRT)
#pragma warning(disable : 4091) // MS's own headers cause warnings
// needed for stack trace functions
#include <dbghelp.h> // has to be here after hplatform.h that includes windows.h
#pragma comment(lib, "dbghelp.lib")
#elif defined(_ANDROID)
#include <unwind.h>
#include <dlfcn.h>
#endif

#define MAX_STACK_ADDRESS_NAME_SIZE 1024
#define MAX_STACK_FRAMES 62 // WinXP has a max of 63 frames and the current frame is always skipped

namespace hltypes
{
	String logTag = "hltypes";
}

uint64_t htime()
{
	return (uint64_t)time(NULL);
}

uint64_t htickCount()
{
#ifdef _WIN32
#ifndef _WINRT // because GetTickCount64() is not available pre-Vista
	return GetTickCount();
#else
	return (uint64_t)GetTickCount64();
#endif
#else
	timeval tv = {0, 0};
	gettimeofday(&tv, NULL);
	uint64_t tv_sec = tv.tv_sec, tv_usec = tv.tv_usec; // cast first, because if we multiply by 1000 before casting we could get an overflow on 32 bit systems
	return (tv_sec * 1000 + tv_usec / 1000);
#endif
}

hltypes::String henv(const hltypes::String& name)
{
	return hltypes::_platformEnv(name);
}

#ifdef _ANDROID
struct StackFrame
{
	hltypes::Array<void*> addresses;
	hltypes::Array<hltypes::String> names;
	int maxFrames;
};

static _Unwind_Reason_Code _traceFunction(struct _Unwind_Context* context, void* arg)
{
	StackFrame* frame = (StackFrame*)arg;
	void* ip = (void*)_Unwind_GetIP(context);
	if (ip != NULL)
	{
		frame->addresses += ip;
		Dl_info info;
		memset(&info, 0, sizeof(Dl_info));
		hstr name = "unknown";
		if (dladdr(ip, &info) != 0)
		{
			if (info.dli_sname != NULL)
			{
				name = hstr(info.dli_sname);
			}
			name += " (" + hstr(info.dli_fname) + ")";
		}
		frame->names += name;
	}
	return (frame->addresses.size() < frame->maxFrames ? _URC_NO_REASON : _URC_END_OF_STACK);
}
#endif

hltypes::String hstackTrace(int maxFrames)
{
	// the implementations on some platforms are not thread-safe
	static hltypes::Mutex stackMutex;
	hltypes::Mutex::ScopeLock lock(&stackMutex);
	// get stack trace
	hltypes::String result = "Stack trace not available on this platform!";
	maxFrames = hclamp(maxFrames, 1, MAX_STACK_FRAMES);
#if defined(_WIN32) && !defined(_WINRT) // doesn't work on WinRT
	result = "Could not obtain stack trace!";
	HANDLE process = GetCurrentProcess();
	if (process != NULL)
	{
		if (SymInitialize(process, NULL, TRUE))
		{
			void* stack[100];
			unsigned short frames = RtlCaptureStackBackTrace(1, maxFrames, stack, NULL); // skipping this function call from the stack trace
			if (frames > 0)
			{
				hltypes::Array<hltypes::String> calls;
				PSYMBOL_INFO symbol = (PSYMBOL_INFO)malloc(sizeof(SYMBOL_INFO) + MAX_STACK_ADDRESS_NAME_SIZE * sizeof(char));
				symbol->MaxNameLen = MAX_STACK_ADDRESS_NAME_SIZE + 1;
				symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
				for_iter (i, 0, frames)
				{
					SymFromAddr(process, (DWORD64)stack[i], 0, symbol);
					calls += hsprintf("0x%0llX - %s\n", symbol->Address, symbol->Name);
				}
				free(symbol);
				result = calls.joined("");
			}
		}
		SymCleanup(process);
	}
#elif defined(_ANDROID)
	result = "Could not obtain stack trace!";
	StackFrame frame;
	frame.maxFrames = maxFrames;
	_Unwind_Backtrace(&_traceFunction, &frame);
	hltypes::Array<hltypes::String> calls;
	for_iter (i, 1, frame.addresses.size()) // skipping this function call from the stack trace
	{
		calls += hsprintf("%p - %s\n", frame.addresses[i], frame.names[i].cStr());
	}
	if (calls.size() > 0)
	{
		result = calls.joined("");
	}
#endif
	return result;
}

int hrand(int min, int max)
{
	if (max <= min)
	{
		return min;
	}
#ifdef _WIN32
	return (int)(min + rand() * (max - min) / (RAND_MAX + 1));
#else
	return (int)(min + ((double)rand()) / ((double)RAND_MAX + 1) * (max - min));
#endif
}

int hrand(int max)
{
	if (max <= 0)
	{
		return 0;
	}
#ifdef _WIN32
	return (rand() * max / (RAND_MAX + 1));
#else
	return (int)((((double)rand()) / ((double)RAND_MAX + 1)) * max);
#endif
}

float hrandf(float min, float max)
{
	if (max <= min)
	{
		return min;
	}
#ifdef _WIN32
	return (min + rand() * (max - min) / (RAND_MAX + 1));
#else
	return (min + ((double)rand()) / ((double)RAND_MAX + 1) * (max - min));
#endif
}

float hrandf(float max)
{
	if (max <= 0.0f)
	{
		return 0.0f;
	}
#ifdef _WIN32
	return (rand() * max / (RAND_MAX + 1));
#else
	return ((((double)rand()) / ((double)RAND_MAX + 1)) * max);
#endif
}

double hrandd(double min, double max)
{
	if (max <= min)
	{
		return min;
	}
#ifdef _WIN32
	return (min + rand() * (max - min) / (RAND_MAX + 1));
#else
	return (min + ((double)rand()) / ((double)RAND_MAX + 1) * (max - min));
#endif
}

double hrandd(double max)
{
	if (max <= 0.0)
	{
		return 0.0;
	}
#ifdef _WIN32
	return (rand() * max / (RAND_MAX + 1));
#else
	return ((((double)rand()) / ((double)RAND_MAX + 1)) * max);
#endif
}

int hfloor(float value)
{
	return (int)(value > 0.0f || value == (int)value ? value : value - 1);
}

int hfloor(double value)
{
	return (int)(value > 0.0 || value == (int)value ? value : value - 1);
}

float hfloorf(float value)
{
	return (float)hfloor(value);
}

double hfloord(double value)
{
	return (double)hfloor(value);
}

int hceil(float value)
{
	return (int)(value < 0.0f || value == (int)value ? value : value + 1);
}

int hceil(double value)
{
	return (int)(value < 0.0 || value == (int)value ? value : value + 1);
}

float hceilf(float value)
{
	return (float)hceil(value);
}

double hceild(double value)
{
	return (double)hceil(value);
}

int hround(float value)
{
	return hfloor(value + 0.5);
}

int hround(double value)
{
	return hfloor(value + 0.5);
}

float hroundf(float value)
{
	return (float)hround(value);
}

double hroundd(double value)
{
	return (double)hround(value);
}

int habs(int value)
{
	return value >= 0 ? value : -value;
}

int64_t habs(int64_t value)
{
	return value >= 0 ? value : -value;
}

long habs(long value)
{
	return value >= 0 ? value : -value;
}

float habs(float value)
{
	return value >= 0.0f ? value : -value;
}

double habs(double value)
{
	return value >= 0.0 ? value : -value;
}

long double habs(long double value)
{
	return value >= 0.0 ? value : -value;
}

int hmod(int i, int m)
{
	return (i >= 0 ? i % m : (m - (-i % m)) % m);
}

int64_t hmod(int64_t i, int64_t m)
{
	return (i >= 0 ? i % m : (m - (-i % m)) % m);
}

float hmodf(float f, float m)
{
	return (f >= 0.0f ? fmod(f, m) : fmod(m - fmod(-f, m), m));
}

double hmodd(double d, double m)
{
	return (d >= 0.0 ? fmod(d, m) : fmod(m - fmod(-d, m), m));
}

float hsqrt(int value)
{
	return sqrt((float)value);
}

float hsqrt(float value)
{
	return sqrt(value);
}

double hsqrt(double value)
{
	return sqrt(value);
}

long double hsqrt(long double value)
{
	return sqrt(value);
}

float hhypot(float a, float b)
{
	return (float)sqrt(a * a + b * b);
}

double hhypot(double a, double b)
{
	return sqrt(a * a + b * b);
}

float hhypot(int a, int b)
{
	return (float)sqrt((float)a * a + (float)b * b);
}

double hhypotd(int a, int b)
{
	return sqrt((double)a * a + (double)b * b);
}

float hhypotSquared(float a, float b)
{
	return (a * a + b * b);
}

double hhypotSquared(double a, double b)
{
	return (a * a + b * b);
}

int hhypotSquared(int a, int b)
{
	return (a * a + b * b);
}

float hhypotfSquared(int a, int b)
{
	return (float)(a * a + b * b);
}

double hhypotdSquared(int a, int b)
{
	return (double)(a * a + b * b);
}

bool heqf(float a, float b, float tolerance)
{
	return (habs(a - b) < tolerance);
}

bool heqd(double a, double b, double tolerance)
{
	return (habs(a - b) < tolerance);
}

int hcmpf(float a, float b, float tolerance)
{
	return (heqf(a, b, tolerance) ? 0 : (a > b ? 1 : -1));
}

int hcmpd(double a, double b, double tolerance)
{
	return (heqd(a, b, tolerance) ? 0 : (a > b ? 1 : -1));
}

int hpotCeil(int value)
{
	--value;
	value |= value >> 1;
	value |= value >> 2;
	value |= value >> 4;
	value |= value >> 8;
	value |= value >> 16;
	++value;
	return value;
}

int64_t hpotCeil(int64_t value)
{
	--value;
	value |= value >> 1;
	value |= value >> 2;
	value |= value >> 4;
	value |= value >> 8;
	value |= value >> 16;
	value |= value >> 32;
	++value;
	return value;
}

int hpotFloor(int value)
{
	int result = hpotCeil(value);
	if (value < result)
	{
		result >>= 1;
	}
	return result;
}

int64_t hpotFloor(int64_t value)
{
	int64_t result = hpotCeil(value);
	if (value < result)
	{
		result >>= 1;
	}
	return result;
}

// CRC32 stuff

static unsigned int crc32Table[256] = {0};
static bool crc32TableCreated = false;
void createCrc32Table()
{
	if (crc32TableCreated)
	{
		return;
	}
	unsigned int polynome = 0xEDB88320;
	unsigned int sum;
	for_iter (i, 0, 256)
	{
		sum = i;
		for_iter (j, 0, 8)
		{
			if ((sum & 1) != 0)
			{
				sum = ((sum >> 1) & 0x7FFFFFFF) ^ polynome;
			}
			else
			{
				sum = ((sum >> 1) & 0x7FFFFFFF);
			}
		}
		crc32Table[i] = sum;
	}
	crc32TableCreated = true;
}

unsigned int hcrc32(const unsigned char* data, unsigned int size)
{
	createCrc32Table();
	unsigned int crc = 0xFFFFFFFF;
	for_itert (unsigned int, i, 0, size)
	{
		crc = ((crc >> 8) & 0xFFFFFF) ^ crc32Table[(crc ^ data[i]) & 0xFF];
	}
	return ((crc & 0xFFFFFFFF) ^ 0xFFFFFFFF);
}

unsigned int hcrc32(hltypes::StreamBase* stream, unsigned int size)
{
	size = hmin(size, (unsigned int)(stream->size() - stream->position()));
	if (size <= 0)
	{
		return 0;
	}
	unsigned char* data = new unsigned char[size];
	int read = stream->readRaw(data, size);
	if (read == 0)
	{
		delete[] data;
		return 0;
	}
	stream->seek(-read);
	unsigned int result = hcrc32(data, size);
	delete[] data;
	return result;
}

unsigned int hcrc32(hltypes::StreamBase* stream)
{
	return hcrc32(stream, (unsigned int)(stream->size() - stream->position()));
}

unsigned int hcrc32(hltypes::Stream* stream, unsigned int size)
{
	return hcrc32(&stream->operator[]((int)stream->position()), hmin(size, (unsigned int)(stream->size() - stream->position())));
}

unsigned int hcrc32(hltypes::Stream* stream)
{
	return hcrc32(&stream->operator[]((int)stream->position()), (unsigned int)(stream->size() - stream->position()));
}

