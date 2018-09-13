// This header file includes itself recursively, so it does not have a
// "once-only" (#ifndef _KEYSTROKES_H) envelope

#define _			0

#define SHFT			1
#define CTL			2
#define ALT			4

#define SKF_CONF		1
#define SKF_HIDE		2
#define SKF_CHAR		8
#define SKF_PREVIEW		16		// keystroke is available in "file preview"

#if  KEYSTROKE_PASS==1

	typedef enum
		{
		#undef  ADD_KEYSTROKE
		#define ADD_KEYSTROKE(code,name,val,comment,vk,shift,flags,uni) code=val,
		#undef  KEYSTROKE_PASS
		#define KEYSTROKE_PASS 99
		#include <keystrokes.h>

		_END_
		} INPUT_SPECIAL_CODES;

	#undef  KEYSTROKE_PASS
	#define KEYSTROKE_PASS  2

	#include <keystrokes.h>

	#undef  ADD_KEYSTROKE
	#define ADD_KEYSTROKE(code,name,val,comment,vk,shift,flags,uni)

#elif KEYSTROKE_PASS==2

	typedef struct
		{
		gchar *name;
		int value;
		gchar *comment;
		uns def_vk;		// default key
		uns def_ss;		// default shift state
		uns flags;
		uns cnf_vk;		// configurable key
		uns cnf_ss;		// configurable shift state
		} SPECIALKEY;

	#if defined (KEYSTROKE_DEFINE)

	#define DEFINING_SKEYS

		SPECIALKEY Keystrokes[] = 
			{

			#undef  ADD_KEYSTROKE
			#define ADD_KEYSTROKE(code,name,val,comment,vk,shift,flags,uni) {GSTR(name),val,GSTR(comment),vk,shift,flags,0,0},
			#undef  KEYSTROKE_PASS
			#define KEYSTROKE_PASS 99
			#include <keystrokes.h>

			{0,0,0}
			};

	#undef DEFINING_SKEYS

	#else

	extern SPECIALKEY Keystrokes[];

	#endif

	#undef  KEYSTROKE_PASS
	#define KEYSTROKE_PASS  3

	#include <keystrokes.h>

	#undef  ADD_KEYSTROKE
	#define ADD_KEYSTROKE(code,name,val,comment,vk,shift,flags,uni)

#elif KEYSTROKE_PASS==3

	typedef struct 
		{
		UCH	string[3];
		int		code;
		} s2n_map;

	#if defined (ILINECODES_DEFINE)

//***************************************************************************
/*	HEADER PARSING:	LINE CODES.
 *	THIS CODE IS USED BY I_INIT AND QUADSR.
 */

/*	The ordinary line codes recognised in aplkeys files are listed below.
 *
 *	The table will be sorted so that the strings are in ascending lexical order,
 *	because lc_str2num uses a binary search on that field.
 *	You MUST NOT rely on the symbolic constants being defined in order -
 *	because they're not.
 *
 *	Codes not listed are:
 *		-	- single-shift mode specification;
 *		F	- Function key specification;
 *		M[0-9]	- Mode switch specification;
 *		0-9	- quadAV element specification.
 *
 *	Currently no <line code> begins with any of these characters, and they
 *	are handled specially in i_str2num() and i_num2str(). These functions
 *	will need to be fixed if that condition ever changes.
 */

		s2n_map i_line_codes[] =
		{
		#undef  KEYSTROKE_PASS
		#define KEYSTROKE_PASS  99

		#undef  ADD_KEYSTROKE
		#define ADD_KEYSTROKE(code,name,val,comment,vk,shift,flags,uni) {name,code},

		#define ILINECODES_PASS
		#include <keystrokes.h>
		#undef ILINECODES_PASS
		};

		#define	NUM_LINE_CODES	(sizeof(i_line_codes) / sizeof(s2n_map))

	#else
		extern s2n_map i_line_codes[];
	#endif

	#undef KEYSTROKE_PASS
	#define KEYSTROKE_PASS 4

	#include <keystrokes.h>

	#undef  ADD_KEYSTROKE
	#define ADD_KEYSTROKE(code,name,val,comment,vk,shift,flags,uni)

#elif KEYSTROKE_PASS==4

typedef  struct
	{
	int key;
	unsigned short uni;
	} UnicodeSpecial;
extern UnicodeSpecial UnicodeSpecials[];

#if defined(DEFINE_UNICODE_MAP)


