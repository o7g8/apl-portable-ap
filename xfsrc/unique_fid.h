#ifndef _UNIQUE_FID_H
#define _UNIQUE_FID_H

#if	UNIX

#include	<sys/types.h>
#include	<sys/stat.h>

struct unique_fid_struct
	{
	enum
		{
		FID_INVALID
	,	FID_FILE
		} fid_type;
	union
		{
		struct
			{
			unsigned last_error;
			} invalid_info;
		struct
			{
			dev_t	st_dev;
			ino_t	st_ino;
			} file_info;
		} info;
	};

typedef struct unique_fid_struct UNIQUE_FID;

#elif	defined(WINDOWS3)

//#if defined(DEFINE_UNIQUE_FID)

#if	defined(_INC_WINDOWS)
#else //defined(_INC_WINDOWS)
#	include	<windowstypes.h>
#endif//defined(_INC_WINDOWS)

typedef enum
	{
	FID_INVALID
,	FID_FILE_FD
,	FID_FILE_NAME
,	FID_CHAR
,	FID_PIPE
,	FID_REMOTE
,	FID_UNKNOWN
	} FID_TYPE;

struct unique_fid_struct
	{
	FID_TYPE fid_type;
	union
		{
		struct
			{
			DWORD	last_error;
			} invalid_info;
		struct
			{
			DWORD	dwVolumeSerialNumber;
			DWORD	nFileIndexHigh;
			DWORD	nFileIndexLow;
			DWORD	nFileSizeHigh;
			DWORD	nFileSizeLow;
			} file_fd_info;
		struct
			{
			DWORD	dwVolumeSerialNumber;
			FILETIME ftLastWriteTime;
			DWORD	nFileSizeHigh;
			DWORD	nFileSizeLow;
			} file_name_info;
		struct
			{
			unsigned anything;
			} pipe_info;
		struct
			{
			unsigned anything;
			} char_info;
		struct
			{
			unsigned anything;
			} remote_info;
		struct
			{
			unsigned anything;
			} unknown_info;
		} info;
	
	};
typedef struct unique_fid_struct UNIQUE_FID;

//#else //defined(DEFINE_UNIQUE_FID)

//#endif //defined(DEFINE_UNIQUE_FID)


#endif//UNIX || defined(WINDOWS3)

extern UNIQUE_FID fd_to_unique_fid(int fd);
extern UNIQUE_FID filename_to_unique_fid(gchar * name);
extern unsigned int match_unique_fid(UNIQUE_FID thisone,UNIQUE_FID thatone);


unsigned valid_fd(int fd);

#endif // _UNIQUE_FID_H
