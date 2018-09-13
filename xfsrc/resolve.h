#ifndef _RESOLVE_H
#define _RESOLVE_H

typedef struct 
	{
	unsigned int tag;
	unsigned int resolved;

	unsigned short first;
	unsigned short second;

	unsigned a;
	unsigned b;
	} overstrike;

extern size_t convlen(void *from, uns eltype);
extern unsigned int convtoav(achar *to,const void *from,uns eltype,size_t max);
extern unsigned int resolve(unsigned int a,unsigned int b);
extern size_t overstrike_candidates(overstrike *buf, size_t blen, unsigned c);

#endif // _RESOLVE_H
