#ifndef _ALLOCA_H_H
#define _ALLOCA_H_H

// Include whatever is needed to declare alloca().
#if MSC
# include <malloc.h>
#elif RIOS && __xlC__
// On AIX xlc complains about an "incorrect pragma" in <alloca.h>.
#else
# include <alloca.h>
#endif

#endif // _ALLOCA_H_H
