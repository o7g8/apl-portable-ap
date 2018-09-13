//	m k a r r a y	

#include <apl.h>
#include <errno.h>
#if	UNIX
	#if	_XOPEN_SOURCE < 600
		#undef _XOPEN_SOURCE
		#define	_XOPEN_SOURCE	600	// want posix_memalign()
	#endif//_XOPEN_SOURCE
	#include	<stdlib.h>
#else //UNIX
	#include	<malloc.h>
	static	int posix_memalign
		(
		void **addr
	,	size_t alignment
	,	size_t nbytes
		)
		{
		void *got = _aligned_malloc(nbytes, alignment);

		if(got)
			{
			*addr = got;
			return 0;
			}
		switch(errno)
			{
		case ENOMEM:
			return ENOMEM;
		default:
			return EINVAL;
			}
		}
#endif//UNIX

#include <string.h>
#include <support.h>

static void (*array_err_fn)(int);

void install_array_err_fn(void (*fn)(int))
{
array_err_fn=fn;
}

static ARRAY *array_err(int err)
{
if (array_err_fn)(*array_err_fn)(err);
return (ARRAY *)0;
}

static UCH SHIFT[NELTYPES] =	// to convert no. of elements to no. of bits 
{	3, 	// APLCHAR 
	0, 	// APLBOOL 
	3, 	// APLSINT 
	4, 	// APLINTG 
	5, 	// APLLONG 
	6, 	// APLDOUB 
	PNTRlog,// APLPNTR 
	3, 	// APLWCHAR8 
	4, 	// APLWCHAR16 
	5, 	// APLWCHAR32 
	7,	// APLCMPX
	0,	// APLRATS
	7,	// APLDECF
	0,	// unused
	7,	// APLDECF
};

ARRAY *alloc(BOUND nwds, unsigned eltype, unsigned rank)
{
	ARRAY *array;
	MEMORYINT *allocated;
	BOUND extended;

	// always ask for 16 byte aligned
	// free_array() can just zap last 4 bits to get real malloced addr
	// always allocate data on "natural" boundary, overkill for CMPX
	// if we will need extra then always allocate 16 bytes extra
	// assume sizeof(void *) == sizeof(MEMORYINT) == 1 << PNTRlog - 3

	int log_align = SHIFT[eltype];		// signed please
	int add;

	log_align -= PNTRlog;
	if(0 >= log_align)
		{				// alignment is OK
		if(posix_memalign((void *)&array, 16, nwds*sizeof(MEMORYINT)))
			array_err(LIMIT);
		return array;
		}
		
	add = (int)offsetof(ARRAY, shape[rank]);	// in bytes
	add >>= PNTRlog;				// in MEMORYINT
	add &= (1 << log_align) - 1;
	extended = nwds;
	extended += add;
	if(extended < nwds)		// overflow ?
		array_err(LIMIT);
	if(posix_memalign((void *)&allocated, 16, extended*sizeof(MEMORYINT)))
		array_err(LIMIT);

	return (ARRAY *)(allocated + add);
}

