#ifndef _DECF_H
#define _DECF_H

/*	Copyright (c) 1982 Dyadic Systems Limited	*/

#if __STDC_WANT_DEC_FP__ // STDC platform
# define DCONST(a, b, c) c
#elif BYTE0123 // IBM LIB platform, big-endian
# define DCONST(a, b, c) { a, b }
#else // IBL LIB platform, little-endian
# define DCONST(a, b, c) { b, a }
#endif

// define constants (static)
static const apldecf128
	DECF_NEGONE	= DCONST(0xb040000000000000ULL, 0x0000000000000001ULL, -1.0DL),
	DECF_ZERO	= DCONST(0x3040000000000000ULL, 0x0000000000000000ULL,  0.0DL),
	DECF_HALF	= DCONST(0x303e000000000000ULL, 0x0000000000000005ULL,  0.5DL),
	DECF_ONE	= DCONST(0x3040000000000000ULL, 0x0000000000000001ULL,  1.0DL),
	DECF_TWO	= DCONST(0x3040000000000000ULL, 0x0000000000000002ULL,  2.0DL),
	DECF_TEN	= DCONST(0x3042000000000000ULL, 0x0000000000000001ULL,  10.0DL),
	DECF_1E2	= DCONST(0x3044000000000000ULL, 0x0000000000000001ULL,  1e2DL),
	DECF_1E20	= DCONST(0x3068000000000000ULL, 0x0000000000000001ULL,  1e20DL),

	DECF_SYSFUZZ	= DCONST(0x3024000000000000ULL, 0x0000000000000001ULL,  1e-14DL),
	DECF_FUZZ1LO	= DCONST(0x3024000000000000ULL, 0x00005af3107a3fffULL,  1.0DL - 1e-14DL),
	DECF_FUZZ1HI	= DCONST(0x3024000000000000ULL, 0x00005af3107a4001ULL,  1.0DL + 1e-14DL),

	DECF_UINTMAX	= DCONST(0x303e000000000009ULL, 0xfffffffffffffffbULL,  18446744073709551615.5DL),
	DECF_INTMAX	= DCONST(0x303e000000000004ULL, 0xfffffffffffffffbULL,  9223372036854775807.5DL),
	DECF_INTMIN	= DCONST(0xb03e000000000005ULL, 0x0000000000000005ULL, -9223372036854775808.5DL),
	DECF_UINT32MAX	= DCONST(0x303e000000000000ULL, 0x00000009fffffffbULL,  4294967295.5DL),
	DECF_INT32MAX	= DCONST(0x303e000000000000ULL, 0x00000004fffffffbULL,  2147483647.5DL),
	DECF_INT32MIN	= DCONST(0xb03e000000000000ULL, 0x0000000500000005ULL, -2147483648.5DL),

	DECF_2P32	= DCONST(0x3040000000000000ULL, 0x0000000100000000ULL,  4294967296.0DL),

	DECF_E		= DCONST(0x2ffe86058a4bf4deULL, 0x4e906accb26abb56ULL,  2.718281828459045235360287471352662DL),
	DECF_LOG10_E	= DCONST(0x2ffcd61fab8bb0faULL, 0x1c6b85b90bd96be3ULL,  0.4342944819032518276511289189166051DL),
	DECF_LN2	= DCONST(0x2ffd55bf79560cbfULL, 0x74832b8a22ff4c06ULL,  6.931471805599453094172321214581766e-1DL),
	DECF_LN10	= DCONST(0x2ffe7186b5b3ada4ULL, 0x144d079b9275f4ccULL,  2.302585092994045684017991454684364DL),

	DECF_NEGPI	= DCONST(0xaffe9ae4795796a7ULL, 0xbabe5564e6f39f8fULL, -3.141592653589793238462643383279503DL),
	DECF_NEG3PIBY4	= DCONST(0xaffe742b5b01b0fdULL, 0xcc0ec00bad36b7abULL, -2.356194490192344928846982537459627DL),
	DECF_NEGPIBY4	= DCONST(0xaffd833b2f5af8a3ULL, 0x52dbd57c41610ee5ULL, -0.7853981633974483096156608458198757DL),
	DECF_PIBY4	= DCONST(0x2ffd833b2f5af8a3ULL, 0x52dbd57c41610ee5ULL,  0.7853981633974483096156608458198757DL),
	DECF_PI2	= DCONST(0x2ffe4d723cabcb53ULL, 0xdd5f2ab27379cfc7ULL,  1.570796326794896619231321691639751DL),
	DECF_3PIBY4	= DCONST(0x2ffe742b5b01b0fdULL, 0xcc0ec00bad36b7abULL,  2.356194490192344928846982537459627DL),
	DECF_PI		= DCONST(0x2ffe9ae4795796a7ULL, 0xbabe5564e6f39f8fULL,  3.141592653589793238462643383279503DL),
	DECF_TWOPI	= DCONST(0x2fff35c8f2af2d4fULL, 0x757caac9cde73f1eULL,  6.283185307179586476925286766559006DL),

	DECF_DCTDEFAULT	= DCONST(0x3008000000000000ULL, 0x0000000000000001ULL,  1e-28DL),
	DECF_DCTMAX	= DCONST(0x30000000000004eeULL, 0x2d6d415b85acef81ULL,  2.3283064365386962890625e-10DL), // 2^-32 (exact)

	DECF_NEGMAX	= DCONST(0xdfffed09bead87c0ULL, 0x378d8e63ffffffffULL, -9.999999999999999999999999999999999e6144DL),
	DECF_MAX	= DCONST(0x5fffed09bead87c0ULL, 0x378d8e63ffffffffULL,  9.999999999999999999999999999999999e6144DL),
	DECF_MINNORM	= DCONST(0x0042000000000000ULL, 0x0000000000000001ULL,  1e-6143DL),
	DECF_NAN	= DCONST(0x7e00000000000000ULL, 0x0000000000000000ULL,  0.0DL / 0.0DL),
	DECF_EPSILON	= DCONST(0x2ffe000000000000ULL, 0x0000000000000001ULL,  1e-33DL),
	DECF_DBL_MAX	= DCONST(0x326658a213cc7a4fULL, 0xfac34c32b3920000ULL,  1.7976931348623157e308DL),
	DECF_DBL_MIN	= DCONST(0xb26658a213cc7a4fULL, 0xfac34c32b3920000ULL, -1.7976931348623157e308DL);

