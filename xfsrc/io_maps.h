#ifndef _IO_MAPS_H
#define _IO_MAPS_H

/*
	Defines the structure of a compiled keyboard map file
*/

#include <sys/types.h>
#include <unique_fid.h>

/*
	Defines the structure of a compiled input translate map file

	┌───────┬───────┬───────┬───────┬───────┬───────┬───────┐
in_fd	│	│   *	│   *	│	│	│   *	│	│ one per
	└───────┴───┬───┴───┬───┴───────┴───────┴───┬───┴───────┘ _NFILE
		    │	    │			    │
		    └───┬───┘			    │
			│			    │
	┌───────────────┴───────┐	┌───────────┴───────────┐
Imap	│			│	│			│ one per
	│			│	│			│  device/inode
	│			│	│			│
	│ map[16]		│	│			│
	│ ****************	│	│ 			│
	└───────┼───────────────┘	└───────────────────────┘
		│				
		│
	┌───────┴───────┐
Mmap	│		│ one per mode in the file
	│		│
	│		│
	│		│
	│		│
	│		│
	└───────────────┘

*/
typedef 
#if	HAS_UNICODE
	int32_t
#else //HAS_UNICODE
	int16_t
#endif//HAS_UNICODE
		result_t;
typedef int16_t COI;
typedef struct
	{
	ubyte istemp;		// is a temporary mode ? 
	ubyte doistrip;		// strip top bit ? 
	ubyte isatty;		// attached to a tty 
	ubyte utfwidth;		// 0 => no utf, 8 or 16: replicated from Imap
	ubyte direct[32];	// 256 bits 1 => return char is in 
	COI char_or_index[256]; // here else this contains row no 
	uint16_t rows;		// rows in strs 
	uint16_t cols;		// cols in strs 
	result_t *result;	// pntr into dynamic area 
	UCH strs[sizeof(void *)];	// dynamic area 
	} Mmap;
	
typedef struct 
	{
	union
		{
		ubyte magic[2];		// Magic number for file(1) 
		UNIQUE_FID unique_fid;	// file identifier
		} header;
	ubyte n_modes;			// Number of Modes 
	ubyte weak_int;			// Weak interrupt character 
	ubyte strong_int;		// Strong interrupt character 
	ubyte prog_mode;			// Mode for programmer input 
	ubyte user_mode;			// Mode for user input 
	ubyte utfwidth;			// 0 => no utf, 8 or 16
	ubyte caps_on;			// flag caps on 
	uint16_t n_labs;		// How many labels are defined 
	ubyte is_lower[32];		// space for 256 bits 
	ubyte caps_xchg[256];		// Caps lock exchange table 
	Mmap *mode[16];			// pntrs into dynamic area 
	achar *labinfo;			// pntr into dynamic area 
	void (LIBCALL *freefn)(void *); // function to free
	} Imap;

struct Fd_map
	{
	Imap *map;
	ubyte curr_mode[2];	// [0] <-> programmer, [1] <-> user 
	};

	/* eg  
		k = inkey((int)fd, (unsigned)PROG);
	*/
	/* eg
		was = keyset((int)fd, (unsigned)willbe, (unsigned)USER);
	*/
	/* eg
		l = inlab(0, 5)
	*/

extern struct Fd_map in_fd[];


/*
	Defines the structure of a compiled output translate map file

	+-------------------------------------------------------+
o_out	|	|   *	|   *	|	|	|   *	|	| one per
	+-------------------------------------------------------+ _NFILE
		    |	    |			    |
		    +-------+			    |
			|			    |
	+-----------------------+	+-----------------------+
struct	| 			|	|			| one per
 Out_fd	|			|	|			|  device/inode
	|			|	|			|
	|			|	|			|
	|			|	|			|
	|			|	|			|
	| *t_out		|	| *t_out		|
	+-----------------------+	+-----------------------+
		|				|
		|				|
		+-------------------------------+
				|
				|
			+---------------+
struct Translate	|		| one per translate file required
			|		|
			|		|
			|		|
			|		|
			|		|
			+---------------+

*/

struct Sas				// Special Action String 
	{
	short code;			// suitable negative code eg B3 
	unsigned short offset;		// offset from strstart		
	unsigned short len;		// length of string (keep alignment) 
	};


struct Translate
	{
	unsigned refcount;		// number of Out_fd's pointing here
	UNIQUE_FID unique_fid;		// file identifier
	unsigned nmodes;		// number of modes		
#if	HAS_UNICODE
	#define MAX_NVALUES	256	// arbitrary maximum but watch out for CC
		unsigned nvalues;	// number of used value slots
		achar values
			[
			MAX_NVALUES	// available slots
		+	1		// force one free slot at the end
			];
#endif//HAS_UNICODE
	unsigned utfwidth;		// don't break the alignment
	unsigned nsas;			// number of non-moded strings	
	struct Sas *special;		// special action strings	
	short unsigned *offset;		// point to start of offsets	
	ubyte *lengths;			// point to start of lengths	
	UCH *strstart;			// point to start of strings	
	ubyte dynamic[1];		// dynamic area			
	};

struct Out_fd
	{
	struct	Translate *t_out;	// selected translate table	
	UNIQUE_FID unique_fid;		// file identifier
	ubyte out_used;			// used this dev for translated output
	unsigned short curr_col;	// current col to handle line feeds
	ubyte unused;
	};

/* I am outlawing the possibility of talking to the same device with different
 * translate tables
 */

extern struct Out_fd *o_out[];

struct Datemap
	{
	UNIQUE_FID unique_fid;		// file identifier
	unsigned n_days;		// number of weekday strings	
	unsigned n_months;		// number of month strings	
	unsigned n_formats;		// number of format strings	
	unsigned n_labels;		// number of format labels	
	struct Sas *strings;		// special action strings	
	UCH *strstart;			// point to start of strings	
	UCH dynamic[1];			// dynamic area			
	};

#endif // _IO_MAPS_H
