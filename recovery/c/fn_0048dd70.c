#include "cmre.h"

/* Field access by byte offset from an object address. Offsets are the only
   names used for object fields: +0x00, +0x04, +0x14, +0x1c, +0x20, +0x53,
   +0x5d and +0x11c.
   Table at 0x00b63d60: 0x23 rows of 0x48 bytes. Row word +0x00 is compared with
   object addresses; the byte at 0x00b63d74 + 0x48 * i is the code returned when
   row i matches (see the row loops below). */
#define OBJ_U32(obj, off) (*(volatile u32 *)((u32)(obj) + (off)))
#define OBJ_U8(obj, off) (*(volatile u8 *)((u32)(obj) + (off)))
#define ROW_OBJ(i) CM_GLOBAL(volatile u32, 0x00b63d60u + 0x48u * (u32)(i))
#define ROW_CODE(i) CM_GLOBAL(u8, 0x00b63d74u + 0x48u * (u32)(i))
#define ROW_COUNT 0x23u

extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern unsigned long long CM_THISCALL cm_fn_005c6710(void *self, u32 arg);
extern char cm_fn_00669470(int *param_1);
extern u32 cm_fn_00669a10(int *param_1);
extern u32 CM_CDECL cm_fn_00669ae0(const volatile u32 *value);

u8 CM_CDECL cm_fn_0048dd70(u32 param_1, u32 param_2)
{
    u32 receiver;
    u32 field_5d;
    u32 r1;
    u32 word_9cf684;
    u32 recv2;
    u32 p14;
    u32 g_plus4;
    u32 obj_1c;
    u32 obj_20;
    u32 obj;
    u32 other;
    u32 field_now;
    u32 row;
    u32 i;
    u32 r;

    /* ECX is loaded from 0x00ae2c74 before any write, and 0x005c6710 receives
       it as its object. Its stack argument is param_1. */
    receiver = CM_GLOBAL(volatile u32, 0x00ae2c74u);
    field_5d = OBJ_U32(param_1, 0x5d);
    r1 = (u32)cm_fn_005c6710((void *)receiver, param_1);
    word_9cf684 = CM_GLOBAL(volatile u32, 0x009cf684u);

    if (r1 == 0 && OBJ_U32(param_1, 0x00) != word_9cf684) {
        goto fallback;
    }
    if (param_2 == 0) {
        goto fallback;
    }
    recv2 = CM_GLOBAL(volatile u32, 0x00ae2c74u);
    p14 = OBJ_U32(param_2, 0x14);
    g_plus4 = OBJ_U32(recv2, 0x04);
    if (p14 != g_plus4) {
        if (OBJ_U32(p14, 0x00) != word_9cf684) {
            goto fallback;
        }
    }

    /* Success path: both argument sub-objects are checked by 0x0053b900. */
    obj_1c = OBJ_U32(param_2, 0x1c);
    r = cm_fn_0053b900((u32 *)obj_1c);
    if (r != 0) {
        return 1;
    }
    obj_20 = OBJ_U32(param_2, 0x20);
    r = cm_fn_0053b900((u32 *)obj_20);
    if (r != 0) {
        return 1;
    }
    obj_1c = OBJ_U32(param_2, 0x1c);
    obj_20 = OBJ_U32(param_2, 0x20);
    obj = OBJ_U32(obj_1c, 0x53);
    other = OBJ_U32(obj_20, 0x53);
    if (obj != other) {
        return 3;
    }
    if (OBJ_U8(obj, 0x11c) == 0) {
        return 3;
    }
    if (obj == 0) {
        goto de36;
    }
    i = 0;
    do {
        row = ROW_OBJ(i);
        if (row != 0) {
            if (OBJ_U8(row, 0x11c) != 0 && row == obj) {
                return ROW_CODE(i);
            }
        }
        i++;
    } while (i < ROW_COUNT);

de36:
    r = cm_fn_0053b900((u32 *)obj_1c);
    return (u8)((r != 0 ? 0xfdu : 0u) + 3u);

fallback:
    field_now = OBJ_U32(param_1, 0x5d);
    if (field_now == 0) {
        goto dea2;
    }
    i = 0;
    do {
        row = ROW_OBJ(i);
        if (row != 0 && OBJ_U8(row, 0x11c) != 0 && row == field_5d) {
            return ROW_CODE(i);
        }
        i++;
    } while (i < ROW_COUNT);
    return 3;

dea2:
    r = cm_fn_00669a10((int *)param_1);
    if (r != 0) {
        return 0;
    }
    r = cm_fn_00669ae0((const volatile u32 *)param_1);
    if (r != 0) {
        return 1;
    }
    return (u8)cm_fn_00669470((int *)param_1);
}
