

/*
 *
 *	This file is similar to /usr/include/regexp.h except that
 *	it has been modified to use the Dyalog apl character set.
 *	There are no signed characters used.
 *	This file depends on quadav.h
 *
 *	An extra parameter has been added to each of the functions 
 *	step() and advance()
 *
 *	The incoming string is not zero terminated. Instead the 
 *	new second parameter 'elp' points to the last character
 *	in the string. (NB not one character past the last)
 */

#define	CBRA		2
#define	CCHR		4
#define	CDOT		8
#define	CCL		12
#define	CDOL		20
#define	CCEOF		22
#define	CKET		24
#define	CBACK		36

#define	ASTERISK	01
#define	RNGE		03
#define LTHAN           40
#define	GTHAN		44
#define	CLCCHR		48	// caseless char , added 13 oct 2000 cvc

#define	NBRA		9


//EPSIZE is measured in number of achars. 
#if HAS_UNICODE
#define EPSIZE          (262144/sizeof(achar))
#undef AV_AU
#undef AV_ZU
#define AV_AU 0x24b6
#define AV_ZU 0x24cf
#else //HAS_UNICODE
#define EPSIZE          32
#endif //HAS_UNICODE

ubyte	bittab[] = 
		{
		1,
		2,
		4,
		8,
		16,
		32,
		64,
		128
		};

#define	NGETC()		(++count, GETC())
#define	NUNGETC(c)	(--count, UNGETC(c))

achar*	braslist[NBRA];
achar*	braelist[NBRA];
achar*	loc1=NULL, *loc2=NULL, *lpstart=NULL;
int	nodelim;

int	circf;
int	low;
int	size;

static int advance(achar* lp, achar* elp, achar* ep);

static void Place(achar *ep, achar c)
{
	int index = c >> 3;
	ubyte *ubp = (ubyte *) ep;

	if (index >= (EPSIZE*sizeof(achar)))
		ERROR(51);

	ubp[index] |= bittab[c & 0x7];
}

static int IsThere(achar *ep, achar c)
{
	int index = c >> 3;
	ubyte *ubp = (ubyte *) ep;

	if (index >= (EPSIZE*sizeof(achar)))
		ERROR(51);

	return (int) (ubp[index] & bittab[c & 0x7]);
}


static int regexp_avtoupper(achar c)
{
	if (c >= AV_AL && c <= AV_ZL)
		c += AV_A-AV_AL;
	else if (c >= AV_AU && c <= AV_ZU)
		c -= AV_AU-AV_A;
	else if (c == AV_DELTAU)
		c = AV_DELTA;
	return c;
}

static achar avtolower(achar c)
{
	if (c >= AV_A && c <= AV_Z)
		c -= AV_A-AV_AL;
	else if (c >= AV_AU && c <= AV_ZU)
		c -= AV_AU-AV_AL;
	else if (c == AV_DELTA)
		c = AV_DELTAU;
	return c;
}
static achar uscore(achar c)
{	
	//xutils ap prior to october 2000 matched a, AV_A, and AV_AU in
		//caseless mode.  This function helps us keep that functionality
	if (c >= AV_AL && c <= AV_ZL)
		c += AV_AU - AV_AL;
	else if (c >= AV_A && c <= AV_Z)
		c += AV_AU - AV_A;
	return c;
}

static int step(achar* p1, achar* ep1, achar* p2)
{
	int c;
	int retval;

	if(circf) 
		{
		loc1 = p1;
		return(advance(p1, ep1, p2));
		}
	// fast check for first character 
	if(*p2==CCHR) 
		{
		c = p2[1];
		do 
			{
			if(p1 <= ep1 && *p1 != c)
				continue;
			if(advance(p1, ep1, p2)) 
				{
				loc1 = p1;
				return(1);
				}
			} while(p1++ <= ep1);
		return(0);
		}
	else if(*p2==CLCCHR) 
		{
		c = p2[1];
		do 
			{
			if(p1 <= ep1 && regexp_avtoupper(*p1) != c)
				continue;
			if(advance(p1, ep1, p2)) 
				{
				loc1 = p1;
				return(1);
				}
			} while(p1++ <= ep1);
		return(0);
		}

		// regular algorithm 
	do 
		{
		if(retval=advance(p1, ep1, p2)) 
			{
			loc1 = p1+(retval==2);
			return(1);
			}
		} while(p1++ <= ep1);
	return(0);
}

