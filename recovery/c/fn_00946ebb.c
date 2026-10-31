#include "cmre.h"

/* WIP: after the time-query and comparison paths, original VAs 0x00946f5a
   through 0x00946f5d execute four MOVSD operations without clearing DF. The
   transfer direction and source/destination progression depend on incoming
   EFLAGS.DF. Plain C cannot preserve that inherited state; no forward copy is
   assumed. */
