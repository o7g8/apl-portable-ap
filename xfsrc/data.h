typedef UCP 	FCP;
typedef void *			FVP;
typedef FVP *			FVPP;

extern void putA(FVP fvp, ARRAY *ptr);
extern ARRAY *getA(FVP fvp);		// get(possibly nested) array from apl	

extern void freeA(void);
