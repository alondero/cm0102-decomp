#include "cmre.h"

/* WIP: the routine builds its result in x87 ST(0), then tail-jumps to
 * VA 0x00945640, which consumes that already-live ST(0) value. Plain C cannot
 * express this entry boundary without changing the x87 call sequence. The
 * observed calculation uses signed WORD [arg1+5], signed BYTE arg2, then
 * clamps the polynomial result between pinned doubles at 0x00967890 and
 * 0x00968ea8 before the tail jump. */
