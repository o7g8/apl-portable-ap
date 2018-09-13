#ifndef _ERROR_H
#define _ERROR_H

#include <stdarg.h>

//	Copyright (c) 1982 Dyadic Systems Limited	

#define EWSFULL		1
#define ESYNTAX		2
#define EINDEX		3
#define ERANK		4
#define ELENGTH		5
#define EVALUE		6
//#define EFORMAT	7	not reffed, clashes with sys/errno.h 
#define ELIMIT		10
#define EDOMAIN		11
#define EHOLD		12
#define EOPTION 	13		// unused
#define ENONCE		16
#define EACCESS		17


#define EFILEFIRST	18
#define	FLTIE		18
#define	FLACCESS	19
#define	FLINDEX		20
#define	FLFULL		21
#define	FLNAME		22
#define	FBADFILE	23
#define	FLTIED		24
#define	FLREMOTE	25
#define	FSYSERR		26
#define	FNAVAIL		28
#define	FSTQUOTA	30
#define	FLQUOTA		31
#define	FLNAMELIST	32
#define	FLSYSFULL	34
#define	FLCONVERT	35
#define EINCOMPAT	36	// issued by partner. file read most likely.
#define	FBADCOMP	38
#define EFILELAST	38
#define EFILEBASE	100

#define IsFileError(en)		(((en)>=EFILEFIRST && (en)<=EFILELAST) || ((en)>=EFILEBASE && (en)<1000))

#define INTERNALERRORS		32768U
#define PROTOTYPEERROR		32768U
#define CRASHFAILEDERROR	32769U
#define CRASHERROR 		32770U
#define COPYERROR 		32771U
#define JUSERROR 		32772U
#define OFFERROR 		32773U

#define EXTERNCUTBACKERROR 	32774U
#define EXTERNRESETERROR 	32775U
#define IOVERERROR		32776U
#define QUIETERROR		32777U

#define ESHELLEX		85
#define EFUTURE			86

#define EEXCEPTION		90
#define EDLLEXCEPTION		91
#define ETRANSLATION		92
#define EUNACCEPTABLECHAR	93
#define ENOTCLASSIC		94
#define EAFFIRM			98
#define EINTERNAL		99
#define EFILEBASE		100

#define	WSFULL_ERROR		error(EWSFULL)
#define SYNTAX_ERROR		error(ESYNTAX)
#define INDEX_ERROR		error(EINDEX)
#define RANK_ERROR		error(ERANK)
#define LENGTH_ERROR		error(ELENGTH)
#define VALUE_ERROR		error(EVALUE)
#define LIMIT_ERROR		error(ELIMIT)
#define DOMAIN_ERROR		error(EDOMAIN)
#define HOLD_ERROR		error(EHOLD)
#define OPTION_ERROR		error(EOPTION)		// unused
#define	NONCE_ERROR		error(ENONCE)
#define	ACCESS_ERROR		error(EACCESS)
#define	TRANSLATION_ERROR	error(ETRANSLATION)
#define	INTERNAL_ERROR		error(EINTERNAL)
#define	FUTURE_ERROR		error(EFUTURE)
#define EXCEPTION		error(EEXCEPTION)

#define domain()		DOMAIN_ERROR
#define domainx(x)		errorx(EDOMAIN,x);
#define nonce()			NONCE_ERROR
#define lengtherr()		LENGTH_ERROR
#define rankerr()		RANK_ERROR
#define syntax()		SYNTAX_ERROR
#define limit()			LIMIT_ERROR

#if 0 // removed for C++ compatibility
//#define value()		VALUE_ERROR		
//#define internal()		INTERNAL_ERROR
#endif

//			window error codes				
#define	CONT_RANK	52
#define	CONT_COLS	53
#define	POSN_ERR	54
#define	SIZE_ERR	55
#define	TYPE_MISMATCH	56
#define	TYPE_ERR	57
#define	ATT_RANK	58
#define	ATT_LENGTH	59
#define	NO_FULLSCR	60
#define	BAD_LINE_CODE	61
#define	KEY_RANK	62
#define	KEY_TYPE	63

