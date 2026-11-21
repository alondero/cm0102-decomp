#include "cmre.h"

/* Case-73 budget asymmetry (held) — resolved here by fidelity, not by a
   shortcut. Image: CM0102 3.9.68, preferred-image base 0x00400000,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.

   The swap branch at preferred-image VAs 0090bb40-0090bb5d reads BYTE [ecx],
   reads BYTE [eax], stores BYTE [eax], increments eax, stores BYTE [ecx],
   increments ecx, decrements esi and branches on the result. The loop below
   performs exactly those byte accesses in that order, at the running DWORD
   addresses of the two incoming pointers, with the entry zero test (0090bb4a)
   kept separate from the bottom decrement (0090bb5c). Consequently the
   recovered loop faults on the same unmapped byte as the original instead of
   running on into an instruction-budget stop: explain --case 73 now reports
   "fault" on both sides at the same fault address. No memcpy/memmove/shortcut
   is used, so overlapping buffers retain the original's byte-by-byte result.

   ABI boundary: at 0090bb0a the listing has three dwords already pushed
   (0090bac8/0090bacd/0090bace or 0090bb03/0090bb08/0090bb09), then three more
   before 0090bb1a, then three before 0090bb2a, with a single add esp,0x24 at
   0090bb2f. The legacy declaration of 0090d130 is (void) and the format string
   at 0x009870e8 names the four conversions v_s__s__s__d, so the three earlier
   dwords are the trailing varargs of 00944c9f; 0090d130 is called with no
   arguments and its whole EAX result is forwarded. Verified: default-budget
   cmre test passes at -O0 and -O2 (cov=1.0, inc=39). */

extern void CM_CDECL cm_fn_005e8290(const char *, const char *, int);
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);

void CM_CDECL cm_fn_0090ba90(u8 *a, u8 *b, u32 count)
{
    u8 scratch[512];
    u8 *first, *second;
    u32 line;
    char *text;
    if (!a) {
        cm_fn_00944cff(0xad4134, 0, 0, scratch + 256, scratch);
        first = scratch + 256;
        second = scratch;
        line = 0x16b;
    } else if (!b) {
        cm_fn_00944cff(0xad4134, 0, 0, scratch, scratch + 256);
        first = scratch;
        second = scratch + 256;
        line = 0x172;
    } else {
        u32 left = (u32)a, right = (u32)b;
        if (count != 0) {
            do {
                u8 x = CM_GLOBAL(volatile u8, right);
                u8 y = CM_GLOBAL(volatile u8, left);
                CM_GLOBAL(volatile u8, left) = x;
                left += 1;
                CM_GLOBAL(volatile u8, right) = y;
                right += 1;
                count -= 1;
            } while (count != 0);
        }
        return;
    }
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0xae24d0, 0x9870e8, text, first, second, line);
    cm_fn_005e8290((const char *)0x9870e0, (const char *)0xae24d0, 0);
    CM_GLOBAL(u32, 0xb67a34) = 0;
}