#if __STDC_WANT_DEC_FP__

////////////////////////////////////////////////////////////////////////////////
// STDC PLATFORM
////////////////////////////////////////////////////////////////////////////////

#include <maths.h>
#include <float.h>
#include <fpxcp.h>

// common functions to manipulate apldecf128 :
// those that cover infix notation will be defined (static) in this header file
// on standard compilers, to avoid cross-module function call cost

static INLINE uns decf_iszero(apldecf128 x)	// is zero ?
{ return x == 0; }
static INLINE uns decf_isnegative(apldecf128 x)// is negative ?
{ return x < 0; }
static INLINE uns decf_ispositive(apldecf128 x)// is positive ?
{ return x > 0; }
static INLINE uns decf_signbit(apldecf128 x)	// 1 = non-positive ; 0 = non-negative
{ return signbit(x); }
static INLINE uns decf_isnan(apldecf128 x)	// is nan ?
{ return isnan(x); }
static INLINE uns decf_isinf(apldecf128 x)	// is infinity ?
{ return isinf(x); }
static INLINE uns decf_isfinite(apldecf128 x)	// is finite ?
#if	RIOS
{ return __d128_isfinite(x); }
#else //RIOS
{ return isfinite(x); }
#endif//RIOS

static INLINE apldecf128 decf_from_int32(int32_t x)
{ return (apldecf128) x; }
static INLINE int32_t decf_to_int32(apldecf128 x)
{ return (int64_t) x; }		// pwr8 32bit can't do int32_t properly [15547]

static INLINE apldecf128 decf_from_uint32(uint32_t x)
{ return (apldecf128) x; }
static INLINE uint32_t decf_to_uint32(apldecf128 x)
{ return (uint64_t) x; }	// this as well although don't have evidence of
				// failure

