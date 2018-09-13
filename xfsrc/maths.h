#ifndef _MATHS_H
#define _MATHS_H

#define _USE_MATH_DEFINES
#include <math.h>

#if	RIOS
	#undef	isfinite
	#define	isfinite	_isfinite
#endif//RIOS

#endif // _MATHS_H
