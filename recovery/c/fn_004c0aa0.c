#include "cmre.h"

extern u32 CM_CDECL cm_fn_009466d0(u32 destination, u32 source, u32 count);
extern u32 CM_CDECL cm_fn_009467ce(const char *path, u32 *out);
extern s32 CM_CDECL cm_fn_00946898(u32 handle, u32 *out);

/* Cdecl, two stack arguments plus one, `ret 0`. The 0x118-byte frame block
   is one scratch array: `frame` is its base (L). Byte offsets 0x00..0x0f hold
   the four-dword lookup record, 0x10 is one dword, and 0x14..0x117 is the
   name buffer copied by the callee at 0x14. Stores into `rec` follow the
   listing order: rec[0x44] before the first lookup, rec[0] after it, then
   rec[2] before rec[1] on success. */
u32 CM_CDECL cm_fn_004c0aa0(const char *path, u32 *rec, u32 mode)
{
    u32 scratch[0x118 / 4];
    u8 *frame = (u8 *)scratch;
    s32 rc;
    u32 attr;

    rec[0x44] = mode;
    rc = (s32)cm_fn_009467ce(path, scratch);
    rec[0] = (u32)rc;
    for (;;) {
        if (rc == -1)
            return 0xffffffffu;
        attr = scratch[0];
        if ((rec[0x44] == 0x10 && (attr & 0x10) != 0) ||
            (rec[0x44] == 0 && (attr & 0x10) == 0))
            break;
        rc = (s32)cm_fn_00946898(rec[0], scratch);
    }
    rec[2] = attr;
    rec[1] = scratch[4];
    cm_fn_009466d0((u32)(rec + 4), (u32)(frame + 0x14), 0xff);
    *(u8 *)((u8 *)rec + 0x10f) = 0;
    return 0;
}
