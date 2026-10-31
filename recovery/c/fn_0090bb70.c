#include "cmre.h"

/* WIP: original VAs 0x0090bbf3, 0x0090bc09, and 0x0090bc1f execute
   REPNE SCASB, REP MOVSD, and REP MOVSB without clearing DF. Their source,
   destination, and scan progression therefore depend on incoming EFLAGS.DF.
   Plain C has no faithful expression for that inherited direction state; no
   forward-copy or zero-DF behavior is assumed. */