static INLINE apldecf128 decf_from_int64(int64_t x)
{ return (apldecf128) x; }
static INLINE int64_t decf_to_int64(apldecf128 x)
{ return (int64_t) x; }

static INLINE apldecf128 decf_from_uint64(uint64_t x)
{ return (apldecf128) x; }
static INLINE uint64_t decf_to_uint64(apldecf128 x)
{ return (uint64_t) x; }

static INLINE double decf_to_double(apldecf128 x)
{ return (double) x; }

static INLINE apldecf128 decf_negate(apldecf128 x)
{ return -x; }
static INLINE apldecf128 decf_add(apldecf128 a, apldecf128 b)
{ return a+b; }
static INLINE apldecf128 decf_sub(apldecf128 a, apldecf128 b)
{ return a-b; }
static INLINE apldecf128 decf_mul(apldecf128 a, apldecf128 b)
{ return a*b; }
static INLINE apldecf128 decf_div(apldecf128 a, apldecf128 b)
{ return a/b; }

static INLINE uns decf_eq(apldecf128 a, apldecf128 b)
{ return a==b; }
static INLINE uns decf_eq_tolerant(apldecf128 a, apldecf128 b)
{ return 2-(a==b); }
static INLINE uns decf_neq(apldecf128 a, apldecf128 b)
{ return a!=b; }
static INLINE uns decf_gt(apldecf128 a, apldecf128 b)
{ return a>b; }
static INLINE uns decf_gte(apldecf128 a, apldecf128 b)
{ return a>=b; }
static INLINE uns decf_lt(apldecf128 a, apldecf128 b)
{ return a<b; }
static INLINE uns decf_lte(apldecf128 a, apldecf128 b)
{ return a<=b; }
static INLINE uns decf_lte_tolerant(apldecf128 a, apldecf128 b)
{ return 2-(a<=b); }

#else	// __STDC_WANT_DEC_FP__ 

////////////////////////////////////////////////////////////////////////////////
// IBM LIB PLATFORM
////////////////////////////////////////////////////////////////////////////////

#include <decf_lib.h> // for DEC_ROUND_* constants

// common functions to manipulate apldecf128 :

uns decf_iszero(apldecf128);		// is zero ?
uns decf_isnegative(apldecf128);	// is negative ?
uns decf_ispositive(apldecf128);	// is positive ?
uns decf_signbit(apldecf128);		// 1 = non-positive; 0 = non-negative
uns decf_isnan(apldecf128);		// is nan ?
uns decf_isinf(apldecf128);		// is infinity ?
uns decf_isfinite(apldecf128);		// is finite ?

apldecf128 decf_from_int32(int32_t);
int32_t decf_to_int32(apldecf128);
apldecf128 decf_from_uint32(uint32_t);
uint32_t decf_to_uint32(apldecf128);

apldecf128 decf_from_int64(int64_t);
int64_t decf_to_int64(apldecf128);
apldecf128 decf_from_uint64(uint64_t);
uint64_t decf_to_uint64(apldecf128);

double decf_to_double(apldecf128);

apldecf128 decf_negate(apldecf128);
apldecf128 decf_add(apldecf128, apldecf128);
apldecf128 decf_sub(apldecf128, apldecf128);
apldecf128 decf_mul(apldecf128, apldecf128);
apldecf128 decf_div(apldecf128, apldecf128);

uns decf_eq(apldecf128, apldecf128);
uns decf_eq_tolerant(apldecf128, apldecf128);
uns decf_neq(apldecf128, apldecf128);
uns decf_gt(apldecf128, apldecf128);
uns decf_gte(apldecf128, apldecf128);
uns decf_lt(apldecf128, apldecf128);
uns decf_lte(apldecf128, apldecf128);
uns decf_lte_tolerant(apldecf128, apldecf128);

// C standard implementation

#undef	DEC128_MAX
#define	DEC128_MAX	DECF_MAX

#undef	DEC128_MANT_DIG
#define	DEC128_MANT_DIG	 34