ARRAY *mkarray
	(					// make an array in memory 
	unsigned eltype,			// element type 
	unsigned rank,				// array rank 
	BOUND *shape				// pointer to shape vector 
	)
	{
	BOUND bound = 1, nwds, *ptr, was;
	unsigned u;
	ARRAY *array;
	unsigned overflow = 0;
	int ishift;

	if(eltype >= NELTYPES)
		return array_err(EDOMAIN);
	if(rank   >  MAXRANK )
		return array_err(RANK);

	for(ptr = shape, u = rank; u--; ++ptr)	// calc no of elements as 
		{
		BOUND this = *ptr;

		if(this)
			{
			was = bound;
			bound *= *ptr;		// product of shape words 
			overflow |= was != (bound / *ptr);
			}
		else				// special case zero
			{
			overflow = 0;
			bound = 0;
			break;
			}
		}

	if (overflow)
		return array_err(LIMIT);

	ishift = SHIFT[eltype] - PNTRlog;
	if(0 <= ishift)
		{
		static BOUND maxima[] =
			{
#if	5 == PNTRlog
			0x80000000
		,	0xc0000000
		,	0xe0000000
		,	0xf0000000
		,	0xf8000000
		,	0xfc000000
#elif	6 == PNTRlog
			0x8000000000000000
		,	0xc000000000000000
		,	0xe000000000000000
		,	0xf000000000000000
		,	0xf800000000000000
		,	0xfc00000000000000
#else
#	error	extra code needed here
#endif//PNTRlog
			};

		if(bound & maxima[ishift])
			return array_err(LIMIT);
		nwds = bound << ishift;		// nwds >= bound
		}
	else
		{
		ishift = -ishift;
		nwds = bound >> ishift;
		nwds += 0 != (bound - (nwds << ishift)); // round up if req
		}
	was = nwds;
	nwds +=					// add extras
		2				// length and zones
		+ rank				// shape vector		
		+ (eltype == APLPNTR && bound == 0);	// prototype if null nested 
	if (was > nwds)				// overflow
		return array_err(LIMIT);

	array = alloc(nwds, eltype, rank);	// allocate space for array 
						// and fill in type ...	
	if(eltype == APLPNTR)			// zero so freearray() works 
		memset(array, 0, nwds * sizeof(MEMORYUNS));
	else
		memset(array, 0, 2 * sizeof(MEMORYUNS)); // zap zones
	array->type = (eltype == APLPNTR ? NESTED : SIMPLE);
	array->length = nwds;			// length		
	array->eltype = eltype;			// element type		
	array->rank = rank;			// rank			

	memcpy(array->shape, shape, rank * sizeof(MEMORYUNS));

	return array;
	}

int released(UCH *thing)
{
	int retn=0;
#if 0

	typedef void (LIBCALL *_invalid_parameter_fn)(const wchar_t *,const wchar_t *,const wchar_t *,unsigned int,uintptr_t);

	_invalid_parameter_fn old = _set_invalid_parameter_handler(fail_free);

	if (setjmp(fail_free_buf))
		retn=1;
	else
	{
	if (_msize(thing) == ~(size_t)0)
		retn = 1;
	}
	_set_invalid_parameter_handler(old);
#endif

	return retn;
}

ARRAY *freearray(ARRAY *thing)			// return an ARRAY to	
{						// the free space pool	
	unsigned n, n_elts = 1;
	ARRAY **ptr;
	void *malloc_pntr = thing;

	if(thing == 0)				// don't try to free	
		return(0);			// if arg is null ptr	

	malloc_pntr = (void *)((IPTR)malloc_pntr & -16);

#if	MSC
	if (released(malloc_pntr))		// invalid or already freed
		return 0;			// best guess for MS libraries
#endif//MSC

	switch(thing->type)
	{
	default:
		// hopefully already freed
		break;
	case NESTED: 				// recursively free	
						// subarrays first	
		for(n = thing->rank; n--;)
			n_elts *= thing->shape[n];

		n_elts += 0 == n_elts;		// include prototype [8832]
		ptr = &data(thing, ARRAY *)[n_elts];
		while(n_elts--)			// free subarrays in	
			freearray(*--ptr);	// reverse order	
	case SIMPLE:
		thing->type = 0; // we hope that free() does not mess with this
				 // enough to give NESTED or SIMPLE
#if	UNIX
		free		// allocated with posix_memalign()
#else
		_aligned_free	// allocated with _aligned_malloc()
#endif		
			(malloc_pntr);	// free this array	
	}
	return 0;
}


/*	a r r a y b o u n d
	return number of elements in an array
	null nested arrays have one element anyway
*/

BOUND	arraybound(ARRAY *array)
{
	BOUND	bd, n, *p;

	bd = 1;
	p = array->shape;

	for(n = array->rank; n--; ++p)
		bd *= *p;

	if(array->eltype == APLPNTR && !bd) bd = 1;
	return bd;
}

/*	a r r a y s t a r t
	return pointer to first byte of data portion
*/

void *arraystart(ARRAY *array)
{
	return	array->shape + array->rank;
}

