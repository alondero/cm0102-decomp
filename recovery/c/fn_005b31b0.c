#include "cmre.h"

/* WIP: the branch at 0x005b3206 enters 0x005b3270 with this function's live
   0x43c-byte frame, saved ECX/EBP, and incoming stack arguments. That code also
   executes REP STOSD without establishing DF. A standalone C call would change
   the inherited frame; plain C has no supported way to preserve the live DF. */