#define FS_TIES_USED		error(72)
#define LOST_SLAVE		error(73)
#define P_TAB_FULL		error(76)
#define AP_EXIT			77
#define TRAPERR			84
#define ESTOP			1001
#define DOQSTOP			error(ESTOP)
#define WEAK_INTERRUPT		error(1002)

extern unsigned enabled;

#define STRONG_INTERRUPT	{enabled=0;error(1003);}
#define INPUT_INTERRUPT		error(1004)
#define EOF_INTERRUPT		error(1005)
#define TIMEOUT			error(1006)
#define TIMEOUTx(ENX)		errorx (1006, ENX)
#define RESIZE_ERR 		1007
#define DEADLOCK		error(1008)


#include <syserror.h>

extern void W3SysErrorReport(int saved, gchar *msg, gchar *file);

#define _S1(X	) 			do { X; } while (0)

#define Assert(en, c)			_S1 ( ifu(!(c))error(en) )
#define Assertx(en, enx, c)		_S1 ( ifu(!(c))errorx(en, enx) )
#define Assertxf(en, enx, c, ...)	_S1 ( ifu(!(c))errorxf(en, enx, __VA_ARGS__) )
#define Assertxo(en, enx, eno, c)	_S1 ( ifu(!(c))errorxo(en, enx, eno) )

extern void maybeerror(unsigned);

typedef struct
	{
	int type;
	int value;
	} DMX_ENO;

extern const DMX_ENO ENO_NULL;

typedef struct
	{
	int err;			// error number
	uns dmx;			// dmx error 
	DMX_ENO eno;			// errno/getlasterror
	} error_str_t;

extern error_str_t new_error_str_exo(int err, int dmx, DMX_ENO eno);

#define new_error_str_exo(err,dmx,eno)	new_error_str_exo(err,dmx,eno)
#define new_error_str_ex(err,dmx)	new_error_str_exo(err,dmx,ENO_NULL)
#define new_error_str_eo(err,eno)	new_error_str_exo(err,0,eno)
#define new_error_str_e(err)		new_error_str_exo(err,0,ENO_NULL)
#define new_error_str()			new_error_str_exo(0,0,ENO_NULL)

extern NORETURN void error_from_str(error_str_t retn);

extern DMX_ENO dmx_geterrno(void);
extern DMX_ENO dmx_getlasterror(void);
#define DMXERRNO dmx_geterrno ()

#if UNIX
#define DMXGETLASTERROR DMXERRNO
#else
#define DMXGETLASTERROR dmx_getlasterror()
#endif

#if AP_BUILD

extern C_EXTERN NORETURN void error(uns n);
#define errorx(en, enx)			error (en)
#define errorxf(en, enx, ...)		error (en)
#define erroro(en, eno)			error (en)
#define errorxo(en, enx, eno)		error (en)
#define errorxof(en, enx, eno, ...)	error (en)
#define errora(en, enx, eno, f, l)	error (en)
#define erroraf(en, enx, eno, f, l,...)	error (en)

#else

extern NORETURN void erroraf(unsigned en, unsigned enx, DMX_ENO eno, const char *file, unsigned line, ...);
extern NORETURN void verroraf(unsigned en, unsigned enx, DMX_ENO eno, const char *file, unsigned line, va_list args);

#define error(en)			errora  (en, DMX_NONE, ENO_NULL,   __FILE__, __LINE__)
#define errorx(en, enx)			errora  (en, enx,      ENO_NULL,   __FILE__, __LINE__)
#define errorxf(en, enx, ...)		erroraf (en, enx,      ENO_NULL,   __FILE__, __LINE__, __VA_ARGS__)
#define erroro(en, eno)			errora  (en, DMX_NONE, eno, __FILE__, __LINE__)
#define errorxo(en, enx, eno)		errora  (en, enx,      eno, __FILE__, __LINE__)
#define errorxof(en, enx, eno, ...)	erroraf (en, enx,      eno, __FILE__, __LINE__, __VA_ARGS__)
#define errora(en, enx, eno, f, l)	erroraf (en, enx,      eno, f,        l)

#endif

extern unsigned	weak,strong,resize;
#define any_weak weak
extern unsigned thread_weak(void);

typedef void (CALLED_BY_DWA_DLL*ERRORTASK)(uns en);
typedef void (CALLED_BY_DWA_DLL*DESCALCTASK)(int);