UnicodeSpecial UnicodeSpecials[]=
	{
// UnicodeSpecials uses the "Corporate area" of the UNICODE set. We start at 0xF800;
// and populate WITH NO GAPS

	#undef  KEYSTROKE_PASS
	#define KEYSTROKE_PASS  99

	#undef  ADD_KEYSTROKE
	#define ADD_KEYSTROKE(code,name,val,comment,vk,shift,flags,uni) {code,uni},

	#define UNICODE_PASS
	#include <keystrokes.h>
	#undef UNICODE_PASS

	};

#define UNICODE_NELTS		(sizeof(UnicodeSpecials)/sizeof(UnicodeSpecials[0]))

	int UnicodeSpecialCount(void) {return UNICODE_NELTS;}

#define UNICODE_SPECIAL_END	(UNICODE_SPECIAL_START + UNICODE_NELTS)
extern int MAP_UNICODE_SPECIAL(int uni);

#else //defined(DEFINE_UNICODE_MAP)


#endif//defined(DEFINE_UNICODE_MAP)

#elif !defined(KEYSTROKE_PASS)
	
	#define VK2KEY_CONF		1
	#define VK2KEY_DEF		2
	#define VK2KEY_EITHER		(VK2KEY_CONF|VK2KEY_DEF)

	extern int vk2key(        int  vk, int  shift 	         );	
	extern int key2vk(int k , int *vk, int *shift , uns which);

	#define KEYSTROKE_PASS 1
	#include <keystrokes.h>
#else


	// VK_* codes listed here are the default keystrokes used if no translate table is present 
	// eg UNICODE version, or CLASSIC version with missing table.
	// Some of these can be configured (if SKF_CONF is set).

	// Anything with SKF_CONF is "configurable" and is displayed in the Configure dialog box
	// AND THE COMMENT IN THE TABLE IS INCLUDED IN THE DIALOG BOX, SO USE SENSIBLE TERMS

#if !defined(UNICODE_PASS)

#define SPEC(a)	a+UNICODE_SPECIAL_START
#define SPECFKEY(a)	a+UNICODE_FKEY_START

	ADD_KEYSTROKE(NOKEY 	,"", 32767 ,"",	_, _, _,0)
#if	!defined(WI)		// not if we already have objects.h
	ADD_KEYSTROKE(WI	,"WI", -1		,"Weak Interrupt",	_, _, _,0)
#endif//!defined(WI,0)
	ADD_KEYSTROKE(SI	,"SI", -2		,"Strong Interrupt",	_, _, _,0)
#endif	// !defined(UNICODE_PASS)

	ADD_KEYSTROKE(QT	,"QT", -3		,"Quit (and lose changes)",	_, _, SKF_CONF,SPEC(0))
	ADD_KEYSTROKE(ER	,"ER", -4		,"Enter", VK_RETURN, _, _,SPEC(1))
	ADD_KEYSTROKE(TB	,"TB", -5		,"Tab between windows",	VK_TAB, CTL, SKF_CONF,SPEC(2))
	ADD_KEYSTROKE(BT	,"BT", -6		,"Back Tab between windows",	VK_TAB, CTL|SHFT, SKF_CONF,SPEC(3))
	ADD_KEYSTROKE(EP	,"EP", -7		,"Exit (and save changes)", _, _, SKF_CONF,SPEC(4))

	ADD_KEYSTROKE(UC	, "UC",-8		,"Up Character", VK_UP, _, SKF_PREVIEW,SPEC(5))
	ADD_KEYSTROKE(DC	, "DC",-9		,"Down Character", VK_DOWN, _, SKF_PREVIEW,SPEC(6))
	ADD_KEYSTROKE(RC	, "RC",-10		,"Right Character", VK_RIGHT, _, SKF_PREVIEW,SPEC(7))
	ADD_KEYSTROKE(LC	, "LC",-11		,"Left Character", VK_LEFT, _, SKF_PREVIEW,SPEC(8))

	ADD_KEYSTROKE(US	, "US",-12		,"Up Screen", VK_PRIOR, _, SKF_PREVIEW,SPEC(9))
