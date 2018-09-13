
// assume version 8 ie NT or WIN95 and Watcom 10.5
// means one segment and all pointers are nice and easy
// this means that the F in FVP FCP doesn't mean FAR

#include <apl.h>
#include <support.h>
#include <errno.h>
#include <data.h>
#include <string.h>

#if	UNIX
	#if	_XOPEN_SOURCE < 600
		#undef	_XOPEN_SOURCE
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


#if  0
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
	PLENGTH extended;

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
			return 0;
		return array;
		}
		
	add = (int)offsetof(ARRAY, shape[rank]);	// in bytes
	add >>= PNTRlog;				// in MEMORYINT
	add &= (1 << log_align) - 1;
	extended = nwds;
	extended += add;
	if(extended < nwds)		// overflow ?
		return 0;
	if(posix_memalign((void *)&allocated, 16, extended*sizeof(MEMORYINT)))
		return 0;

	return (ARRAY *)(allocated + add);
}
#endif

static void free_align(void *vp)
	{
	intptr_t ip = (intptr_t)vp;

	ip >>= 4;
	ip <<= 4;
	free((void *)ip);
	}

#define Su (sizeof(MEMORYUNS))
typedef size_t RDWRFN(int, void *, size_t);


// the functions data_read, data_write are covers
// that look like read() and write().
// The "pipe" argument is ignored
// data_size() looks like data_write() but only counts the size
// writing will be a two pass operation

static FCP data_ptr;

static void data_seek(FVP fvp)
	{
	data_ptr = fvp;
	}

static  size_t data_read(int pipe, void * vp,  size_t len)
	{
	memcpy((FVP)vp, data_ptr, len);
	data_ptr += len;
	return len;
	}

static  size_t data_write(int pipe, void *vp,  size_t len)
	{
	memcpy(data_ptr, (FVP)vp, len);
	data_ptr += len;
	return len;
	}

static  size_t data_size(int pipe, void *vp,  size_t len)
	{
	data_ptr += len;
	return len;
	}


static BOUND d_fromapl(void)
	{
	BOUND value;

	(void)data_read(0, &value, sizeof(value));
	return value;
	}

static void d_toapl(RDWRFN *rdwrfn, MEMORYUNS value)
	{
	(void)(*rdwrfn)(0, &value, sizeof(value));
	}

ARRAY *getA(FVP fvp)		// get(possibly nested) array from apl	
	{
	FVPP fvpp = (FVPP)fvp;
	unsigned rank;
	BOUND *shape;
	ARRAY *array, **ptr;
	size_t nwords;			// number of words remaining to read	
	ARRAY temp;

	data_seek(*fvpp);		// position pointer to start of data

	if((nwords = d_fromapl()) == 0)	// read word count		
		return(0);		// left arg omitted		
	fields(&temp) = d_fromapl();	// read type/rank/eltype	

	array = alloc(--nwords, temp.eltype, temp.rank);// adjust word count &
					// allocate space for array	
	shape = &(array->shape[0]);	// take pointer to shape	
	array->length = nwords;		// fill in array length		
	fields(array) = fields(&temp);	// and type/rank/eltype		

	nwords -= 2;			// decr words to be read	

	if(array->type == SIMPLE)
		(*data_read)(0, shape, Su * nwords);// simple array 
	else
		{
		rank = array->rank;
		(*data_read)( 0, shape, Su * rank);// read shape	
		nwords -= rank;			// ... and decr count	

		/*	now recursively read in sub-arrays and
			install pointers in parent array.
		*/
		for(ptr = data(array, ARRAY *); nwords--;)
			*ptr++ = getA(&data_ptr);
		}
	return array;			// return pointer to array	
	}


static void sub_putA(RDWRFN *rdwrfn, ARRAY *array)
	{
	ARRAY **ptr;
	size_t rank, nwords;

	if(array == 0)
		d_toapl(rdwrfn, 0);		// null result		
	else
		{
		d_toapl(rdwrfn, nwords = array->length+1);// pipe length word 
		if(array->type == SIMPLE)
			(*rdwrfn)( 0, &fields(array), Su * (nwords-2));
		else
			{
			rank = array->rank;
			// pipe and shape 
			(*rdwrfn)( 0, &fields(array), Su * (rank+1));
			nwords -= rank+1+2;	// words remaining	
			// recursively put each subarray		
			for(ptr = data(array, ARRAY *); nwords--;)
				sub_putA(rdwrfn, *ptr++);
			}
		}
	}

typedef struct Chain
	{
	struct Chain	*next;
	FVP		data;
	} CHAIN;

static CHAIN *head;

void freeA(void)
	{
	CHAIN *this;
	CHAIN *next;

	for
		(
		next = head
	,	head = 0
	;	this = next
	;	next = this->next
	,	free_align(this->data)
	,	free_align(this)
		);
	}

static void add_chain(FVP data)
	{
	CHAIN *this;
	int ignore;

	ignore = posix_memalign((void *)&this, 16, sizeof(CHAIN));
	this->next = head;
	head = this;
	this->data = data;
	}

void putA(FVP fvp, ARRAY *ptr)
	{
	FVPP fvpp = (FVPP)fvp;
	FVP data;
	size_t size_of_data;
	int ignore;

	// pass one size the data
	data_seek(0);
	sub_putA(data_size, ptr);
	size_of_data = data_ptr - (FCP)0;

	// allocate some space for it
	ignore = posix_memalign((void *)&data, 16, size_of_data);
	
	// pass two fill it in
	data_seek(data);
	sub_putA(data_write, ptr);

	// tell APL where it is
	*fvpp = data;
	add_chain(data);		// so we can free it later
	}
