/* Preferred-image VA 0x0053AC40, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: cdecl (ret 0, caller pops). The first argument is a byte at [esp+4],
   sign-extended and reduced by 5. The index is compared unsigned against 10 (ja),
   then dispatched through an 11-entry jump table at 0x0053ACC8. The second argument
   at [esp+8] is an output pointer. Table mapping (index: target):
     0, 7: push 0x9B6440, call 0x66F4E0(0xAE1980, string), then
           if out != NULL and 0x66F4D0() == 4, store AL (4) at *out;
     1, 8: as above with string 0x9B642C;
     3, 10: push 0x9B6448, call 0x66F4E0(0xAE1980, string), then
           if out != NULL and 0x66F4D0() == 4, store 1 at *out;
     2, 4, 5, 6, 9 and out-of-range indices: store 0 to the byte at 0xAE1980.
   Every path returns 0xAE1980 in EAX as a 32-bit pointer. The stored byte values and
   the callee's (discarded) return are kept exactly as in the listing. */
#include "cmre.h"

extern char *CM_CDECL cm_fn_0066f4e0(void *param_1, char *param_2);
extern u32 CM_CDECL cm_fn_0066f4d0(void);

char *CM_CDECL cm_fn_0053ac40(s8 selector, u8 *out)
{
    s32 index = (s32)selector - 5;
    char *result = &CM_GLOBAL(char, 0xae1980);
    u32 probe;

    switch (index) {
    case 0:
    case 7:
        cm_fn_0066f4e0(result, &CM_GLOBAL(char, 0x9b6440));
        if (out != 0) {
            probe = cm_fn_0066f4d0();
            if (probe == 4u) {
                *out = (u8)probe;
            }
        }
        return result;
    case 1:
    case 8:
        cm_fn_0066f4e0(result, &CM_GLOBAL(char, 0x9b642c));
        if (out != 0) {
            probe = cm_fn_0066f4d0();
            if (probe == 4u) {
                *out = (u8)probe;
            }
        }
        return result;
    case 3:
    case 10:
        cm_fn_0066f4e0(result, &CM_GLOBAL(char, 0x9b6448));
        if (out != 0) {
            probe = cm_fn_0066f4d0();
            if (probe == 4u) {
                *out = 1;
            }
        }
        return result;
    default:
        CM_GLOBAL(u8, 0xae1980) = 0;
        return result;
    }
}
