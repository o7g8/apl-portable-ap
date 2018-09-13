#ifndef _TRANS_H
#define _TRANS_H

typedef enum {ASCII, QUADAV} CHARSET;

#include <io_maps.h>		// input/output mapping structure 

extern gchar *map_environment_name(gchar *fm); // uses local static buffer (256 char limit)
extern const gchar *fextend(achar *fname, const gchar *fext);
extern const gchar *fextend_ext(achar *fname, const gchar *fext);
extern const gchar *fextend_dot(achar *fname);
extern achar *ftrunc(achar **thisname,achar **lim,const gchar *ext,CHARSET charset);
extern achar *get_date_string(int code);
extern achar *inlab(int fd,achar *name);
extern achar *lc_num2str(int code);
extern UCH *t_buffer(void (*fn)(int ),int p1);

extern size_t key_to_codes(int key, ubyte *codes);

extern UCH   WINCHARS[256];
extern UCH BODGECHARS[256];

#if HAS_UNICODE

#define asctoav(a,mode)	(a)
#define avtounix(c)	(c)
#define unixtoav(a)	(a)
#define WINCHARS(a)	(a)
#define BODGECHARS(a)	(a)

#else //HAS_UNICODE

extern gchar asctoav(gchar a,unsigned int aplmode);
extern UCH avtounix(UCH c);
extern UCH unixtoav(UCH a);
#define WINCHARS(a)	WINCHARS[(unsigned char)(a)]
#define BODGECHARS(a)	BODGECHARS[(unsigned char)(a)]

#endif //HAS_UNICODE

extern gchar *gstrncpy0(gchar *dst,gchar *src,size_t len);
extern int GetKey(ubyte vk,ubyte shift,int allow_mode);
extern int Inkey(int fd,int up,unsigned int (*mc_fn)(ubyte *,Imap *,unsigned int ,unsigned int ,unsigned int ));
extern int MapScanCode(int sc,int vk);

typedef enum
	{
	WITHPOINT = 0
,	WITHOUTPOINT = 1
	} WITHPOINT_t;

extern int aisalpha(achar c);
extern int char_in_word(achar c, WITHPOINT_t withpoint);
extern int in_word(achar c);

extern int LIBCALL avstrcmp(const achar *s1,const achar *s2);
extern int LIBCALL avstricmp(const achar *s1,const achar *s2);

extern int avstrncmp(const achar *from,const achar *to,size_t max);
extern int avstrnicmp(const achar *s1,const achar *s2,size_t len);

extern int avtoupper(achar c);

extern void AVtoUpper(achar *cp,size_t len);
extern void ToLower(gchar *cp,size_t len);
extern void ToUpper(gchar *cp,size_t len);

extern int upper_of(int fd,achar key);
extern int lower_of(int fd,achar key);

extern int err_eq_out(void);
extern int getlinecode(UCH *cp);
extern int getsafelinecode(UCH *cp);
extern int gui_Inkey(int fd,int up,unsigned int (*mc_fn)(ubyte *,Imap *,unsigned int ,unsigned int ,unsigned int ),int ansi,int allow_mode);
extern void init_out(UPTR describe,void (*fail)(void ));
extern int inkey(int fd,int up);
extern int t_count(void (*fn)(int ),int param);
extern int t_safe(UCP cp,unsigned int count);
extern unsigned int keyset(int fd,unsigned int newmode,int up);
extern unsigned int mc_dflt(ubyte *cmode,Imap *imp,unsigned int up,unsigned int thismode,unsigned int newmode);
extern unsigned int unixstr2av(gchar *dest,gchar *src,unsigned int maxchars);
extern unsigned short get_date_length(int code);
extern void *__cdecl get_ours(const gchar *envname);
extern void __cdecl o_finish(void );
extern void ansi2av(UCH *cptr,size_t size);
extern void ascsp(int code);
extern void av2ansi(UCH *cptr,size_t size);
extern void avtoasc(achar c);


#if HAS_UNICODE

#define avtow3(str)
#define w3toav(str)
#define AVtoW3(av,len)
#define W3toAV(av,len)

#else //HAS_UNICODE

extern void w3toav(UCH *av);
extern void avtow3(UCH *av);
extern void AVtoW3(UCH *av,size_t len);
extern void W3toAV(UCH *av,size_t len);

#endif //HAS_UNICODE

extern void d_init(void );
extern void set_key_caps_lock(int flag);
extern void special(int code);
extern void t_safe_init_out(UPTR fd);
extern int (*readfn)(int,int);
extern int (*out_fn)(ubyte *, unsigned);
extern bool handles_layout_switch(int fd);
extern int zero_is_input_fd(void);

extern int SESSION_IN_FD;

#define TAG_OVERSTRIKE		1
#define TAG_BACKTICK		2

#endif // _TRANS_H
