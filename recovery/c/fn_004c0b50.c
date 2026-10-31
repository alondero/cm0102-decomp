#include "cmre.h"

extern u32 CM_CDECL cm_fn_009466d0(u32 destination, u32 source, u32 count);
extern s32 CM_CDECL cm_fn_00946898(u32 handle, u32 *out);

/* Cdecl, one stack argument (the record pointer), `ret 0`. The 0x118-byte
   frame block is one scratch array: `frame` is its base (L). The listing
   stores rec[1] and rec[2] into scratch[1] and scratch[0] before the first
   copy; the callee at 0x946898 then overwrites scratch[0]. Copy-in is
   rec+0x10 -> frame+0x14 (0x100 bytes); copy-out on success is
   frame+0x14 -> rec+0x10 (0xff bytes). Loop and copy-out order follow the
   listing: stores rec[1] then rec[2] before the copy-out. */
u32 CM_CDECL cm_fn_004c0b50(u32 *rec)
{
    u32 scratch[0x118 / 4];
    u8 *frame = (u8 *)scratch;
    s32 rc;
    u32 attr;

    if (rec[0] == 0xffffffffu)
        return 0xffffffffu;
    scratch[1] = rec[1];
    scratch[0] = rec[2];
    cm_fn_009466d0((u32)(frame + 0x14), (u32)(rec + 4), 0x100);
    rc = (s32)cm_fn_00946898(rec[0], scratch);
    if (rc == -1)
        return 0xffffffffu;
    for (;;) {
        attr = scratch[0];
        if ((rec[0x44] == 0x10 && (attr & 0x10) != 0) ||
            (rec[0x44] == 0 && (attr & 0x10) == 0))
            break;
        rc = (s32)cm_fn_00946898(rec[0], scratch);
        if (rc == -1)
            return 0xffffffffu;
    }
    rec[1] = scratch[4];
    rec[2] = attr;
    cm_fn_009466d0((u32)(rec + 4), (u32)(frame + 0x14), 0xff);
    *(u8 *)((u8 *)rec + 0x10f) = 0;
    return 0;
}