#undef	DS			// name clash with linux - ucontext.h
	ADD_KEYSTROKE(DS	, "DS",-13		,"Down Screen", VK_NEXT, _, SKF_PREVIEW,SPEC(0xa))
	ADD_KEYSTROKE(RS	, "RS",-14		,"Right Screen",	_, _, SKF_CONF,SPEC(0xb))
	ADD_KEYSTROKE(LS	, "LS",-15		,"Left Screen",	_, _, SKF_CONF,SPEC(0xc))

	ADD_KEYSTROKE(UL_	, "UL", -16		,"Up Limit", VK_HOME, CTL, SKF_PREVIEW,SPEC(0xd))
	ADD_KEYSTROKE(DL_	, "DL", -17		,"Down Limit", VK_END, CTL, SKF_PREVIEW,SPEC(0xe))
	ADD_KEYSTROKE(RL_	, "RL", -18		,"Right Limit", VK_END, _, SKF_PREVIEW,SPEC(0xf))
	ADD_KEYSTROKE(LL_	, "LL", -19		,"Left Limit", VK_HOME, _, SKF_PREVIEW,SPEC(0x10))

	ADD_KEYSTROKE(HO	, "HO",-20		,"Scroll to Current Line",	_, _, SKF_CONF,SPEC(0x11))

// Ctrl XCV versions of CT CP and PT are hardcoded in skeys.c (for windows)
	ADD_KEYSTROKE(CT	, "CT",-21		,"Cut",	VK_DELETE, SHFT, SKF_CONF,SPEC(0x12))
	ADD_KEYSTROKE(CP	, "CP",-32		,"Copy", VK_INSERT, CTL, SKF_CONF|SKF_PREVIEW,SPEC(0x1d))
	ADD_KEYSTROKE(PT	, "PT",-22		,"Paste", VK_INSERT, SHFT, SKF_CONF,SPEC(0x13))

#undef	IN			// name clash with windows - winsock.h
	ADD_KEYSTROKE(IN	, "IN",-23		,"Insert Mode Toggle", VK_INSERT, _, _,SPEC(0x14))
	ADD_KEYSTROKE(II	, "II",-24		,"Insert Item",	_, _, _,SPEC(0x15))
	ADD_KEYSTROKE(DI	, "DI",-25		,"Delete Item", VK_DELETE, _, _,SPEC(0x16))
	ADD_KEYSTROKE(DP	, "DP",-26		,"Destructive Space", _, _, _,SPEC(0x17))
	ADD_KEYSTROKE(DB	, "DB",-27		,"Destructive Backspace", VK_BACK, _, _,SPEC(0x18))
#if defined (DEFINING_SKEYS)
	ADD_KEYSTROKE(DB	, "DB",-27		,"Destructive Backspace", VK_BACK, SHFT, _,SPEC(0x18))
#endif
	ADD_KEYSTROKE(RD	, "RD",-28		,"Reformat", _, _, SKF_CONF,SPEC(0x19))

	ADD_KEYSTROKE(TG	, "TG", -29		,"Tag", _, _, _,SPEC(0x1a))
	ADD_KEYSTROKE(DK	, "DK", -30		,"Delete Block", VK_DELETE, CTL, SKF_CONF,SPEC(0x1b))
	ADD_KEYSTROKE(OP	, "OP", -31		,"Open", _, _, _,SPEC(0x1c))


	ADD_KEYSTROKE(MV	, "MV", -33		,"Move Block", _, _, _,SPEC(0x1e))

	ADD_KEYSTROKE(FD	, "FD",-34		,"Forward or Redo", _, _, SKF_CONF,SPEC(0x1f))
	ADD_KEYSTROKE(BK	, "BK",-35		,"Backward or Undo", _, _,SKF_CONF,SPEC(0x20))

	ADD_KEYSTROKE(ZM	, "ZM",-36		,"ZooM", _, _, _,SPEC(0x21))

	ADD_KEYSTROKE(SC	, "SC",-37		,"Search", _, _, SKF_CONF,SPEC(0x22))
	ADD_KEYSTROKE(RP	, "RP",-38		,"Replace", _, _, SKF_CONF,SPEC(0x23))
	ADD_KEYSTROKE(NX	, "NX",-39		,"Next", _, _, SKF_CONF,SPEC(0x24))
	ADD_KEYSTROKE(PV	, "PV",-40		,"Previous", _, _, SKF_CONF,SPEC(0x25))

	ADD_KEYSTROKE(RT	, "RT",-41		,"Repeat", _, _, _,SPEC(0x26))
	ADD_KEYSTROKE(RA	, "RA",-42		,"Repeat all", _, _, _,SPEC(0x27))

	ADD_KEYSTROKE(ED	, "ED",-43		,"Edit", _, _, SKF_CONF,SPEC(0x28))
	ADD_KEYSTROKE(TC	, "TC",-44		,"Trace", _, _, SKF_CONF,SPEC(0x29))