#define SAVEENV(e) memcpy(save##e,e,sizeof(parse_jmp_buf))
#define RESTOREENV(e) memcpy(e,save##e,sizeof(parse_jmp_buf))

#define SAVEENV_FS(e) 		memcpy(save##e,e,sizeof(jmp_buf))
#define RESTOREENV_FS(e)	memcpy(e,save##e,sizeof(jmp_buf))


#define JMPVAR(type, name)	volatile type name = 0
#define JMPINT(name)		JMPVAR(int,name)

extern ERRORTASK errortask;				
extern DESCALCTASK descalctask;				

#if	defined(ACCRUED_EXCEPTION)
	extern uns LIBCALL accrued_exception(void);
#else //defined(ACCRUED_EXCEPTION)
#	define	accrued_exception() 0
#endif//defined(ACCRUED_EXCEPTION)

extern uns decf_accrued_exception(void);
extern uns accrued_exceptions(void);

#define	testrong()	_S1 ( if(strong && enabled)STRONG_INTERRUPT )

#if GUI && defined(WINDOWS3)
# define SYSERROR_ATTR
#else
# define SYSERROR_ATTR NORETURN
#endif
extern SYSERROR_ATTR void syserror(unsigned int code);
extern SYSERROR_ATTR void syserror2(unsigned int code,int err);
extern SYSERROR_ATTR void syserrorfmt
	(
	unsigned int code
,	const gchar * user_fmt
,	const gchar * wsdump_fmt
,	...
	);
extern void (*syserrortask)(uns err);

extern int W3SysError(gchar *msg,gchar * file,int code);


#if	defined(AP_BUILD)
#include	<assert.h>
#define AssertSysError(assertion, syserrnum, errnum) assert(assertion)
#else	//defined(AP_BUILD)
#define AssertSysError(assertion, syserrnum, errnum) \
	_S1 ( if (!(assertion)) syserror2(syserrnum, errnum) )
#endif 	//defined(AP_BUILD)

extern void aplcoredump(gchar *file);

extern unsigned int weak_test(void);
extern void GetQException(void);

#define WSCHECK_WEAK 	0x00000001
#define WSCHECK_HARSH 	0x00000002
#define WSCHECK_DFLAG_W 0x00000004
#define WSCHECK_DFLAG_w 0x00000008
#define WSCHECK_DFLAG_S 0x00000010
#define WSCHECK_FROM_NQ 0x00000020

#define WSCHECK_W	(WSCHECK_HARSH|WSCHECK_DFLAG_W)
#define WSCHECK_w	(WSCHECK_HARSH|WSCHECK_DFLAG_w)
#define WSCHECK_S	(WSCHECK_WEAK |WSCHECK_DFLAG_S)
#define WSCHECK_NQ	(WSCHECK_HARSH|WSCHECK_FROM_NQ)

typedef uns WSCHECK_SEVERITY;

extern uns wscheck(WSCHECK_SEVERITY severity);
extern uns wscheck_with_return(WSCHECK_SEVERITY severity);
extern void WsCheckRequests(void);
extern void init_failure_message(const char *msg);
extern void w3error_error(void);
extern void w3limit_error(void);
extern void abort_dll(int code);

extern void LIBCALL strongint(int signo);
extern void LIBCALL weakint(int signo);
extern void LIBCALL winch(int signo);
extern void LIBCALL core_dump(int signo);

extern const gchar * limit_error_text;

#define TRAP_DISABLE_ALL 	1
#define TRAP_DISABLE_AT_SUSP 	2

extern unsigned TrapFlags;

extern void Raise_Exception(uns n);
extern size_t QEXCEPTMessage(gchar *emsg, size_t len);
extern unsigned no_error_messages_please;

extern NORETURN void error_with_msg(uns err, const gchar *msg);
#if HAS_UNICODE
extern NORETURN void error_with_char_msg (unsigned int err, const char *text);
#else // HAS_UNICODE
#define error_with_char_msg error_with_msg
#endif

#if !AP_BUILD
#include <qem.h>
#include <dmx_macros.h>
#endif

#include <affirm.h>

//	Copyright (c) 1982 Dyadic Systems Limited	

#endif // _ERROR_H
