#ifndef _UNICODE_H
#define _UNICODE_H

#define MAX_UNICODE 0x2600
#define UNICODE_SPECIAL_START	0xF800
#define UNICODE_FKEY_START	0xF700

#define MAX_CODEPOINT		(17 * 0x10000 - 1)

extern unicode_t Unicode[256];

#define WDCHalloc(length) ((unicode_t *) alloca( length * sizeof(unicode_t)))

extern int FromUNICODE(const unicode_t *cp,gchar *buf,size_t csize);
extern int ToUNICODE(gchar *cp,unicode_t *buf,size_t osize);

extern int FromUNICODElen(const unicode_t *ocp);

extern void unitounix(UCP targ,unicode_t *wptr,size_t len);
extern void unitoav(gchar *cp,const unicode_t *sp,size_t len);
extern void avtouni(unicode_t *wcp,gchar *cp,size_t len);

extern int UNICODEtoKEY(int key);
extern uns KEYtoUNICODE(int key);
extern ubyte UNICODEtoAV(int uni);

#define IS_HI_UTF16(u) ((u)>=0xd800 && (u)<=0xdbff)
#define IS_LO_UTF16(u) ((u)>=0xdc00 && (u)<=0xdfff)

#if HAS_UNICODE
#define AVtoUNICODE(av)	(av)
#else
extern unicode_t AVtoUNICODE(UCH av);
#endif

extern void unixtouni(unicode_t *wptr,const UCH *src,size_t len);
extern void SetUnicodeTable(void);
extern void UpdateDotNetUnicodeTable(void);
extern int MAP_UNICODE_SPECIAL(int i_uni)				;

#if 0

#include <wchar.h>

static void *___tmp;

#define ALLOCFAIL	(~(IPTR)0)

static unicode_t **_WCOPY(unicode_t *w,const UCH *c)
{
if (w && (IPTR)w!=ALLOCFAIL)
	unixtouni(w,c,strlen((const char *)c)+1);
return 0;
}

static void _UCOPY(UCH *c,unicode_t *w)
{
	unitounix(c,w,wcslen(w)+1);
}


#define WIDE(str)	  wide_##str
#define PWIDE(str)	p_wide_##str

#define __WALLOC(len)	(___tmp=(unicode_t*)alloca((1+(len))*sizeof(unicode_t)),___tmp?___tmp:ALLOCFAIL)
#define  _WALLOC(str)	((str)?__WALLOC(strlen(str)):0)

#define WCOPY(str) unicode_t *WIDE(str)=_WALLOC(str),**PWIDE(str)=_WCOPY(WIDE(str),str)
#define UCOPY(str) if (str) _UCOPY(str,WIDE(str))

#define jdWALLOC(name,len) unicode_t *WIDE(name)=(name?__WALLOC(len):0)

#define WCHECK(name,retn)	if (ALLOCFAIL==(WIDE(name))) return retn

#define WIDEN1(name1,retn)		WCOPY(name1); WCHECK(name1,retn)
#define WIDEN2(name1,name2,retn)	WCOPY(name1); WCOPY(name2); WCHECK(name1,retn); WCHECK(name2,retn)

#endif

#endif // _UNICODE_H