/* Following not used from 5.3 onwards but must keep them for existing
 * quadsm users who might just be using them as exit keys 
 */

	ADD_KEYSTROKE(NB	, "NB",-45		,"Non Destructive Backspace", _, _, _,SPEC(0x2a))
	ADD_KEYSTROKE(NS	, "NS",-46		,"Non Destructive Space", _, _, _,SPEC(0x2b))
	ADD_KEYSTROKE(ST	, "ST",-47		,"Start of Line", _, _, _,SPEC(0x2c))
	ADD_KEYSTROKE(EN	, "EN",-48		,"End of line", _, _, _,SPEC(0x2d))
	ADD_KEYSTROKE(IF	, "IF",-49		,"Insert off", _, _, _,SPEC(0x2e))

// Late additions 

	ADD_KEYSTROKE(HK	, "HK",-50		,"Hot Key", _, _, _,SPEC(0x2f))
	ADD_KEYSTROKE(FX	, "FX",-51		,"Fix the current function", _, _, SKF_CONF,SPEC(0x30))
	ADD_KEYSTROKE(LN	, "LN",-52		,"Toggle Line numbers", _, _, SKF_CONF,SPEC(0x31))

	ADD_KEYSTROKE(MC	, "MC",-53		,"Mode Change", _, _, _,SPEC(0x32))
	ADD_KEYSTROKE(MR	, "MR",-54		,"Move / Resize", _, _, _,SPEC(0x33))

// Very Late additions 

	ADD_KEYSTROKE(JP	, "JP",-55		,"JumP between current window and session window", _, _, SKF_CONF,SPEC(0x34))

// XAPL Mouse QuadSM exit keys 
	ADD_KEYSTROKE(D1	, "D1",-56		,"", _, _, _,SPEC(0x35))
	ADD_KEYSTROKE(D2	, "D2",-57		,"", _, _, _,SPEC(0x36))
	ADD_KEYSTROKE(D3	, "D3",-58		,"", _, _, _,SPEC(0x37))
	ADD_KEYSTROKE(D4	, "D4",-59		,"", _, _, _,SPEC(0x38))
	ADD_KEYSTROKE(D5	, "D5",-60		,"", _, _, _,SPEC(0x39))
	ADD_KEYSTROKE(U1	, "U1",-61		,"", _, _, _,SPEC(0x3a))
	ADD_KEYSTROKE(U2	, "U2",-62		,"", _, _, _,SPEC(0x3b))
	ADD_KEYSTROKE(U3	, "U3",-63		,"", _, _, _,SPEC(0x3c))
	ADD_KEYSTROKE(U4	, "U4", -64		,"", _, _, _,SPEC(0x3d))
	ADD_KEYSTROKE(U5	, "U5",-65		,"", _, _, _,SPEC(0x3e))

// GUI text selection keys 

	ADD_KEYSTROKE(LCS	, "Lc",-66		,"Left cursor with selection", VK_LEFT, SHFT, SKF_PREVIEW,SPEC(0x3f))
	ADD_KEYSTROKE(RCS	, "Rc",-67		,"Right cursor with selection", VK_RIGHT, SHFT, SKF_PREVIEW,SPEC(0x40))
	ADD_KEYSTROKE(LW	, "LW",-68		,"Left word", VK_LEFT, CTL, SKF_PREVIEW,SPEC(0x41))
	ADD_KEYSTROKE(RW	, "RW",-69		,"Right word", VK_RIGHT, CTL, SKF_PREVIEW,SPEC(0x42))
	ADD_KEYSTROKE(LWS	, "Lw",-70		,"Left word width selection", VK_LEFT, SHFT+CTL, SKF_PREVIEW,SPEC(0x43))
	ADD_KEYSTROKE(RWS	, "Rw",-71		,"Right word with selection", VK_RIGHT, SHFT+CTL, SKF_PREVIEW,SPEC(0x44))
	ADD_KEYSTROKE(UCS	, "Uc",-72		,"Up cursor with selection", VK_UP, SHFT, SKF_PREVIEW,SPEC(0x45))
	ADD_KEYSTROKE(DCS	, "Dc",-73		,"Down cursor with selection", VK_DOWN, SHFT, SKF_PREVIEW,SPEC(0x46))
	ADD_KEYSTROKE(LLS	, "Ll",-74		,"Select to start of line", VK_HOME, SHFT, SKF_PREVIEW,SPEC(0x47))
	ADD_KEYSTROKE(RLS	, "Rl",-75		,"Select to end of line", VK_END, SHFT, SKF_PREVIEW,SPEC(0x48))
	ADD_KEYSTROKE(ULS	, "Ul",-76		,"Select to top", VK_HOME, SHFT+CTL, SKF_PREVIEW,SPEC(0x49))
	ADD_KEYSTROKE(DLS	, "Dl",-77		,"Select to bottom", VK_END, SHFT+CTL, SKF_PREVIEW,SPEC(0x4a))
	ADD_KEYSTROKE(USS	, "Us",-78		,"Select up a page", VK_PRIOR, SHFT, SKF_PREVIEW,SPEC(0x4b))
	ADD_KEYSTROKE(DSS	, "Ds",-79		,"Select down a page", VK_NEXT, SHFT, SKF_PREVIEW,SPEC(0x4c))
	ADD_KEYSTROKE(DD	, "DD",-80		,"Drag and drop selection", _, _, _,SPEC(0x4d))
	ADD_KEYSTROKE(DH	, "DH",-81		,"Delete Highlighted selection", _, _, _,SPEC(0x4e))

