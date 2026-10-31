/* WIP: both 0x004a2e10 calls produce an x87 value consumed immediately by
   0x00945640 (__ftol). That helper reads ST0 with FISTP under a temporary
   control word and returns EDX:EAX; the plain-C boundary cannot preserve this
   live x87 handoff. The entry uses ECX plus one stack DWORD and returns with
   RET 4. The first helper call receives ECX=[self+8], stack args (arg,0x13);
   the second receives the same ECX and (arg,0x12). No substitute conversion
   or numerical interpretation has been added. No test was run. */
#include "cmre.h"
