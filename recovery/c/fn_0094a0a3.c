/* WIP: original stores the live ESP at [EBP-0x18] for its EH unwind record at
   VA 0x0094a0c6. Plain C has no supported way to reproduce that exact stack
   value/layout while keeping the required four-DWORD SEH record. */
#include "cmre.h"
