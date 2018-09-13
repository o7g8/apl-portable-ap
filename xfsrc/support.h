
//	Copyright (c) 1982 Dyadic Systems Limited	

//	support.h - header for programs using functions in 'support.c'	
//	startup(), alloc(), getarray(), putarray(), freearray() etc.	

extern	sigjmp_buf	errbuff;// main should 'sigsetjmp(errbuff)' where	
				// it wants to re-enter from error()	

				// non-integer functions in support.c	

#if	__cplusplus
extern "C" 
{
#endif//__cplusplus

ARRAY		*getarray(void);
ARRAY		*mkarray(unsigned, unsigned, BOUND *);
ARRAY		*freearray(ARRAY *);
ARRAY		*throwarray(ARRAY *);
ARRAY		*alloc(BOUND, unsigned, unsigned);
BOUND		fromapl(void);
BOUND		arraybound(ARRAY *);
void		*arraystart(ARRAY *);
void 		putarray(ARRAY *);
void		startup(unsigned);
void		define(gchar*, int, int);
void		filerror(void);
void NORETURN   error(int errnumber);
void		toapl(BOUND n);
BOUND		fromapl(void);

#if	__cplusplus
}
#endif//__cplusplus

void install_array_err_fn(void (*fn)(int));

uns interpreter_space(void);
uns interpreter_unicode(void);
				// useful macros 			
#define	data(array, type)	((type *)&array->shape[array->rank])
#define	fields(array)		(((MEMORYUNS *)(array))[1])

 

//	Copyright (c) 1982 Dyadic Systems Limited	