void getrnge(achar* str)
{
	low = *str++;
	size = *str == 255 ? 20000 :*str - low;
}

int ecmp(achar* a, achar* b, int count)
{
	while(count--)
		if(*a++ != *b++)
			return(0);
	return(1);
}

static int regexp_in_word(achar c)
{
if (c>=AV_ZERO && c<=AV_NINE) return 1;
if (c>=AV_A && c <=AV_Z) return 1;
if (c>=AV_AL && c<=AV_ZL) return 1;
if (c>=AV_AU && c<=AV_ZU) return 1;
return 0;
}

static int advance(achar* lp, achar* elp, achar* ep)
{
	achar* curlp;
	achar c;
	achar* bbeg;
	int ct;

	while(1)
		switch(*ep++) 
			{

		case CCHR:
			if(lp > elp)
				return 0;

			if(*ep++ == *lp++)
				continue;
			return(0);
		case CLCCHR:
			if(lp > elp)
				return 0;

			if(*ep++ == regexp_avtoupper(*lp++))
				continue;
			return(0);
		case CDOT:
			if(lp++ <= elp)
				continue;
			return(0);

		case CDOL:
			if(lp == elp+1)
				continue;
			return(0);

		case CCEOF:
			loc2 = lp;
			return(1);
		case LTHAN:
#if 0
/* This was the code as of July 15, 1997 */			
			if (lp==lpstart) continue;
			if (regexp_in_word(*lp)) return 0;
		   // 	*--ep =0; 
			return (advance(++lp,elp,ep) ? 2 : 0) ;
			
			/*
			 * This code had problems with:
			 * finding \<word\> in 'word'
			 */
#endif
			if(lp > elp)	//added here for prob sept 2000
				return 0; 

				/*new code to fix bug 8084 */
				/* added july 22,1997, CVC */
			ep++; 	//corresponds to ep++ in compile().
			if (lp==lpstart) 
				continue;
			if (regexp_in_word(*(lp-1)) )
				return 0;
			else
				continue;
		case GTHAN:
			if (lp >=elp) continue;
			if (regexp_in_word(*lp)) return 0;
		//	*--ep=0; 
			return advance(lp,elp,ep);
		case CCL:
			if(lp > elp)
				return 0; 
			c = *lp++;
			if (IsThere(ep, c) ){
					ep += EPSIZE;
					continue;
					}
			return(0);
			
		case CBRA:
			if(lp > elp)
				return 0; 
			braslist[*ep++] = lp;
			continue;

		case CKET:
			if(lp > elp)
				return 0; 
			braelist[*ep++] = lp;
			continue;

		case CCHR|RNGE:
			if(lp > elp)
				return 0; 

			c = *ep++;
			getrnge(ep);
			for(;low--;lp++)
				if(*lp != c)
					return(0);
			curlp = lp;
			for(;size--;lp++) 
				if(*lp != c)
					break;
			ep += 2;
			goto asterisk;

		case CLCCHR|RNGE:
			if(lp > elp)
				return 0; 

			c = *ep++;
			getrnge(ep);
			for(;low--;lp++)
				if(regexp_avtoupper(*lp) != c)
					return(0);

			curlp = lp;
			for(;size--;lp++) 
				if(regexp_avtoupper(*lp) != c)
					break;

			ep += 2;
			goto asterisk;


		case CDOT|RNGE:
			if(lp > elp)
				return 0; 

			getrnge(ep);
			for(;low--;lp++)
				if(lp > elp)
					return(0);
			curlp = lp;
			for(;size--;lp++)
				if(lp > elp)
					break;
			ep += 2;
			goto asterisk;

		case CCL|RNGE:
			if(lp > elp)
				return 0; 

			getrnge(ep + EPSIZE);
			for(;low--;lp++) 
				{
				c = *lp;
				if(!IsThere(ep, c))
					return(0);
				}
			curlp = lp;
			for(;size--;lp++) 
				{
				c = *lp;
				if(!IsThere(ep, c))
					break;
				}
			ep += EPSIZE+2;
			goto asterisk;
		case CBACK:
			bbeg = braslist[*ep];
			ct = braelist[*ep++] - bbeg;

			if(lp > elp)
				return 0; 

			if(ecmp(bbeg, lp, ct)) 
				{
				lp += ct;
				continue;
				}
			return(0);

		case CBACK|ASTERISK:
			bbeg = braslist[*ep];
			ct = braelist[*ep++] - bbeg;
			curlp = lp;

			while((lp + ct <= elp) && ecmp(bbeg, lp, ct))
					lp += ct;

			while(lp >= curlp) 
				{
				if(advance(lp, elp, ep)) return(1);
				lp -= ct;
				}
			return(0);


		case CDOT|ASTERISK:
			curlp = lp;
			while(lp <= elp)
				lp++;
			goto asterisk;

		case CCHR|ASTERISK:
			curlp = lp;
			while(lp <= elp && *lp == *ep)
				lp++;
			ep++;
			goto asterisk;
	
		case CLCCHR|ASTERISK:
			curlp = lp;
			while(lp <= elp && regexp_avtoupper(*lp) == *ep)
				lp++;
			ep++;
			goto asterisk;

		case CCL|ASTERISK:
			curlp = lp;
			for(;lp <= elp; lp++)
				{
				c = *lp;
				if (!IsThere(ep, c))
					break;
				}
			ep += EPSIZE;
			goto asterisk;

		asterisk:
			do 
				{
				if(advance(lp, elp, ep))
					return(1);
				--lp;
				} while(lp >= curlp);
			return(0);

		}
}
void compile(achar* instring, achar* ep, achar* endbuf, UNS elen, int caseless) 
{
	INIT	// Dependent declarations and initializations 
	unsigned c;
	unsigned count;
	achar* lastep = instring;
	int cclcnt;
	achar bracket[NBRA];
	achar* bracketp;
	int closed = 0;
	achar neg;
	achar* tmpptr;
	int lc;
	int i, cflg;
	int nbra=0, ebra=0;	//Fix Problem 1933 
	count = 0;
	lastep = 0;

	if(elen == 0)		// null string: can't risk NGETC().
		{
		if(*ep == 0)
			ERROR(41);
		goto ret;
		}

	c = NGETC();
	if(c == AV_NEWLINE) 
		{
		NUNGETC(c);
		nodelim = 1;
		if(*ep == 0)
			ERROR(41);
		goto ret;
		}
	bracketp = bracket;
	circf = 0;
	if(c == AV_AND)
		circf++;
	else
		NUNGETC(c);
	while(1)
		{
		if(ep >= endbuf)
			ERROR(50);

		if(count >= elen)		// none left
			{
			*ep++ = CCEOF;
			goto ret;
			}

		c = NGETC();
		if(c != AV_STAR && ((c != AV_SLOPE) || (PEEKC() != AV_LBRACE)))
			lastep = ep;
		switch(c) 
			{
		case AV_POINT:
			*ep++ = CDOT;
			continue;

		case AV_NEWLINE:
			NUNGETC(c);
			*ep++ = CCEOF;
			nodelim = 1;
			goto ret;
		case AV_STAR:
			if(lastep==0 || *lastep==CBRA || *lastep==CKET)
				goto defchar;
			*lastep |= ASTERISK;
			continue;

		case AV_DOLLAR:
			if(count < elen && PEEKC() != AV_NEWLINE)
				goto defchar;
			*ep++ = CDOL;
			continue;
		
		case AV_RBRACKET:
			ERROR(49);
		case AV_LBRACKET:
			if(&ep[EPSIZE+1] >= endbuf)
				ERROR(50);

			*ep++ = CCL;
			lc = 0;
			for(i = 0; i < EPSIZE; i++)
				ep[i] = 0;

			neg = 0;
			if((c = NGETC()) == AV_AND) 
				{
				neg = 1;
				c = NGETC();  
				}
			if (c==AV_RBRACKET) // first el 
			{
				tmpptr = sp;
				for (i=count;i<=elen;i++)
				if (AV_RBRACKET == *tmpptr++)
				{	
					Place(ep, c);
					c=NGETC();
					break;
				}
			}
			while(AV_RBRACKET != c)
				{
				if(c == 0 || c == AV_NEWLINE)
					ERROR(49);
				if(c == AV_MINUS && lc != 0) 
					{
					if(AV_RBRACKET==(c = NGETC())) 
						{
						Place(ep, AV_MINUS);
						break;
						}
					while(lc < c) 
						{
						Place(ep, lc);
						lc++;
						}
					}
				lc = c;
				if (caseless)
					{
					Place(ep, avtolower(c));
					Place(ep, regexp_avtoupper(c));
					Place(ep, uscore(c));
					}
				else
					{
					Place(ep, c);   
					}
				c=NGETC();
				}
			if(neg) 
				{
				for(cclcnt = 0; cclcnt < EPSIZE; cclcnt++)
#if HAS_UNICODE
					ep[cclcnt] ^= 0xffffffff;
#else //HAS_UNICODE
					ep[cclcnt] ^= 0xff;
#endif   //HAS_UNICODE
				ep[0] &= 0376;
				}

			ep += EPSIZE;

			continue;

		case AV_SLOPE:
			switch(c = NGETC()) 
				{
			case AV_LT:
				*ep++ = LTHAN;
				ep++;
				continue;
			case AV_GT:
				*ep++ = GTHAN;
				continue;
			case AV_LPAREN:
				if(nbra >= NBRA)
					ERROR(43);
				*bracketp++ = nbra;
				*ep++ = CBRA;
				*ep++ = nbra++;
				continue;
				
			case AV_RPAREN:
				if(bracketp <= bracket || ++ebra != nbra)
					ERROR(42);
				*ep++ = CKET;
				*ep++ = *--bracketp;
				closed++;
				continue;

			case AV_LBRACE:
				if(lastep == (achar* ) (0))
					goto defchar;
				*lastep |= RNGE;
				cflg = 0;
			nlim:
				c = NGETC();
				i = 0;
				do 
					{
					if(AV_ZERO <= c && c <= AV_NINE)
						i = 10 * i + c - AV_ZERO;
					else
						ERROR(16);
					} while(((c = NGETC()) != AV_SLOPE)
						&& (c != AV_COMMA));
				if(i > 255)
					ERROR(11);
				*ep++ = i;
				if(c == AV_COMMA) 
					{
					if(cflg++)
						ERROR(44);
					if((c = NGETC()) == AV_SLOPE)
						*ep++ = 255;
					else 
						{
						NUNGETC(c);
						goto nlim;
						// get 2'nd number 
						}
					}
				if(NGETC() != AV_RBRACE)
					ERROR(45);
				if(!cflg)	// one number 
					*ep++ = i;
				else if((ep[-1]) < (ep[-2]))
					ERROR(46);
				continue;

			case AV_NEWLINE:
				ERROR(36);

			case AV_N:
				c = AV_NEWLINE;
				goto defchar;

			default:
				if(c >= AV_ONE && c <= AV_NINE) 
					{
					if((c -= AV_ONE) >= closed)
						ERROR(25);
					*ep++ = CBACK;
					*ep++ = c;
			continue;
					}
				}
	// Drop through to default to use \ to turn off special chars 
		defchar:
		default:
			lastep = ep;
			if (caseless)
				{
				*ep++ = CLCCHR;
				*ep++ = regexp_avtoupper(c);
				}
			else
				{
				*ep++ = CCHR;
				*ep++ = c;
				}

			}
			continue;
		ret:
		if (nbra != closed) ERROR(42);
		return;
		}
}

