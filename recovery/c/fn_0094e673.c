/* WIP: -O0 fault data address mismatch (stack address 16 bytes deeper than the
   original's frame). A per-function optimize attribute made it pass but only by
   matching compiler frame layout, which is not source evidence (C.md); removed
   by the parent. The logic itself passes at -O2. */
#include "cmre.h"

extern void CM_CDECL cm_fn_00948835(void);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);
extern void CM_CDECL cm_fn_00947b09(int param_1);
extern u32 CM_CDECL cm_fn_009488e0(u32 source);
extern u32 *CM_CDECL cm_fn_0094c3a0(u32 *param_1, u32 *param_2);

/* Cdecl, no arguments, `ret 0`, no return value. Works on the global string
   block at 0x00de2ff8 (NUL-terminated strings, ended by an empty string).
   Pass 1 walks the strings with the length helper at 0x9488e0 and counts those
   whose first byte is not '='. The count sizes a pointer array (count + 1
   dwords) from 0x944e46 stored at 0x00de2fb4. Pass 2 copies each non-'=' string
   with its NUL (length + 1 from 0x9488e0) into a new block from 0x944e46,
   stores the block pointer in the next array slot and calls 0x94c3a0 (dst,
   src). Failed allocations call 0x947b09(9). The block is then released with
   0x9452ca, the global string pointer and the array terminator are zeroed,
   and the flag at 0x00de462c is set to 1. The 0x00de4630 flag gates the lifted
   0x948835 call at entry. */
void CM_CDECL cm_fn_0094e673(void)
{
    u8 *cur;
    u32 n;
    u32 *slot;

    if (CM_GLOBAL(int, 0x00de4630) == 0)
        cm_fn_00948835();

    cur = CM_GLOBAL(u8 *, 0x00de2ff8);
    n = 0;
    while (*cur != 0) {
        if (*cur != '=')
            n++;
        cur += cm_fn_009488e0((u32)cur) + 1;
    }

    slot = (u32 *)(u32)cm_fn_00944e46(n * 4u + 4u);
    CM_GLOBAL(u32 *, 0x00de2fb4) = slot;
    if (slot == 0)
        cm_fn_00947b09(9);

    cur = CM_GLOBAL(u8 *, 0x00de2ff8);
    if (*cur != 0) {
        do {
            n = cm_fn_009488e0((u32)cur) + 1u;
            if (*cur != '=') {
                *slot = (u32)cm_fn_00944e46(n);
                if (*slot == 0)
                    cm_fn_00947b09(9);
                cm_fn_0094c3a0((u32 *)*slot, (u32 *)cur);
                slot++;
            }
            cur += n;
        } while (*cur != 0);
    }

    cm_fn_009452ca(CM_GLOBAL(u32, 0x00de2ff8));
    CM_GLOBAL(u32, 0x00de2ff8) = 0;
    *slot = 0;
    CM_GLOBAL(u32, 0x00de462c) = 1;
}
