#ifndef _GFNS_H
#define _GFNS_H

#ifdef __cplusplus
extern "C"{
#endif 

#include <stdarg.h>
#include <wchar.h>
#include <string.h>

#define GFN 	extern

#define galloc(l)		(gchar*)malloc((l)*sizeof(gchar))
#define galloca(l)		(gchar*)alloca((l)*sizeof(gchar))
#define aalloc(l)		(achar*)malloc((l)*sizeof(achar))
#define salloc(l)		(symchar*)malloc((l)*sizeof(symchar))
#define aalloca(l)		(achar*)alloca((l)*sizeof(achar))
#define salloca(l)		(symchar*)alloca((l)*sizeof(symchar))

static INLINE gchar *gmemcpy(gchar *s1, const gchar *s2, size_t len)
	{
	return (gchar *)memcpy(s1,s2,len*sizeof(gchar));
	}
static INLINE achar *amemcpy(achar *s1, const achar *s2, size_t len)
	{
	return (achar *)memcpy(s1,s2,len*sizeof(achar));
	}
static INLINE symchar *smemcpy(symchar *s1, const symchar *s2, size_t len)
	{
	return (symchar *)memcpy(s1,s2,len*sizeof(symchar));
	}

static INLINE int gmemcmp(const gchar *s1, const gchar *s2, size_t len)
	{
#if	BYTE0123	// big endian just works as bytes
	return memcmp(s1,s2,(len)*sizeof(gchar));
#else //BYTE0123
	for		// little endian requires more effort
		(
		--s1
	,	--s2
	,	++len
	;	--len 
	&&	*++s1 == *++s2
	;
		);
	return len ? *s2 - *s1 : 0;
#endif//BYTE0123
	}
static INLINE gchar *gmemmove(gchar *s1, const gchar *s2, size_t len)
	{
	return (gchar *)memmove(s1,s2,len*sizeof(gchar));
	}

static INLINE achar *amemmove(achar *s1, const achar *s2, size_t len)
	{
	return (achar *)memmove(s1,s2,len*sizeof(achar));
	}


#define gstrsize(s1)		(sizeof(gchar)*gstrlen( s1))	// does NOT include null
#define gstrsize0(s1)		(sizeof(gchar)+gstrsize(s1))	// include null

#define gstrleft(buf)		(sizeof(buf)-gstrsize0(buf))

#if	HAS_UNICODE
	#define gsscanf		swscanf
#else //HAS_UNICODE
	#define gsscanf		sscanf
#endif//HAS_UNICODE

extern int LIBCALL gstricmp(const gchar *s0,const gchar *s1);
extern int LIBCALL gstrnicmp(const gchar *s0,const gchar *s1,size_t count);


GFN void * u32memset(const uint32_t *cp, uint32_t c, size_t len);
GFN void *gmemset(void *dest, int c, size_t count);
GFN void *amemset(achar *dest, achar c, size_t count);
GFN void* amemchr(const achar *buf,achar ch,size_t count);
GFN void* smemchr(const symchar *buf,symchar ch,size_t count);


GFN size_t g32strlen(const uint32_t *cp);
GFN size_t gstrlen(const gchar *cp);
GFN size_t astrlen(const achar *cp);
GFN size_t sstrlen(const symchar *cp);
GFN size_t astrncpy(achar *s1,const achar *s2,size_t count);
GFN size_t astrcat(achar *s1,const achar *s2);

GFN size_t astrnlen(const achar *cp, size_t count);

GFN void gstrtolower(gchar  *);
GFN void gstrtoupper(gchar  *);

GFN int gisdigit(int c);
GFN int gisalpha(int c);
GFN int gtolower(int c);
GFN int gtoupper(int c);
GFN int gstrncmp(const gchar *s1,const gchar *s2,size_t count);
GFN int LIBCALL gstrcmp(const gchar *s1,const gchar *s2);
GFN gchar* gstrchr(const gchar *s1,gchar ch);
GFN gchar* gstrrchr(const gchar *s1,gchar ch);
GFN void* gmemchr(const void *buf,int ch,size_t count);
GFN size_t gstrspn(const gchar *s1,const gchar *s2);
GFN size_t gstrcspn(const gchar *s1,const gchar *s2);
GFN gchar* gstrpbrk(const gchar *s1,const gchar *s2);
GFN gchar* gstrstr(const gchar *s1,const gchar *s2);
GFN gchar* gstrtok(gchar *s1,const gchar *s2, gchar **ptr);
GFN gchar* gstrncat(gchar *s1,const gchar *s2,size_t count);
GFN gchar* gstrcat(gchar *s1,const gchar *s2);

GFN gchar* gstrcpy(gchar *s1,const gchar *s2);
GFN size_t astrcpy(achar *s1,const achar *s2);

GFN gchar* gstrncpy(gchar *s1,const gchar *s2,size_t count);
GFN gchar* gstrncpy0(gchar *dst,gchar *src,size_t len);
GFN int gvsprintf(gchar *buf,size_t bsize,const gchar *fmt, va_list args );
GFN int gsprintf(gchar *buf,size_t bsize,const gchar* fmt, ...);
GFN long gatol(gchar *cp);
GFN int gatoi(gchar *cp);
GFN gchar * gitoa(int value, gchar *str,int radix);
GFN gchar * gltoa(long value, gchar *str,int radix);
GFN double gstrtod(const gchar *cp, gchar **ep);
GFN apldecf128 gstrtod128(const gchar *cp, gchar **ep);
GFN unsigned long gstrtoul(gchar *cp, gchar **ep, int base);
GFN          long gstrtol (gchar *cp, gchar **ep, int base);
GFN uintmax_t gstrtoumax(gchar *cp, gchar **ep, int base);
GFN gchar * gstrdup(const gchar *s);
GFN achar * astrdup(const achar *s);

GFN gchar * gstrerror(int errnum);
GFN gchar * getlasterrorstr(int errnum);


#if !defined GFNS_NO_IO
#if UNIX
#	include <unistd.h>
#else
#	include <io.h>
#endif
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

GFN int aaccess(achar *file, int mode);
GFN int gaccess(gchar *file, int mode);
GFN FILE* gfopen(const gchar *filename, const gchar *mode);
GFN FILE* afopen(const achar *filename, const gchar *mode);

#if UNIX
#include <ourdirent.h>
GFN DIR* gopendir(const gchar *dirname);
GFN DIR* aopendir(const achar *dirname);
#else // UNIX
//#include <direct.h>
#endif //UNIX

GFN FILE* gfdopen(int fd, const gchar *mode);


GFN int gopen(const gchar *file,int oflag, ...);
GFN int aopen(const achar *file,int oflag, ...);

GFN int gsopen(const gchar *file,int oflag, int shflag, ...);
GFN int asopen(const achar *file,int oflag, int shflag, ...);

GFN int grename(const gchar *from,const gchar *to);
GFN int arename(achar *from, achar *to);

GFN int gremove(gchar *filename);

GFN int gunlink(const gchar *filename);
GFN int aunlink(const achar *filename);

GFN int grmdir(const gchar *filename);
GFN int armdir(const achar *filename);

GFN int gmkdir(const gchar *filename);
GFN int amkdir(const achar *filename);

GFN gchar * gfullpath(gchar *absPath, const gchar *relPath,size_t maxlen);
GFN achar * afullpath(achar *absPath, const achar *relPath,size_t maxlen);

GFN int gstat(const gchar *path, GSTAT *info);
GFN int astat(const achar *path, GSTAT *info);

#if UNIX
GFN int glstat (const gchar *path, GSTAT *info);
GFN int alstat (const achar *path, GSTAT *info);
#endif // UNIX

#if UNIX
GFN ssize_t greadlink (const gchar *path, gchar *buf, size_t maxlen);
GFN ssize_t areadlink (const achar *path, achar *buf, size_t maxlen);
#endif // UNIX

GFN gchar * gmktemp(gchar *absPath);
GFN achar * amktemp(achar *absPath);

GFN void gperror(gchar *msg);

GFN int gvfprintf(FILE* file, const gchar* fmt, va_list args );
GFN int gfprintf(FILE* file, const gchar* fmt, ...);

#endif // GFNS_NO_IO

GFN int gchdir(const gchar *dirname);
GFN gchar *ggetcwd(gchar *buf, int maxlen);
GFN achar *agetcwd(achar *buf, int maxlen);

GFN achar * agetenv(const gchar *envname);

#if 0
//	gctime is commented out of gfns.c.  It was causing a problem with CE
//	build of the installer, so I will now strip out the include and gctime
//	below.
#include <time.h>
GFN gchar * gctime(time_t *timer);
#endif //0

#if	UNIX
GFN void* gapldlopen(const gchar *lib, int flag);
#else //UNIX
GFN int get_temp_fd(void);
#endif//UNIX

GFN int gsprintf_l (gchar *buf, size_t bsize, const gchar *fmt, ...);
GFN int gvsprintf_l (gchar *buf, size_t bsize, const gchar *fmt, va_list args);



#undef GFN

#ifdef __cplusplus
}
#endif 

#endif // _GFNS_H
