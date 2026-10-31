#include "cmre.h"

/* Object fields by byte offset from the object pointer (arg1). Offsets:
   0x00 dword, 0x04 dword pointer, 0x08 dword pointer, 0x0c dword pointer,
   0x39 dword pointer, 0x3d signed byte, and dword fields at 0x33 of the
   objects those pointers name. */
#define OBJ_U8(off) (*(volatile u8 *)(obj + (off)))
#define OBJ_U32(off) (*(volatile u32 *)(obj + (off)))

/* Local frame. Relative to the frame base L (ESP after `sub esp, 0x160`):
   0x000..0x007 the two dwords read from 0x00dd6ec8 and 0x00dd6ecc;
   0x008..0x0e7 the record passed as `this` to 0x0077bbe0 and 0x00785790
   (the listing points every such call at L+8);
   0x0e8..0x15f the 30-dword buffer filled by 0x008d1da0 and read back below. */
typedef union {
    u8 bytes[0x160];
    u16 words[0x160 / 2];
    u32 dwords[0x160 / 4];
} cm_frame_t;

extern int cm_fn_0090cfc0(int param_1);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self_arg, u32 index_arg, u32 value_arg);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, u32 arg1, u32 arg2);
/* ECX is read on entry (`mov esi, ecx`), so the receiver is the ECX value the
   caller loads from 0x00de1f54; the four stack arguments follow. */
extern char CM_THISCALL cm_fn_008d1da0(u32 self, int param_1, u32 param_2, int *param_3, char param_4);

/* Void: no direct caller reads EAX after the call (cmre semantic.eax_readers). */
void CM_STDCALL cm_fn_0069fd50(u32 arg1, u32 arg2)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    cm_frame_t frame;
    u32 obj;
    u32 value;
    u32 g_a;
    u32 g_b;
    u32 g_e;
    u32 g_f;
    u32 obj1;
    u32 obj2;
    u32 obj3;
    u32 ptr39;
    u32 buf_index;
    u32 count;
    u16 cx;
    u16 g_c;
    int r;
    char al;

    seh.state = -1;
    seh.handler = 0x0095cd6bu;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    obj = arg1;
    if (obj == 0u) {
        goto done;
    }
    if (OBJ_U32(0x39) == 0u) {
        goto done;
    }
    r = cm_fn_0090cfc0(4);
    if (r == 0) {
        goto done;
    }

    g_a = CM_GLOBAL(u32, 0x00dd6ec8);
    g_b = CM_GLOBAL(u32, 0x00dd6ecc);
    g_c = CM_GLOBAL(u16, 0x00ae2c92);
    frame.dwords[0] = g_a;
    frame.dwords[1] = g_b;
    cx = frame.words[1];
    if (cx > g_c) {
        goto done;
    }
    if (cx == g_c) {
        if ((s16)(u16)g_a > (s16)CM_GLOBAL(u16, 0x00ae2c90)) {
            goto done;
        }
    }

    /* Main body: fill the record at L+8 through the callee's object methods. */
    cm_fn_0077bbe0((void *)&frame.bytes[8], 0x1776u, 0u);
    value = OBJ_U32(0x00);
    seh.state = 0;
    cm_fn_00785790((void *)&frame.bytes[8], 0u, value);

    obj1 = OBJ_U32(0x04);
    value = *(volatile u32 *)(obj1 + 0x33u);
    cm_fn_00785790((void *)&frame.bytes[8], 1u, value);

    obj2 = OBJ_U32(0x08);
    value = *(volatile u32 *)(obj2 + 0x33u);
    cm_fn_00785790((void *)&frame.bytes[8], 2u, value);

    obj3 = OBJ_U32(0x0c);
    value = *(volatile u32 *)(obj3 + 0x33u);
    cm_fn_00785790((void *)&frame.bytes[8], 3u, value);

    value = (u32)(s32)(s8)OBJ_U8(0x3d);
    cm_fn_00785790((void *)&frame.bytes[8], 4u, value);

    ptr39 = OBJ_U32(0x39);
    value = *(volatile u32 *)ptr39;
    cm_fn_00785790((void *)&frame.bytes[8], 5u, value);

    value = *(volatile u32 *)arg2;
    cm_fn_00785790((void *)&frame.bytes[8], 6u, value);

    r = cm_fn_0090cfc0(4);
    value = (u32)(s32)(s16)r;
    cm_fn_00785790((void *)&frame.bytes[8], 7u, value);

    g_e = CM_GLOBAL(u32, 0x00de1f54);
    if (g_e != 0u) {
        al = cm_fn_008d1da0(g_e, (int)obj, 0x52u, (int *)&frame.bytes[0xe8], (char)0x1e);
    } else {
        al = 0;
    }
    if ((s8)al <= 0) {
        goto done;
    }

    /* Loop over the al buffer entries (count is the signed byte in AL). */
    count = (u32)(s32)(s8)al;
    buf_index = 0u;
    do {
        value = frame.dwords[0xe8 / 4 + buf_index];
        g_f = CM_GLOBAL(u32, 0x00b7431c);
        cm_fn_007861e0((void *)g_f, (u32)&frame.bytes[8], value);
        buf_index = buf_index + 1u;
        count = count - 1u;
    } while (count != 0u);

done:
    CM_FS(u32, 0) = seh.prev;
}
