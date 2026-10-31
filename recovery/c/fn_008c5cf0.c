#include "cmre.h"

/* Object fields by byte offset from the object pointer in ECX:
   0x00 array base (table of element pointers), 0x04 signed element count.
   Element fields by byte offset: 0x00 header pointer, 0x0c key dword,
   0x20 sub-object passed as the source of 0x0068d0d0, 0x2c kind byte,
   0x2e byte. Header fields: 0x02 dword read, 0x0a dword written.
   Stack arguments: a1 (pointer to key; reused as the return counter once
   the original overwrites that slot), a2 (self for 0x0068d0d0), a3 (flag). */
#define OBJ_U32(off) (*(volatile u32 *)(self + (off)))

extern unsigned long long CM_THISCALL cm_fn_0068d0d0(volatile u8 *self, const u8 *src, s16 count);
extern u32 CM_FASTCALL cm_fn_0068d1b0(u32 header);

u32 CM_THISCALL cm_fn_008c5cf0(u32 self, u32 a1, u32 a2, u32 a3)
{
    u32 key;
    s32 count;
    u32 base;
    u32 i;
    u32 element;
    u32 header;
    u32 r;

    /* 0x008c5cf9: mov edi, [eax] reads the key before the null test. */
    key = *(volatile u32 *)a1;
    if (a1 != 0 && (s32)key >= CM_GLOBAL(s32, 0x00ae2364)) {
        return 0;
    }

    count = (s32)OBJ_U32(0x04);
    base = OBJ_U32(0x00);
    if (count <= 0) {
        return 0;
    }

    i = 0;
    for (;;) {
        element = *(volatile u32 *)(base + i * 4u);
        if (element != 0 && *(volatile u32 *)(element + 0x0c) == key) {
            break;
        }
        i = i + 1;
        if ((s32)i >= count) {
            return 0;
        }
    }

    /* 0x008c5d40: re-read of the matching slot. */
    element = *(volatile u32 *)(base + i * 4u);
    if (element == 0) {
        return 0;
    }

    header = *(volatile u32 *)element;
    if (header != 0) {
        u32 tmp = *(volatile u32 *)(header + 0x02);
        *(volatile u32 *)(header + 0x0a) = tmp;
        a1 = 0;                                   /* [esp+0x18] = 0: counter */
        r = cm_fn_0068d1b0(*(volatile u32 *)element);
        while (r != 0) {
            u32 idx = *(volatile u32 *)r;
            u32 arr = OBJ_U32(0x00);
            u32 e2 = *(volatile u32 *)(arr + idx * 4u);
            u32 act = 0;

            if (e2 != 0 && *(volatile u32 *)(e2 + 0x0c) == key) {
                if (a3 != 0) {
                    act = 1;
                } else {
                    u8 kind = *(volatile u8 *)(e2 + 0x2c);
                    if (kind == 2 || kind == 3 || kind == 0x14 || kind == 0x10 || kind == 4) {
                        act = 0;
                    } else if (kind == 0x0f) {
                        act = (*(volatile u8 *)(e2 + 0x2e) < 3);
                    } else {
                        act = 1;
                    }
                }
            }
            if (act) {
                if (a2 != 0) {
                    cm_fn_0068d0d0((volatile u8 *)a2, (const u8 *)r, 4);
                }
                a1 = a1 + 1;
            }
            r = cm_fn_0068d1b0(*(volatile u32 *)element);
        }
        return a1;
    }

    /* header == 0 path (0x008c5de9). */
    if (a3 == 0) {
        u8 kind = *(volatile u8 *)(element + 0x2c);
        if (kind == 2 || kind == 3 || kind == 0x14 || kind == 0x10 || kind == 4) {
            return 0;
        }
        if (kind == 0x0f && *(volatile u8 *)(element + 0x2e) >= 3) {
            return 0;
        }
    }
    if (a2 != 0) {
        cm_fn_0068d0d0((volatile u8 *)a2, (const u8 *)(element + 0x20), 4);
    }
    return 1;
}
