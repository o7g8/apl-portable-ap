#ifndef _MACHINE_H
#define _MACHINE_H

// This file is purely so that we pick up "port.h" from the appropriate place
// in the include path.
// Dyalog's developers should avoid using it to configure global information.
// If such information is required prior to port.h then put it in the mk_cc
// scripts. Otherwise d_machin.h is the correct place. History: d_machin.h used
// to be called default_machine.h before we had DOS and 8.3 file names.

	#include	<port.h>
	#include	<d_machin.h>

#endif // _MACHINE_H
