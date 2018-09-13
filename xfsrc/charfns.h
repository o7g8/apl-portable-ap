#ifndef _CHARFNS_H
#define _CHARFNS_H

#include <alloca_h.h>

#ifdef __cplusplus
extern "C"{
#endif 

#include <eltypes.h>

#define APLTCHAR	10

#if NEED_STRCASECMP
extern int LIBCALL strcasecmp(const char *s1,const char *s2);
extern int LIBCALL strncasecmp(const char *s1,const char *s2,size_t len);
#endif

#if 1 // HAS_UNICODE

extern int 	et_strcmp_et(const void *to, uns t_type, const void *fm, uns f_type);
extern int    	et_stricmp_et(const void *v1, uns type1, const void *v2, uns type2);
extern int    	et_strncmp_et(const void *to, uns t_type, const void *fm, uns f_type,size_t len);
extern int    	et_strnicmp_et(const void *to, uns t_type, const void *fm, uns f_type,size_t len);
extern int    	et_memcmp_et(const void *to, uns type1, const void *fm, uns type2, size_t len);
extern size_t 	et_memcpy_et(void *to, uns type1, const void *fm, uns type2, size_t len);
extern size_t 	et_strcpy_et(void *to, uns t_type, const void *fm, uns f_type);
extern size_t 	et_strncpy_et(void *to, uns t_type, const void *fm, uns f_type,size_t len);
extern size_t 	et_strcat_et(void *to, uns t_type, const void *fm, uns f_type);

extern void * 	et_strchr(const void *to, uns t_type, uns ch);
extern void * 	et_strnchr(const void *to, uns t_type, uns ch,size_t size);
extern void * 	et_strstr_et(const void *v1, uns t1,const void *v2,uns t2);
extern void * 	et_strrchr(const void *to, uns t_type, uns ch);
extern size_t 	et_strlen(const void *to, uns t_type);
extern void * 	et_memchr(const void *to, uns t_type, uns ch,size_t len);
extern void * 	et_memrchr(const void *to, uns t_type, uns ch,size_t len);
extern size_t 	et_strcspn(const void *to, uns t_type, const void *fm, uns f_type);
extern int 	et_symcmp(const void *name,uns t_type,const void *sym,BOUND size);
extern int 	et_symicmp(const void *name,const void *sym,uns t_type,BOUND size);
extern int	et_symicmp_et(const void *name,uns n_type,const void *sym,uns s_type,BOUND size);

extern void 	et_memset(void *to, uns t_type, uns val,size_t n);


#endif

extern void *	et_deb(void ** start, uns eltype, size_t *size);

#if POCKET_H_INCLUDED

extern void *	p_strchr(PP p,uns c);
extern void *	p_strnchr(PP p,uns c,size_t);
extern int 	p_strncmp(PP p, const void *str, uns eltype,size_t len);
extern int 	p_strncmp_p(PP to, PP from, size_t len);
extern int 	p_stricmp(PP p, const void *str, uns eltype);
extern int 	p_strnicmp(PP p, const void *str, uns eltype,size_t len);
extern int 	p_strcmp(PP p, const void *str, uns eltype);

#define p_strcmp_g(p,s)		p_strcmp(p,s,APLTCHAR)
#define p_strncmp_g(p,s,l)	p_strncmp(p,s,APLTCHAR,l)
#define p_stricmp_g(p,s)	p_stricmp(p,s,APLTCHAR)
#define p_stricmp_s(p,s)	p_stricmp(p,s,APLSYMCHAR)
#define p_strnicmp_g(p,s,l)	p_strnicmp(p,s,APLTCHAR,l)

#endif // POCKET_H_INCLUDED

#define s_strlen(s)			et_strlen(s, APLSYMCHAR)


#if defined(WINDOWS3) && HAS_UNICODE
// optimizations

#define g_strcpy_s(to,fm)		wcscpy(to,fm);
#define g_memcpy_s(to,fm,len)		memcpy(to,fm,(len)*sizeof(gchar));

#else

#define g_strcpy_s(to,fm)		et_strcpy_et(to,APLGUICHAR,fm, APLSYMCHAR)
#define g_memcpy_s(to,fm,len)		et_memcpy_et(to,APLGUICHAR,fm,APLSYMCHAR,len)

#endif


#define g_strcpy_a(to,fm)		et_strcpy_et(to,APLGUICHAR,fm, APLCHAR)
#define g_strcpy_i(to,fm)		et_strcpy_et(to,APLGUICHAR,fm, APLCHAR8)
#define a_strcpy_i(to,fm)		et_strcpy_et(to,APLCHAR   ,fm, APLCHAR8)
#define a_strcpy_g(to,fm)		et_strcpy_et(to,APLCHAR   ,fm, APLGUICHAR)
#define a_strcpy_s(to,fm)		et_strcpy_et(to,APLCHAR   ,fm, APLSYMCHAR)
#define s_strcpy_g(to,fm)		et_strcpy_et(to,APLSYMCHAR,fm, APLGUICHAR)
#define i_strcpy_g(to,fm)		et_strcpy_et(to,APLCHAR8,fm, APLGUICHAR)

#define g_memcpy_a(to,fm,len)		et_memcpy_et(to,APLGUICHAR,fm,APLCHAR   ,len)
#define a_memcpy_g(to,fm,len)		et_memcpy_et(to,APLCHAR   ,fm,APLGUICHAR,len)
#define a_memcpy_a(to,fm,len)		et_memcpy_et(to,APLCHAR   ,fm,APLCHAR,len)
#define s_memcpy_a(to,fm,len)		et_memcpy_et(to,APLSYMCHAR,fm,APLCHAR   ,len)
#define a_memcpy_s(to,fm,len)		et_memcpy_et(to,APLCHAR   ,fm,APLSYMCHAR,len)
#define g_memcpy_i(to,fm,len)		et_memcpy_et(to,APLGUICHAR,fm,APLCHAR8,len)
#define a_memcpy_i(to,fm,len)		et_memcpy_et(to,APLCHAR,fm,APLCHAR8,len)
#define a_memcpy_et(to,fm,et,len)	et_memcpy_et(to,APLCHAR   ,fm,et        ,len)
#define g_memcpy_et(to,fm,et,len)	et_memcpy_et(to,APLGUICHAR,fm,et        ,len)
#define s_memcpy_et(to,fm,et,l)		et_memcpy_et(to,APLSYMCHAR,fm,et,l)

#define astrncmp(to,fm,len)		et_strncmp_et(to,APLCHAR  ,fm,APLCHAR,len)

#define astrcspn(to,fm)			et_strcspn(to,APLCHAR  ,fm,APLCHAR)
#define a_strcspn_g(to,fm)		et_strcspn(to,APLCHAR  ,fm,APLGUICHAR)

#define   astrcmp(to,fm)	       	et_strcmp_et(to,APLCHAR   ,fm,APLCHAR)
#define  a_strcmp_g(to,fm)		et_strcmp_et(to,APLCHAR,fm,APLGUICHAR)
#define  i_strcmp_a(to,fm)		et_strcmp_et(to,APLCHAR8,fm,APLCHAR)
#define et_strcmp_g(to,et,fm)		et_strcmp_et(to,et,fm,APLGUICHAR)
#define et_strcmp_s(to,et,fm)		et_strcmp_et(to,et,fm,APLSYMCHAR)
#define  s_strcmp_g(to,fm)		et_strcmp_et(to,APLSYMCHAR,fm,APLGUICHAR)
#define  s_strcmp_s(to,fm)		et_strcmp_et(to,APLSYMCHAR,fm,APLSYMCHAR)
#define  g_strcmp_s(to,fm)		et_strcmp_et(to,APLGUICHAR,fm,APLSYMCHAR)

#define et_strncmp_g(to,et,fm,l)	et_strncmp_et(to,et,fm,APLGUICHAR,l)
#define et_strncmp_s(to,et,fm,l)	et_strncmp_et(to,et,fm,APLSYMCHAR,l)
#define a_strncmp_g(to,fm,l)		et_strncmp_et(to,APLCHAR,fm,APLGUICHAR,l)
#define s_strncmp_s(to,fm,l)		et_strncmp_et(to,APLSYMCHAR,fm,APLSYMCHAR,l)

#define astrchr(to,ch)	        	et_strchr(to,APLCHAR   ,ch)
#define astrrchr(to,ch)	        	et_strrchr(to,APLCHAR  ,ch)
#define symstrrchr(to,ch)        	et_strrchr(to,APLSYMCHAR,ch)

#define amemcmp(to,fm,len)		et_memcmp_et(to,APLCHAR   ,fm,APLCHAR,len)

#define a_strncpy_g(to,fm,len)		et_strncpy_et(to,APLCHAR  ,fm, APLGUICHAR,len)
#define g_strncpy_a(to,fm,len)		et_strncpy_et(to,APLGUICHAR, fm, APLCHAR,len)
#define g_strncpy_s(to,fm,len)		et_strncpy_et(to,APLGUICHAR, fm, APLSYMCHAR,len)
#define s_strncpy_s(to,fm,len)		et_strncpy_et(to,APLSYMCHAR, fm, APLSYMCHAR,len)

#define a_strcat_g(to,fm)		et_strcat_et(to,APLCHAR   ,fm, APLGUICHAR)
#define g_strcat_s(to,fm)		et_strcat_et(to,APLGUICHAR   ,fm, APLSYMCHAR)
#define s_strcat_g(to,fm)		et_strcat_et(to,APLSYMCHAR   ,fm, APLGUICHAR)
#define g_strcat_a(to,fm)		et_strcat_et(to,APLGUICHAR   ,fm, APLCHAR)
#define g_strcat_i(to,fm)		et_strcat_et(to,APLGUICHAR   ,fm, APLCHAR8)

#define et_stricmp_g(to,et,fm)		et_stricmp_et(to,et,fm,APLGUICHAR)
#define et_stricmp_s(to,et,fm)		et_stricmp_et(to,et,fm,APLSYMCHAR)
#define s_stricmp_g(to,fm)		et_stricmp_et(to,APLSYMCHAR,fm,APLGUICHAR)
#define a_stricmp_g(to, fm)		et_stricmp_et(to,APLCHAR,fm,APLGUICHAR)
#define astricmp(to, fm)		et_stricmp_et(to,APLCHAR,fm,APLCHAR)

#define et_strnicmp_g(to,et,fm,l)	et_strnicmp_et(to,et,fm,APLGUICHAR,l)
#define et_strnicmp_i(to,et,fm,l)	et_strnicmp_et(to,et,fm,APLCHAR8,l)
#define et_strnicmp_s(to,et,fm,l)	et_strnicmp_et(to,et,fm,APLSYMCHAR,l)

#if 1 // HAS_UNICODE

	#define a_alloca_g(ptr,name) 					\
			{						\
			size_t __len = 1+gstrlen(name);			\
			ptr=(achar*)alloca(__len*sizeof(achar));	\
			a_memcpy_g(ptr,name,__len);			\
			}				
	#define g_alloca_a(ptr,name,len) 				\
			{						\
			ptr=(gchar*)alloca(len*sizeof(gchar));	\
			g_memcpy_a(ptr,name,len);			\
			}				

	#define g_alloca_sym(ptr,name) 					\
			{						\
			size_t __len = 1+symlen(name);			\
			ptr=(gchar*)alloca(__len*sizeof(achar));	\
			g_memcpy_s(ptr,name,__len);			\
			}				
	#define a_alloca_sym(ptr,name) 					\
			{						\
			size_t __len = 1+symlen(name);			\
			ptr=(achar*)alloca(__len*sizeof(achar));	\
			a_memcpy_s(ptr,name,__len);			\
			}				
	#define sym_alloca_a(ptr,name,len) 				\
			{						\
			ptr=(symchar*)alloca(len*sizeof(symchar));	\
			s_memcpy_a(ptr,name,len);			\
			}				
#elif	IS_INTEROP_CODE	// eg partner.c
#else			// classic interpreter only has one byte chars

	extern void *memrchr(const void *cp,int c,size_t len);
	extern ubyte *strnchr(ubyte *cp,int c,size_t len);
	extern int t_stricmp(ubyte *l,ubyte *r);
	extern int t_strcmp(ubyte *l,ubyte *r);


	#define a_alloca_sym(ptr,name)     ((ptr)=(achar  *)(name))
	#define sym_alloca_a(ptr,name,len) ((ptr)=(symchar*)(name))

	#define g_alloca_sym(ptr,name)     ((ptr)=(achar  *)(name))
	#define sym_alloca_g(ptr,name,len) ((ptr)=(symchar*)(name))

	#define a_alloca_g(ptr,name)       ((ptr)=(achar  *)(name))
	#define g_alloca_a(ptr,name,len)   ((ptr)=(gchar  *)(name))

	#undef et_strncpy_et
	#define et_strncpy_et(a,b,c,d,n)	strncpy(a,c,n)

	#undef et_strcpy_et	
	#define et_strcpy_et(a,b,c,d)	strcpy(a,c)

	#undef et_strcat_et
	#define et_strcat_et(a,b,c,d)	strcat(a,c)

	#undef et_strlen
	#define et_strlen(a,b)	strlen(a)

	#undef et_memset
	#define et_memset(a,b,c,d)	memset(a,c,d)

	#undef et_stricmp_et
	#define et_stricmp_et(a,b,c,d)	((d==APLTCHAR)?t_stricmp(a,c):strcasecmp(a,c))

	#undef et_strcmp_et
	#define et_strcmp_et(a,b,c,d)	((d==APLTCHAR)?t_strcmp(a,c):strcmp(a,c))

	#undef et_strncmp_et
	#define et_strncmp_et(a,b,c,d,n)	strncmp((achar*)a,c,n)

	#undef et_strnicmp_et
	#define et_strnicmp_et(a,b,c,d,n)	avstrnicmp((achar*)a,(achar*)c,n)

	#undef et_memcpy_et
	#define et_memcpy_et(a,b,c,d,n)	memcpy(a,c,n)

	#undef et_memcmp_et
	#define et_memcmp_et(a,b,c,d,n)	memcmp(a,c,n)

	#undef et_strchr
	#define et_strchr(a,b,c)	strchr(a,c)

	#undef et_strstr_et
	#define et_strstr_et(a,b,c,d)	strstr(a,c)

	#undef et_strnchr
	#define et_strnchr(a,b,c,n)	strnchr((ubyte*)a,c,n)

	#undef et_strrchr
	#define et_strrchr(a,b,c)	strrchr(a,c)

	#undef et_memchr
	#define et_memchr(a,b,c,n)	memchr(a,c,n)

	#undef et_memrchr
	#define et_memrchr(a,b,c,n)	memrchr(a,c,n)
#endif

#define asizeof(buf)			(sizeof(buf)/sizeof(achar))

#ifdef __cplusplus
}
#endif 

#endif // _CHARFNS_H
