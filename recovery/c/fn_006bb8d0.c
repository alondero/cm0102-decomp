#include "cmre.h"

/* Original callee 0x00716450 (call site 0x006bb93a): __thiscall, ECX = entry
   pointer, one float stack argument that the callee pops (ret 4). The return
   value is never observed. */
extern CM_THISCALL void cm_fn_00716450(void *entry, float value);

/* fn_006bb8d0 (VA 0x006bb8d0): __thiscall with ECX = base object and two stack
   arguments: arg1 (EBX) is a pointer to one entry of the window, arg2 (EBP) is
   the float factor. Returns void (ret 8). */
CM_THISCALL void cm_fn_006bb8d0(void *self, void *arg1, float arg2)
{
    u8 *const owner = (u8 *)arg1;
    const s32 row = (s32)(s8)owner[0x27];
    u8 *const row_base = (u8 *)self + 0x4796 + (u32)row * 8920u;
    s32 i;

    for (i = 0; i < 0x14; i++) {
        u8 *const entry = row_base + (u32)i * 0x1beu;
        const u32 active = *(const u32 *)(const void *)(entry + 0x69);

        if (active != 0) {
            if (entry == owner) {
                /* The owner entry keeps the incoming factor unchanged. */
                cm_fn_00716450(entry, arg2);
            } else {
                const s32 sign = (s32)(s8)entry[0x19];
                long double v;

                if (sign >= 0) {
                    /* 0x006bb925: FADD [0x967890] then FMUL [0x968968]. */
                    v = ((long double)arg2
                         + (long double)CM_GLOBAL(double, 0x967890))
                        * (long double)CM_GLOBAL(double, 0x968968);
                } else {
                    /* 0x006bb913: FMUL [0x96a7a8] then FADD [0x96d0e8]. */
                    v = ((long double)arg2
                         * (long double)CM_GLOBAL(double, 0x96a7a8))
                        + (long double)CM_GLOBAL(double, 0x96d0e8);
                }

                cm_fn_00716450(entry, (float)v);
            }
        }
    }
}
