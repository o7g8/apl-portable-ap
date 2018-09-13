#ifndef	OURDIRENT_H
#define	OURDIRENT_H
#if	LINUX && !defined(_BSD_SOURCE)
	#define		_BSD_SOURCE	1	// get values for dt_value
		#include	<dirent.h>
	#undef		_BSD_SOURCE
#elif UNIX
	#include	<dirent.h>
#endif//LINUX
#if	RIOS	// doesn't give us 64 bit inodes
	#define	DIR		DIR64
	#define	dirent		dirent64
	#define	opendir		opendir64
	#define	readdir		readdir64
	#define readdir_r	readdir64_r
	#undef	rewinddir			// defined in terms of seekdir
	#define rewinddir	rewinddir64
	#define telldir		telldir64
	#define seekdir		seekdir64
	#define alphasort	alphasort64
	#define	scandir		scandir64
	#define	closedir	closedir64
#endif//RIOS
#endif//OURDIRENT_H