// end of GUI text selection keys

	ADD_KEYSTROKE(BH	, "BH",-82	 	,"Run to exit (in tracer)", _, _, SKF_CONF,SPEC(0x4f))
	ADD_KEYSTROKE(BP	, "BP",-83		,"Toggle breakpoint", _, _, SKF_CONF,SPEC(0x50))
	ADD_KEYSTROKE(AB	, "AB",-84		,"Prompt user to abort changes?", _, _, _,SPEC(0x51))

// Indent / Undent 

	ADD_KEYSTROKE(HT	, "HT",-85	,"", VK_TAB, _, _,SPEC(0x52))
	ADD_KEYSTROKE(TH	, "TH",-86	,"", _, _, _,SPEC(0x53))

	ADD_KEYSTROKE(RM	, "RM",-87		,"Resume execution (in tracer)", _, _, SKF_CONF,SPEC(0x54))
	ADD_KEYSTROKE(CBP	, "CB",-88		,"Clear all trace/stop/monitor in function", _, _, _,SPEC(0x55))
	ADD_KEYSTROKE(PRP	, "PR",-89		,"Show object properties", _, _, _,SPEC(0x56))

// quadsr/dq exit specials 	

#if !defined UNICODE_PASS
	ADD_KEYSTROKE(TERMSR_CONTINUE	,"", -90	,"", _, _, _,0)
	ADD_KEYSTROKE(TERMSR_STOP	,"", -91	,"", _, _, _,0)
#endif // !defined UNICODE_PASS

// editor commands

	ADD_KEYSTROKE(TGL	, "TL",-92		,"Toggle localisation", _, _, SKF_CONF,SPEC(0x59))

	ADD_KEYSTROKE(UA	, "UA",-93		,"Undo all changes", _, _, SKF_CONF,SPEC(0x5a))

	ADD_KEYSTROKE(AO	, "AO",-94		,"Comment out selected lines", _, _, SKF_CONF,SPEC(0x5b))
	ADD_KEYSTROKE(DO	, "DO",-95		,"Uncomment selected lines", _, _, SKF_CONF,SPEC(0x5c))
	ADD_KEYSTROKE(GTL	, "GL",-96		,"Go to Line", _, _, SKF_CONF,SPEC(0x5d))

	ADD_KEYSTROKE(CH	, "CH",-97		,"Change editor Hint", _, _, _,SPEC(0x5e))

	ADD_KEYSTROKE(PTU	, "PU",-99		,"Paste unicode", _, _, _,SPEC(0x5f))
	ADD_KEYSTROKE(PTA	, "PA",-100		,"Paste ansi", _, _, _,SPEC(0x60))
