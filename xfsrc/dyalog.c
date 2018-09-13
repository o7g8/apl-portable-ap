#include <machine.h>

#if	UNIX
#else //UNIX
#	include <windows.h>
#endif//UNIX

#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>

DLLEXPORT VP LIBCALL MEMCPY(VP to, VP from, size_t nchars)
	{
	return memcpy
		(
		to
	,	from
	,	nchars
		);
	}

DLLEXPORT VP LIBCALL STRNCPY(VP to, VP from, size_t nchars)
	{
	return strncpy
		(
		to
	,	from
	,	nchars
		);
	}

DLLEXPORT VP  LIBCALL STRNCPYA(VP to, VP from, size_t nchars)
	{
	return STRNCPY(to,from,nchars);
	}

DLLEXPORT VP  STRNCPYW(VP to, VP from, size_t nchars)
	{
	return wcsncpy(to,from,nchars);
	}

DLLEXPORT size_t LIBCALL STRLEN(VP str)
	{
	return strlen(str);
	}

DLLEXPORT unsigned geterrno(void)
	{
	return errno;
	}

#if	UNIX
#else //UNIX

int APIENTRY DllMain
	(
	HANDLE hdll
,	DWORD reason
,	LPVOID  reserved
	)
	{
	return 1;
	}

#endif//UNIX