#define FE_DEC_TONEAREST		BID_ROUNDING_TO_NEAREST
#define FE_DEC_DOWNWARD			BID_ROUNDING_DOWN
#define FE_DEC_UPWARD			BID_ROUNDING_UP
#define FE_DEC_TOWARDZERO		BID_ROUNDING_TO_ZERO
#define FE_DEC_TONEARESTFROMZERO	BID_ROUNDING_TIES_AWAY

#define FE_DEC_DEFAULT FE_DEC_TONEAREST

int fe_dec_setround(int round);
int fe_dec_getround(void);

apldecf128 copysignd128(apldecf128, apldecf128);
apldecf128 fabsd128(apldecf128);
apldecf128 ceild128(apldecf128);
apldecf128 floord128(apldecf128);
apldecf128 rintd128(apldecf128);
apldecf128 truncd128(apldecf128);
apldecf128 roundd128(apldecf128);
apldecf128 fmodd128(apldecf128, apldecf128);
apldecf128 fmind128(apldecf128, apldecf128);
apldecf128 fmaxd128(apldecf128, apldecf128);

apldecf128 quantized128(apldecf128 x, apldecf128 y);
int ilogbd128(apldecf128);
apldecf128 scalbnd128(apldecf128, int);

apldecf128 sqrtd128(apldecf128);
apldecf128 expd128(apldecf128);
apldecf128 logd128(apldecf128);
static INLINE apldecf128 log10d128(apldecf128 a) {return decf_div(logd128(a), DECF_LN10);}
apldecf128 powd128(apldecf128, apldecf128);

apldecf128 sind128(apldecf128);
apldecf128 cosd128(apldecf128);
apldecf128 tand128(apldecf128);
apldecf128 asind128(apldecf128);
apldecf128 acosd128(apldecf128);
apldecf128 atand128(apldecf128);
apldecf128 atan2d128(apldecf128 y, apldecf128 x);

apldecf128 sinhd128(apldecf128);
apldecf128 coshd128(apldecf128);
apldecf128 tanhd128(apldecf128);
apldecf128 asinhd128(apldecf128);
apldecf128 acoshd128(apldecf128);
apldecf128 atanhd128(apldecf128);

apldecf128 hypotd128(apldecf128,apldecf128);

apldecf128 strtod128(const char* nptr, char** endptr);
apldecf128 wcstod128(const wchar_t* nptr, wchar_t** endptr);

#endif	// __STDC_WANT_DEC_FP__

// platform-independent functions

uns decf_accrued_exception(void);

// The buffer passed in must have room for DEC128_MANT_DIG + 1 characters
// (the + 1 is for the terminating nul character).
void decf_efcvt(apldecf128 x, int edigits, int fdigits, int *dec, int *sign,  char *buf);
static INLINE void decf_ecvt(apldecf128 x, int pp, int *decpt, int *sign, char *buf)
{
	// The smallest non-zero DECF needs 6000-odd digits after the decimal
	// point. Set edigits to something comfortably larger than this.
	decf_efcvt(x, pp, 10000, decpt, sign, buf);
}

bool decf_is_int32(apldecf128 got,aplint32 *lng); // fits exactly into int32
apldecf128 decf_from_double(double x);

apldecf128 decf_canonise(apldecf128 x);

static INLINE apldecf128 decf_log(apldecf128 a, apldecf128 b)
{ return decf_div(logd128(b), logd128(a)); }

static INLINE BOUND decf_to_bound(apldecf128 x)
{
#if xxBIT == 32
	return decf_to_uint32(x);
#elif xxBIT == 64
	return decf_to_uint64(x);
#else
# error
#endif
}

static INLINE apldecf128 decf_from_bound(BOUND x)
{
#if xxBIT == 32
	return decf_from_uint32(x);
#elif xxBIT == 64
	return decf_from_uint64(x);
#else
# error
#endif
}

#if xxBIT == 32
# define DECF_BOUNDMAX DECF_UINT32MAX
#elif xxBIT == 64
# define DECF_BOUNDMAX DECF_UINTMAX
#endif

#endif // _DECF_H
