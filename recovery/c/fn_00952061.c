/* WIP: the listing uses three string MOVSD instructions at 0x0095208d,
   0x0095208e, and 0x00952090 without clearing DF. The source/destination
   direction, and therefore input reads and stack writes, depend on incoming
   EFLAGS.DF. Plain C in the current boundary cannot transport that state.
   No test was run; the routine is not approximated with a forward copy. */
#include "cmre.h"