// 101-109 defined below. dunno why they are moved
	ADD_KEYSTROKE(SA	, "SA",-112		,"Select all", _, _, SKF_CONF,SPEC(0x61))
	ADD_KEYSTROKE(RZ	, "RZ",-110		,"Snap window size", _, _, SKF_CONF,SPEC(0x62))
	ADD_KEYSTROKE(AC	, "AC",-111		,"Align Comments", _, _, SKF_CONF,SPEC(0x63))
	ADD_KEYSTROKE(MA	, "MA",-109		,"Resume all threads (in tracer)", _, _, SKF_CONF,SPEC(0x64))
	ADD_KEYSTROKE(OF	, "OF",-113		,"Open file in editor", _, _, SKF_CONF,SPEC(0x65))
	ADD_KEYSTROKE(SF	, "FS",-114		,"Save file in editor", _, _, SKF_CONF,SPEC(0x66))
	ADD_KEYSTROKE(SFA	, "FA",-115		,"Save file \"As\" in editor", _, _, SKF_CONF,SPEC(0x67))
	ADD_KEYSTROKE(TT	, "TT",-116		,"Toggle the tree view in the editor", _, _, SKF_CONF,SPEC(0x68))
	ADD_KEYSTROKE(FT	, "FT",-117		,"Switch focus between the tree view and the editor", _, _, SKF_CONF,SPEC(0x69))
	ADD_KEYSTROKE(PL	, "PL",-118		,"Go back to Previous Location", _, _, SKF_CONF,SPEC(0x6a))


//	Function Keys	

#if !defined (ILINECODES_PASS)

	ADD_KEYSTROKE(F0	, "",-256		,"F0", _	   , _, _,SPECFKEY(0x0))
	ADD_KEYSTROKE(F1	, "",(F0-1)	,"F1", VK_F1 , _, _,SPECFKEY(0x01))
	ADD_KEYSTROKE(F2	, "",(F0-2)	,"F2", VK_F2 , _, _,SPECFKEY(0x02))
	ADD_KEYSTROKE(F3	, "",(F0-3)	,"F3", VK_F3 , _, _,SPECFKEY(0x03))
	ADD_KEYSTROKE(F4	, "",(F0-4)	,"F4", VK_F4 , _, _,SPECFKEY(0x04))
	ADD_KEYSTROKE(F5	, "",(F0-5)	,"F5", VK_F5 , _, _,SPECFKEY(0x05))
	ADD_KEYSTROKE(F6	, "",(F0-6)	,"F6", VK_F6 , _, _,SPECFKEY(0x06))
	ADD_KEYSTROKE(F7	, "",(F0-7)	,"F7", VK_F7 , _, _,SPECFKEY(0x07))
	ADD_KEYSTROKE(F8	, "",(F0-8)	,"F8", VK_F8 , _, _,SPECFKEY(0x08))
	ADD_KEYSTROKE(F9	, "",(F0-9)	,"F9", VK_F9 , _, _,SPECFKEY(0x09))
	ADD_KEYSTROKE(F10	, "",(F0-10)	,"F10", VK_F10, _, _,SPECFKEY(0x0a))

#if !defined UNICODE_PASS

	ADD_KEYSTROKE(F11	, "",(F0-11)	,"F11", VK_F11, _, _,0)
	ADD_KEYSTROKE(F12	, "",(F0-12)	,"F12", VK_F12, _, _,0)

	ADD_KEYSTROKE(F13	, "",(F0-13)	,"F13", VK_F1 , SHFT, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F14	, "",(F0-14)	,"F14", VK_F2 , SHFT , SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F15	, "",(F0-15)	,"F15", VK_F3 , SHFT , SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F16	, "",(F0-16)	,"F16", VK_F4 , SHFT , SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F17	, "",(F0-17)	,"F17", VK_F5 , SHFT , SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F18	, "",(F0-18)	,"F18", VK_F6 , SHFT , SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F19	, "",(F0-19)	,"F19", VK_F7 , SHFT , SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F20	, "",(F0-20)	,"F20", VK_F8 , SHFT , SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F21	, "",(F0-21)	,"F21", VK_F9 , SHFT , SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F22	, "",(F0-22)	,"F22", VK_F10, SHFT, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F23	, "",(F0-23)	,"F23", VK_F11, SHFT, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F24	, "",(F0-24)	,"F24", VK_F12, SHFT, SKF_CONF|SKF_HIDE,0)

	ADD_KEYSTROKE(F25	, "",(F0-25)	,"F25", VK_F1 , CTL , SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F26	, "",(F0-26)	,"F26", VK_F2 , CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F27	, "",(F0-27)	,"F27", VK_F3 , CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F28	, "",(F0-28)	,"F28", VK_F4 , CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F29	, "",(F0-29)	,"F29", VK_F5 , CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F30	, "",(F0-30)	,"F30", VK_F6 , CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F31	, "",(F0-31)	,"F31", VK_F7 , CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F32	, "",(F0-32)	,"F32", VK_F8 , CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F33	, "",(F0-33)	,"F33", VK_F9 , CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F34	, "",(F0-34)	,"F34", VK_F10 , CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F35	, "",(F0-35)	,"F35", VK_F11 , CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F36	, "",(F0-36)	,"F36", VK_F12 , CTL, SKF_CONF|SKF_HIDE,0)

	ADD_KEYSTROKE(F37	, "",(F0-37)	,"F37", VK_F1 , SHFT|CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F38	, "",(F0-38)	,"F38", VK_F2 , SHFT|CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F39	, "",(F0-39)	,"F39", VK_F3 , SHFT|CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F40	, "",(F0-40)	,"F40", VK_F4 , SHFT|CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F41	, "",(F0-41)	,"F41", VK_F5 , SHFT|CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F42	, "",(F0-42)	,"F42", VK_F6 , SHFT|CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F43	, "",(F0-43)	,"F43", VK_F7 , SHFT|CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F44	, "",(F0-44)	,"F44", VK_F8 , SHFT|CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F45	, "",(F0-45)	,"F45", VK_F9 , SHFT|CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F46	, "",(F0-46)	,"F46", VK_F10 , SHFT|CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F47	, "",(F0-47)	,"F47", VK_F11 , SHFT|CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F48	, "",(F0-48)	,"F48", VK_F12 , SHFT|CTL, SKF_CONF|SKF_HIDE,0)

