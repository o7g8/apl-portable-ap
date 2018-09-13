#ifndef _T_TABLES_H
#define _T_TABLES_H

//	Copyright (c) 1982 Dyadic Systems Limited	

#if !HAS_UNICODE

//	Standard translate tables 			

static const UCH aplmap[128] =
// 0/8,    1/9,    2/A,    3/B,    4/C,    5/D,    6/E,    7/F	
{
AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, // 0 
AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, // 0 

AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, // 1 
AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, // 1 

AV_SP, AV_DIRESIS, AV_RPAREN, AV_LT, AV_LE, AV_EQ, AV_GT, AV_RBRACKET, // 2 
AV_OR, AV_AND, AV_NE, AV_DIVIDE, AV_COMMA, AV_PLUS, AV_POINT, AV_SLASH, // 2 

AV_ZERO, AV_ONE, AV_TWO, AV_THREE, AV_FOUR, AV_FIVE, AV_SIX, AV_SEVEN, // 3 
AV_EIGHT, AV_NINE, AV_LPAREN, AV_LBRACKET, AV_SEMI, AV_TIMES, AV_COLON, AV_SLOPE, // 3 

AV_HIGH, AV_ALPHA, AV_BASE, AV_CAP, AV_DSTILE, AV_EPSILON, AV_UNDER, AV_DEL, // 4 
AV_DELTA, AV_IOTA, AV_JOT, AV_QUOTE, AV_QUAD, AV_STILE, AV_TOP, AV_CIRCLE, // 4 

AV_STAR, AV_QUERY, AV_RHO, AV_UPSTILE, AV_TILDE, AV_DOWN, AV_CUP, AV_OMEGA, // 5 
AV_RSHOE, AV_UP, AV_LSHOE, AV_LEFT, AV_EURO, AV_RIGHT, AV_GE, AV_MINUS, // 5 

AV_DIAMOND, AV_A, AV_B, AV_C, AV_D, AV_E, AV_F, AV_G, // 6 
AV_H, AV_I, AV_J, AV_K, AV_L, AV_M, AV_N, AV_O, // 6 

AV_P, AV_Q, AV_R, AV_S, AV_T, AV_U, AV_V, AV_W, // 7 
AV_X, AV_Y, AV_Z, AV_MATCH, AV_LTACK, AV_NOTMATCH, AV_DOLLAR, AV_SQUISH		// 7 
};

static const UCH asciimap[] =
// 0/8, AV_1/9, AV_2/A, AV_3/B, AV_4/C, AV_5/D, AV_6/E, AV_7/F	
{
0     , AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, // 0 
AV_BS, AV_SQUISH, AV_NEWLINE, AV_SQUISH, AV_SQUISH, AV_LF, AV_SQUISH, AV_SQUISH, // 0 

/* above translates added 10-10-85 DSG:		ASCII	APL
obviously				0x08	BS	BS
even though ASCII NEWLINE is LF		0x0a	NEWLINE	NEWLINE
& just to be helpful			0x0d	CR	LF
*/

AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, // 1 
AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, AV_SQUISH, // 1 

AV_SP, AV_SHRIEK, AV_DQUOTE, AV_HASH, AV_DOLLAR, AV_PERCENT, AV_AMPERSAND, AV_ACUTE, // 2 
AV_LPAREN, AV_RPAREN, AV_STAR, AV_PLUS, AV_COMMA, AV_MINUS, AV_POINT, AV_SLASH, // 2 

AV_ZERO, AV_ONE, AV_TWO, AV_THREE, AV_FOUR, AV_FIVE, AV_SIX, AV_SEVEN, // 3 
AV_EIGHT, AV_NINE, AV_COLON, AV_SEMI, AV_LT, AV_EQ, AV_GT, AV_QUERY, // 3 

AV_ASCAT, AV_A, AV_B, AV_C, AV_D, AV_E, AV_F, AV_G, // 4 
AV_H, AV_I, AV_J, AV_K, AV_L, AV_M, AV_N, AV_O, // 4 

AV_P, AV_Q, AV_R, AV_S, AV_T, AV_U, AV_V, AV_W, // 5 
AV_X, AV_Y, AV_Z, AV_LBRACKET, AV_SLOPE, AV_RBRACKET, AV_CIRCUMFLEX, AV_UNDER, // 5 

AV_GRAVE, AV_AL, AV_BL, AV_CL, AV_DL, AV_EL, AV_FL, AV_GL, // 6 
AV_HL, AV_IL, AV_JL, AV_KL, AV_LL, AV_ML, AV_NL, AV_OL, // 6 

AV_PL, AV_QL, AV_RL, AV_SL, AV_TL, AV_UL, AV_VL, AV_WL, // 7 
AV_XL, AV_YL, AV_ZL, AV_LBRACE, AV_ASCBAR, AV_RBRACE, AV_TILDE, AV_SQUISH		// 7 
};

static const UCH unixtab[256] =
{/*
 0   1   2   3   4   5   6   7   8   9   a   b   c   d   e   f	*/
0  ,010,012,012,' ',127,127,127,127,127,127,127,'%',047,127,127,//0
'_','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o',//1
'p','q','r','s','t','u','v','w','x','y','z',127,127,'-','.',127,//2
'0','1','2','3','4','5','6','7','8','9',127,127,127,'$',127,127,//3
127,'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O',//4
'P','Q','R','S','T','U','V','W','X','Y','Z','.',127,127,127,127,//5
127,'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o',//6
'p','q','r','s','t','u','v','w','x','y','z','{',127,'}',127,127,//7
127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,//8
127,127,127,127,127,127,127,127,127,127,127,'[','/',127, 92,127,//9
'<',127,'=',127,'>',127,127,'^','-','+',127,127,'?',127,127,'~',//a
127,127,127,127,'*',127,127,127,127,'(',127,127,127,127,127,127,//b
'|',';',',',127,127,127,127,127,127,127,127,127,'!',127,127,127,//c
127,127,127,127,127,127,127,'"','#',127,'&',047,127,127,127,127,//d
127,127,127,127,127,127,127,'@',127,127,127,'^',127,'`','|',127,//e
':',127,127,127,127,127,127,127,')',']',127,127,127,127,127,127 //f
};

#endif // HAS_UNICODE

//	Copyright (c) 1982 Dyadic Systems Limited	


#endif // _T_TABLES_H
