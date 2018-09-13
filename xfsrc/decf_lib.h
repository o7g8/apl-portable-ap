#ifndef _DECF_LIB_H
#define _DECF_LIB_H

////////////////////////////////////////////////////////////////////////////////
// Intel library declarations
////////////////////////////////////////////////////////////////////////////////

// conversion functions from libdecf
extern BID_UINT128 LIBCALL bid128_from_int32(int32_t);
extern BID_UINT128 LIBCALL bid128_from_uint32(int32_t);
extern BID_UINT128 LIBCALL bid128_from_int64(int64_t);
extern BID_UINT128 LIBCALL bid128_from_uint64(int64_t);
extern int32_t LIBCALL bid128_to_int32_xint(BID_UINT128);
extern int32_t LIBCALL bid128_to_uint32_xint(BID_UINT128);
extern int64_t LIBCALL bid128_to_int64_xint(BID_UINT128);
extern int64_t LIBCALL bid128_to_uint64_xint(BID_UINT128);
extern BID_UINT128 LIBCALL __binary64_to_bid128(double);
extern double LIBCALL bid128_to_binary64(BID_UINT128);
extern BID_UINT128 LIBCALL bid_to_dpd128(BID_UINT128);
extern BID_UINT128 LIBCALL bid_dpd_to_bid128(BID_UINT128);
extern BID_UINT128 LIBCALL bid128_from_string(char*);
extern void LIBCALL bid128_to_string(char*, BID_UINT128);
extern BID_UINT128 LIBCALL bid128_strtod(char*, char**);
extern char* LIBCALL bid128_ecvt(BID_UINT128 x, int pp, int* dec, int* sign);

// arithmetic functions froms libdecf
extern BID_UINT128 LIBCALL bid128_add(BID_UINT128, BID_UINT128);
extern BID_UINT128 LIBCALL bid128_sub(BID_UINT128, BID_UINT128);
extern BID_UINT128 LIBCALL bid128_mul(BID_UINT128, BID_UINT128);
extern BID_UINT128 LIBCALL bid128_div(BID_UINT128, BID_UINT128);
extern BID_UINT128 LIBCALL bid128_rem(BID_UINT128, BID_UINT128);
extern BID_UINT128 LIBCALL bid128_negate(BID_UINT128);
extern BID_UINT128 LIBCALL bid128_abs(BID_UINT128);
extern BID_UINT128 LIBCALL bid128_copySign(BID_UINT128, BID_UINT128);
extern BID_UINT128 LIBCALL bid128_sqrt(BID_UINT128);
extern int LIBCALL bid128_quiet_equal(BID_UINT128, BID_UINT128);
extern int LIBCALL bid128_quiet_equal_tolerant(BID_UINT128, BID_UINT128);
extern int LIBCALL bid128_quiet_not_equal(BID_UINT128, BID_UINT128);
extern int LIBCALL bid128_quiet_greater(BID_UINT128, BID_UINT128);
extern int LIBCALL bid128_quiet_greater_equal(BID_UINT128, BID_UINT128);
extern int LIBCALL bid128_quiet_less(BID_UINT128, BID_UINT128);
extern int LIBCALL bid128_quiet_less_equal(BID_UINT128, BID_UINT128);
extern int LIBCALL bid128_quiet_less_equal_tolerant(BID_UINT128, BID_UINT128);
extern BID_UINT128 LIBCALL bid128_round_integral_negative(BID_UINT128);
extern BID_UINT128 LIBCALL bid128_round_integral_positive(BID_UINT128);
extern BID_UINT128 LIBCALL bid128_round_integral_exact(BID_UINT128);
extern BID_UINT128 LIBCALL bid128_round_integral_zero(BID_UINT128);
extern BID_UINT128 LIBCALL bid128_round_integral_nearest_away(BID_UINT128);
extern BID_UINT128 LIBCALL bid128_minnum(BID_UINT128, BID_UINT128);
extern BID_UINT128 LIBCALL bid128_maxnum(BID_UINT128, BID_UINT128);
extern BID_UINT128 LIBCALL bid128_quantize(BID_UINT128, BID_UINT128);

// misc stuff from libdecf
extern int LIBCALL bid128_ilogb(BID_UINT128);		// (int) log10(x)
extern BID_UINT128 LIBCALL bid128_scalbn(BID_UINT128, int);	// x * 10^n
extern int LIBCALL bid128_isSigned(BID_UINT128);	// is negative ?
extern int LIBCALL bid128_isFinite(BID_UINT128);	// not nan or infinity
extern int LIBCALL bid128_isZero(BID_UINT128);	// is zero
extern int LIBCALL bid128_isInf(BID_UINT128);		// is infinity
extern int LIBCALL bid128_isNaN(BID_UINT128);		// is nan

// rounding modes
#define BID_ROUNDING_TO_NEAREST     0x00000
#define BID_ROUNDING_DOWN           0x00001
#define BID_ROUNDING_UP             0x00002
#define BID_ROUNDING_TO_ZERO        0x00003
#define BID_ROUNDING_TIES_AWAY      0x00004

typedef unsigned int _IDEC_round;
extern __thread _IDEC_round _IDEC_glbround;	// init to ROUNDING_TO_NEAREST

// exception flags
#define DEC_FE_INVALID      0x01
#define DEC_FE_UNNORMAL     0x02
#define DEC_FE_DIVBYZERO    0x04
#define DEC_FE_OVERFLOW     0x08
#define DEC_FE_UNDERFLOW    0x10
#define DEC_FE_INEXACT      0x20

// status
#define BID_FLAG_MASK           0x0000003f
#define DEC_FE_ALL_EXCEPT       0x0000003f
#define BID_IEEE_FLAGS          0x0000003d
#define BID_EXACT_STATUS        0x00000000

typedef unsigned int _IDEC_flags;
extern __thread _IDEC_flags _IDEC_glbflags;	// init to EXACT_STATUS

#endif // _DECF_LIB_H