#if	UNIX	// conflicts with bill

	ADD_KEYSTROKE(F49	, "",(F0-49)	,"F49", VK_F1 , ALT, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F50	, "",(F0-50)	,"F50", VK_F2 , ALT, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F51	, "",(F0-51)	,"F51", VK_F3 , ALT, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F52	, "",(F0-52)	,"F52", VK_F4 , ALT, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F53	, "",(F0-53)	,"F53", VK_F5 , ALT, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F54	, "",(F0-54)	,"F54", VK_F6 , ALT, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F55	, "",(F0-55)	,"F55", VK_F7 , ALT, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F56	, "",(F0-56)	,"F56", VK_F8 , ALT, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F57	, "",(F0-57)	,"F57", VK_F9 , ALT, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F58	, "",(F0-58)	,"F58", VK_F10, ALT, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F59	, "",(F0-59)	,"F59", VK_F11, ALT, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F60	, "",(F0-60)	,"F60", VK_F12, ALT, SKF_CONF|SKF_HIDE,0)

#endif//UNIX

	ADD_KEYSTROKE(F61	, "",(F0-61)	,"F61", VK_F1 , ALT|SHFT, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F62	, "",(F0-62)	,"F62", VK_F2 , ALT|SHFT, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F63	, "",(F0-63)	,"F63", VK_F3 , ALT|SHFT, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F64	, "",(F0-64)	,"F64", VK_F4 , ALT|SHFT, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F65	, "",(F0-65)	,"F65", VK_F5 , ALT|SHFT, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F66	, "",(F0-66)	,"F66", VK_F6 , ALT|SHFT, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F67	, "",(F0-67)	,"F67", VK_F7 , ALT|SHFT, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F68	, "",(F0-68)	,"F68", VK_F8 , ALT|SHFT, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F69	, "",(F0-69)	,"F69", VK_F9 , ALT|SHFT, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F70	, "",(F0-70)	,"F70", VK_F10, ALT|SHFT, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F71	, "",(F0-71)	,"F71", VK_F11, ALT|SHFT, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F72	, "",(F0-72)	,"F72", VK_F12, ALT|SHFT, SKF_CONF|SKF_HIDE,0)

	ADD_KEYSTROKE(F73	, "",(F0-73)	,"F73", VK_F1 , ALT|CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F74	, "",(F0-74)	,"F74", VK_F2 , ALT|CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F75	, "",(F0-75)	,"F75", VK_F3 , ALT|CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F76	, "",(F0-76)	,"F76", VK_F4 , ALT|CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F77	, "",(F0-77)	,"F77", VK_F5 , ALT|CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F78	, "",(F0-78)	,"F78", VK_F6 , ALT|CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F79	, "",(F0-79)	,"F79", VK_F7 , ALT|CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F80	, "",(F0-80)	,"F80", VK_F8 , ALT|CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F81	, "",(F0-81)	,"F81", VK_F9 , ALT|CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F82	, "",(F0-82)	,"F82", VK_F10, ALT|CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F83	, "",(F0-83)	,"F83", VK_F11, ALT|CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F84	, "",(F0-84)	,"F84", VK_F12, ALT|CTL, SKF_CONF|SKF_HIDE,0)

	ADD_KEYSTROKE(F85	, "",(F0-85)	,"F85", VK_F1 , ALT|SHFT|CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F86	, "",(F0-86)	,"F86", VK_F2 , ALT|SHFT|CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F87	, "",(F0-87)	,"F87", VK_F3 , ALT|SHFT|CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F88	, "",(F0-88)	,"F88", VK_F4 , ALT|SHFT|CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F89	, "",(F0-89)	,"F89", VK_F5 , ALT|SHFT|CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F90	, "",(F0-90)	,"F90", VK_F6 , ALT|SHFT|CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F91	, "",(F0-91)	,"F91", VK_F7 , ALT|SHFT|CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F92	, "",(F0-92)	,"F92", VK_F8 , ALT|SHFT|CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F93	, "",(F0-93)	,"F93", VK_F9 , ALT|SHFT|CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F94	, "",(F0-94)	,"F94", VK_F10, ALT|SHFT|CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F95	, "",(F0-95)	,"F95", VK_F11, ALT|SHFT|CTL, SKF_CONF|SKF_HIDE,0)
	ADD_KEYSTROKE(F96	, "",(F0-96)	,"F96", VK_F12, ALT|SHFT|CTL, SKF_CONF|SKF_HIDE,0)

