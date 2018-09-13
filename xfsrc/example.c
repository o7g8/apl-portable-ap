#include	<machine.h>

#include	<apl.h>
#include	<trans.h>
#include	<gfns.h>
#include	<support.h>

gchar taskid[] = GSTR("example");
ARRAY *idn_d(ARRAY *), *d_idn(ARRAY *, ARRAY *);

int
#if	defined(DOS)
	main()
#elif	defined(WINDOWS3)
	ap_main(int argc, char *argv[])
#else
	main(int argc, char *argv[])
#endif
{
	ARRAY	*rslt, *larg, *rarg;
	int	fncode;

	startup(2);
	define(GSTR("IDN_TO_DATE"), RSLT|MONA, 1);
	define(GSTR("DATE_TO_IDN"), RSLT|OPTL, 2);

	larg = rarg = rslt = 0;
	sigsetjmp(errbuff, 1);

	while(1)
		{
		larg = freearray(larg);
		rarg = freearray(rarg);
		rslt = freearray(rslt);

		fncode = fromapl();

		switch(fncode)
			{
			case -1:		// AP about to be terminated 
				toapl(0);	// Signal go-ahead to apl    
				fromapl();	// and wait for kill	     
				exit(0); 	// ( For DOS version only. ) 
			case 1:
				rarg = getarray();
				rslt = idn_d(rarg);
				break;
			case 2:
				larg = getarray();
				rarg = getarray();
				rslt = d_idn(larg, rarg);
				break;
			}
		putarray(rslt);
		}
	return 0;
}

static int day_tab[2][13] =
{
	{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	{0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

static gchar month[][4] =
{
	GSTR("Nul"),
	GSTR("Jan"), GSTR("Feb"), GSTR("Mar"), GSTR("Apr"),
	GSTR("May"), GSTR("Jun"), GSTR("Jul"), GSTR("Aug"),
	GSTR("Sep"), GSTR("Oct"), GSTR("Nov"), GSTR("Dec")
};

ARRAY	*idn_d(ARRAY *rarg) // international day number to date 
{

	ARRAY		*rslt;
	gchar	*cptr, *format;
	int 		(*makedint)(void *, BOUND);
	int		year, day, leap, i;
	BOUND 		numchars;
	unsigned	idn;

	static gchar cbuf[12];

	numchars = 11;
	format = GSTR("%02d %s %4d");

	if(rarg->type == NESTED) error(EDOMAIN);
	if(rarg->rank > 1) error(RANK);
	if(rarg->rank && arraybound(rarg) != 1) error(LENGTH);

	makedint = initdint(rarg);
	idn = (*makedint)((int *)arraystart(rarg),0);

	year = idn/365;
	day = (idn - year*365) - ((year == 0) ? year : year-1)/4;
	year = 1900 + year - (day <= 0);
	leap = ((year%4 == 0) && (year%100 != 0)) || (year%400 == 0);
	day = day + (day <= 0) * (365 + leap); 

	for(i = 1; day > day_tab[leap][i]; i++) day -= day_tab[leap][i];

	rslt = mkarray(APLGCHAR, 1, &numchars);
	cptr = arraystart(rslt);
	gsprintf(cbuf,gsizeof(cbuf), format, day, month[i], year);
	for(i = 0; i < numchars; i++) *cptr++ = unixtoav(cbuf[i]);

	return rslt;
}

ARRAY	*d_idn(ARRAY *larg, ARRAY *rarg) // date to international day number 
{
	// note this is an example. The larg if given is ignored.
	ARRAY	*rslt;
	UCH	avdate[11], uxdate[11], *dp;
	UCH 	mnth[4];
	int	day, year, leap;
	int	i, mno;
	BOUND	zero;

	if(rarg->rank != 1) error(RANK);
	switch(rarg->eltype)
	{
	case APLNCHAR:
		dp = avdate;
		break;
	case APLWCHAR8:
		dp = uxdate;
		break;
	default:
		error(EDOMAIN);
	}
	if(rarg->shape[0] > 11) error(LENGTH);

	strncpy(dp, arraystart(rarg), rarg->shape[0]); // both one byte types
	dp[rarg->shape[0]] = '\0';
	if(rarg->eltype == APLNCHAR)
		for(i=0; i<11; i++)
			uxdate[i] = avtounix(avdate[i]);
	sscanf(uxdate, "%d %3s %d", &day, mnth, &year);

	if(year < 100) year = year + 2000;	// two digits years are this century
	for(mno = 1; mno <= 12; mno++)
	{
		for(i = 0; i < 4; i++)
		{
		// NB      gchar            UCH	       not necessarilly the same
			if(month[mno][i] != mnth[i])
				break;
		}
		if(i == 4)
			break;
	}
	if (mno > 12) error(EDOMAIN);

	leap = ((year%4 == 0) && (year%100 != 0)) || (year%400 == 0);
	if ((day < 1) || day > day_tab[leap][mno]) error(EDOMAIN);

	for(i = 1; i < mno; i++) day += day_tab[leap][i];

	year = year-1900;
	if (year < 0) error(EDOMAIN);

	day = day + (365 * year) + (year ? year - 1 : 0) / 4;

	zero = 0;
	rslt = mkarray(APLLONG, 0, &zero);
	rslt->shape[0] = day;

	return rslt;
}
