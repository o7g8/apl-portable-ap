#ifndef _WINDOWSTYPES_H
#define _WINDOWSTYPES_H

#if !defined(_INC_WINDOWS)

#include <apltypes.h>

	typedef uintptr_t DWORD;
	typedef struct
		{
		int x;
		int y;
		} POINT;
	typedef struct
		{
		int left;
		int right;
		int top;
		int bottom;
		} RECT;
	typedef RECT *LPRECT;
	typedef void *WNDPROC;
	typedef void	*HANDLE;
	typedef void	*GLOBALHANDLE;
#define	strict(name)	typedef struct { int name; } *name
	strict(HWND);
	strict(HFONT);
	strict(HMENU);
	strict(HPALETTE);
	strict(HCURSOR);
	strict(HRGN);
	strict(HBITMAP);
	strict(HDC);
	strict(HICON);
	strict(HIMAGELIST);
	strict(HINSTANCE);
	strict(HMODULE);
	strict(HBRUSH);
	strict(HPEN);
	strict(HMETAFILE);
#undef	strict
	typedef int TEXTMETRIC;
	typedef int PAINTSTRUCT;
	typedef int BITMAPINFO;
	typedef int LPENUMUNKNOWN;
	typedef UCH	*LPSTR;
	typedef int32_t	LRESULT;
#ifndef _HRESULT_DEFINED
	typedef DWORD	HRESULT;
#endif
	typedef DWORD	WPARAM;
	typedef DWORD	LPARAM;
	typedef DWORD	UINT;
	typedef uint16_t	WORD;
	typedef UCH BYTE;
	typedef unsigned BOOL;
	typedef unsigned short ATOM;
	typedef void	*LPVOID;
	typedef void 	*FARPROC;

	typedef struct _FILETIME {
	    DWORD dwLowDateTime;
	    DWORD dwHighDateTime;
	} FILETIME;

	typedef UPTR	UINT_PTR;

#endif //!defined(_INC_WINDOWS)

#endif // _WINDOWSTYPES_H