//	Go to Mode Mn	

	ADD_KEYSTROKE(M0	, "",-512	,"", _, _, _,0)


//	Old-style Labels on Function Keys	

	ADD_KEYSTROKE(L0	, "",-1024	,"", _, _, _,0)


//	Special attributes	

	ADD_KEYSTROKE(PROG	, "",-2048	,"", _, _, _,0)
	ADD_KEYSTROKE(USER	, "",-2049	,"", _, _, _,0)

//	New specials appear to go here - counting from -87024 towards zero

	ADD_KEYSTROKE(NOINPUTDEVICE	,"", -87021,"Attempt to read from closed file desciptor", _, _, _,0)
	ADD_KEYSTROKE(UNICODEFOLLOWS	,"", -87022,"Return next key with no processing", _, _, _,0)
	ADD_KEYSTROKE(MTQUEUE	, "",-87023	,"Flag for no key", _, _, _,0)
	ADD_KEYSTROKE(ILLEGAL	, "",-87024	,"Flag for illegal", _, _, _,0)
	ADD_KEYSTROKE(GUICOMPLETE,"", -87025	,"Indicates no char for gui keydown", _, _, _,0)
	ADD_KEYSTROKE(DONTTRANSLATE,"", -87026	,"Don't translate the next character", _, _, _,0)

// for the command below, characters are received in the sequence:
// 	DIRECTTOWINDOW,windowid,char
	ADD_KEYSTROKE(DIRECTTOWINDOW,"", -87027	,"Following character is sent to specific window", _, _, _,0)

#endif // defined UNICODE_PASS

#endif // !defined (ILINECODES_PASS)

	ADD_KEYSTROKE(TO	, "TO",-101		,"Toggle outline", _, _, SKF_CONF,SPEC(0x86))
	ADD_KEYSTROKE(MO	, "MO",-102		,"Goto matching outline", _, _, SKF_CONF,SPEC(0x87))
	ADD_KEYSTROKE(S1	, "S1",-103		,"Fix script and exit editor", _, _, SKF_CONF,SPEC(0x8d))
	ADD_KEYSTROKE(S2	, "S2",-104		,"Fix script but remain in editor", _, _, SKF_CONF,SPEC(0x8e))
	ADD_KEYSTROKE(OS	, "OS",-105		,"(IME only) Begin overstrike sequence", _, _,_, SPEC(0x8f))
	ADD_KEYSTROKE(IG	, "IG",-106		,"(IME only) Ignore next combination", _, _, _,SPEC(0x90))
	ADD_KEYSTROKE(SR	, "SR",-107		,"Screen redraw", _, _, _,SPEC(0x57))
	ADD_KEYSTROKE(IS	, "IS",-108		,"Initialize session", _, _, _,SPEC(0x58))	// bin marks, home cursor, empty line. For RIDE

#endif // KEYSTROKE_PASS==0

#undef _	

