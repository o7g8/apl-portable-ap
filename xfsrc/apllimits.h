#ifndef _APLLIMITS_H
#define _APLLIMITS_H
				// Maximum:
#define MAXRANK 15		//  rank of array
/* limited only by ⎕WA */	//  number of names in symbol table
/* limited only by ⎕WA */	//  stack (state-indicator) depth
/* limited only by ⎕WA */	//  depth of nested array
/* 255 (8 bits)        */	//  control-structure nesting depth
/* 255 (8 bits)        */	//  dfn/dop nesting depth
#define SPACE_LIMIT 256		//  namespace depth. 
#define     NFILES  512		//  number of component file ties
#define MAX_NFILES  128		//  number of native file ties
/*  4095 (12 bits)     */	//  number of names+constants in function body
/* 65535 (16 bits)     */	//  size of function body (max token offset).
/*  4095 (12 bits)     */	//  number of lines in function
/* posix FILENAME_MAX  */	//  length of file name

#define VIDCS_SIZE	2048	// should any of these
#define JWMINP_MAX	2048	// ever differ
#define VFB_MAX 	2048	// from each other?

#define JWM_MAXWINS	255	// maximum number of open edit windows
#define GUI_MDI_LIMIT	100	// maximum number of GUI MDI children


#define SYMBOL_MAX	INT_MAX // MAX characters in a symbol

#endif // _APLLIMITS_H

