#include "cmre.h"

/* Object fields by byte offset from the object pointer in ECX (the `self`
   parameter). Offsets: 0x04 dword, 0x0c dword array base, 0x30 signed dword
   limit, 0x3e signed word count, 0x40 word, 0xa7 dword array base (6-byte
   records), 0xb2 dword array base, 0xb6 signed dword count, 0xb1 and 0x53 are
   fields of the array elements. */
#define OBJ_U16(off) (*(volatile u16 *)(self + (off)))
#define OBJ_U32(off) (*(volatile u32 *)(self + (off)))

/* Local frame. Relative to the frame base L (ESP after `sub esp, 0x10`):
   bytes 0x00..0x03 and 0x04..0x07 hold the two dwords read through arg1;
   bytes 0x08..0x0f are the 8-byte output written by the 0x00549f70 callee
   (word at 0x08, word at 0x0a, dword at 0x0c). */
typedef union {
    u8 bytes[0x10];
    u16 words[8];
    u32 dwords[4];
} cm_locals_t;

extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern short *CM_THISCALL cm_fn_00549f70(short *param_1, short param_2, char param_3, u16 param_4, u32 param_5);
extern u32 CM_THISCALL cm_fn_0051f890(int *param_1, int *param_2, int param_3);
extern void CM_THISCALL cm_fn_00775070(int param_1, int param_2, char param_3);
extern void CM_THISCALL cm_fn_00775220(int param_1, int *param_2, int *param_3);

u32 CM_THISCALL cm_fn_00915e40(u32 self, u32 arg1, u32 arg2)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    cm_locals_t frame;
    u32 key_lo;
    u32 key_hi;
    u32 g3;
    u32 g4;
    u32 g5;
    u32 edx_val;
    u32 base;
    u32 elem;
    u32 field;
    u32 field_ptr;
    u32 a;
    u32 arr2;
    u32 el;
    u32 ecx2;
    u32 eax2;
    s32 cnt;
    s32 limit;
    s32 e;
    u16 di;
    u8 bl;
    u32 r;

    /* SEH record: state -1, handler, then link into fs:[0]. */
    seh.state = -1;
    seh.handler = 0x00965868u;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    /* Output buffer at L+8 (8 bytes). The callee's param_4 is consumed only as
       its low word; the caller passes the word stored at object offset 0x40. */
    r = (u32)cm_fn_00549f70((short *)&frame.words[4], (short)0x000a, (char)0, (u16)OBJ_U16(0x40), 0xffffffffu);

    seh.state = 0;
    key_lo = *(volatile u32 *)arg1;
    key_hi = *(volatile u32 *)(arg1 + 4u);
    frame.dwords[0] = key_lo;
    frame.dwords[1] = key_hi;

    /* Word compares: high word of the first dword against the callee word at +0x0a,
       low word against the callee word at +0x08. */
    if (frame.words[1] != frame.words[5]) {
        goto done;
    }
    if ((u16)key_lo != frame.words[4]) {
        goto done;
    }
    if (arg2 == 0u) {
        goto done;
    }

    g3 = CM_GLOBAL(u32, 0x009cfa14);
    g4 = CM_GLOBAL(u32, 0x00ae23a4);
    edx_val = g4 + g3 * 198u;
    g5 = CM_GLOBAL(u32, 0x00b74318);
    cm_fn_00775070((int)g5, (int)edx_val, (char)0);

    /* First loop: over the pointer array at 0x0b2, count at 0x0b6. */
    cnt = (s32)OBJ_U32(0xb6);
    di = 0u;
    if (cnt > 0) {
        do {
            base = OBJ_U32(0xb2);
            e = (s32)(s16)di;
            elem = *(volatile u32 *)(base + (u32)e * 4u);
            if (elem != 0u) {
                a = OBJ_U32(0x04);
                field = *(volatile u32 *)(elem + 0x53u);
                g5 = CM_GLOBAL(u32, 0x00b74318);
                cm_fn_00775220((int)g5, (int *)field, (int *)a);
            }
            di = (u16)(di + 1u);
            cnt = (s32)OBJ_U32(0xb6);
        } while ((s32)(s16)di < cnt);
    }

    limit = (s32)OBJ_U32(0x30);
    bl = 0xffu;
    if (limit < -1) {
        goto done;
    }

    do {
        if (bl == 0xffu) {
            di = 0u;
            if ((s16)OBJ_U16(0x3e) > 0) {
                do {
                    a = OBJ_U32(0x04);
                    e = (s32)(s16)di;
                    edx_val = (u32)e * 3u;
                    base = OBJ_U32(0xa7);
                    elem = *(volatile u32 *)(base + edx_val * 2u);
                    field = *(volatile u32 *)(elem + 0x53u);
                    g5 = CM_GLOBAL(u32, 0x00b74318);
                    cm_fn_00775220((int)g5, (int *)field, (int *)a);
                    di = (u16)(di + 1u);
                } while ((s16)di < (s16)OBJ_U16(0x3e));
            }
        } else {
            arr2 = OBJ_U32(0xc);
            di = 0u;
            el = *(volatile u32 *)(arr2 + (u32)(s32)(s8)bl * 4u);
            if ((s16)*(volatile u16 *)(el + 0x3e) > 0) {
                do {
                    a = OBJ_U32(0x04);
                    e = (s32)(s16)di;
                    ecx2 = *(volatile u32 *)(el + 0xb1);
                    eax2 = (u32)e * 59u;
                    field_ptr = *(volatile u32 *)(ecx2 + eax2);
                    g5 = CM_GLOBAL(u32, 0x00b74318);
                    field = *(volatile u32 *)(field_ptr + 0x53u);
                    cm_fn_00775220((int)g5, (int *)field, (int *)a);
                    di = (u16)(di + 1u);
                } while ((s16)di < (s16)*(volatile u16 *)(el + 0x3e));
            }
        }
        bl = (u8)(bl + 1u);
        limit = (s32)OBJ_U32(0x30);
    } while ((s32)(s8)bl <= limit);

done:
    r = cm_fn_0051f890((int *)self, (int *)arg1, (int)arg2);
    CM_FS(u32, 0) = seh.prev;
    return r;
}
