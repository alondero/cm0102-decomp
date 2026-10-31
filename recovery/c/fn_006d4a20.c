#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_005c6710(void *self, u32 arg);
extern void CM_THISCALL cm_fn_0079ce10(void *self, u8 *arg);
extern s32 CM_CDECL cm_fn_0090cfc0(s32 param_1);

/* Thiscall with no stack arguments (ret 0). The receiver's fields, by offset: pointers at
   +0xf5b6 (to a dword) and +0x4792 (to a record whose +0x14 pointer is used), bytes at
   +0xf5cb, +0xf5cc, +0xf601 and +0xf602, and a signed word at +0xf5ba. A small record of
   seven bytes is built on the stack: a dword (+0) and three bytes, cl (+4), bl (+5) and dl
   (+6), and its address is passed to 0x0079ce10 with the global 0x00b74340 as receiver.
   The routine exits early when a pointer is null, when the dword equals -1, or when the
   0x005c6710 check (run only if the global 0x00ae2c74 is nonzero) returns nonzero.
   The two random-gated adjustments use 0x0090cfc0 (cdecl) with arguments 500 and 25. */
void CM_THISCALL cm_fn_006d4a20(u8 *param_1)
{
    u8 blk[8];
    u32 x;
    u32 v;
    u32 y;
    u32 z;
    u32 g;
    u8 c1;
    u8 d1;
    u8 b1;
    u8 b2;
    u8 cl;
    u8 bl;
    u8 dl;
    s16 w;
    long long prod;
    s32 h;
    s32 r;
    int done;

    x = *(u32 *)(param_1 + 0xf5b6);
    if (x == 0) {
        return;
    }
    v = *(u32 *)x;
    if (v == 0xffffffffU) {
        return;
    }
    y = *(u32 *)(param_1 + 0x4792);
    if (y == 0) {
        return;
    }
    z = *(u32 *)(y + 0x14);
    if (z == 0) {
        return;
    }
    g = CM_GLOBAL(volatile u32, 0x00ae2c74);
    if (g != 0) {
        if ((u32)cm_fn_005c6710((void *)g, z) != 0) {
            return;
        }
    }

    c1 = *(u8 *)(param_1 + 0xf601);
    d1 = *(u8 *)(param_1 + 0xf5cc);
    b1 = *(u8 *)(param_1 + 0xf5cb);
    cl = (u8)(c1 + b1);
    b2 = *(u8 *)(param_1 + 0xf602);
    bl = (u8)(b2 + d1);
    w = (s16)*(u16 *)(param_1 + 0xf5ba);
    prod = (long long)(s32)0x10624dd3U * (long long)w;
    h = (s32)((unsigned long long)prod >> 32);
    h = h >> 6;
    h = h + (s32)((u32)h >> 31);
    dl = (u8)h;

    blk[4] = cl;
    blk[5] = bl;

    if ((s8)cl > 2) {
        dl = (u8)(dl - 2);
    } else if ((s8)cl > 1) {
        dl = (u8)(dl - 1);
    } else if ((s8)cl == 0) {
        dl = (u8)(dl + 1);
    }
    if ((s8)bl > 5) {
        dl = (u8)(dl - 1);
    } else if ((s8)bl < 3) {
        dl = (u8)(dl + 1);
    }

    blk[6] = dl;
    done = 0;
    if ((s8)dl < 4) {
        r = cm_fn_0090cfc0(500);
        if (r != 0) {
            dl = 4;
            done = 1;
        } else {
            dl = blk[6];
        }
    }
    if (!done) {
        if ((s8)dl > 8) {
            r = cm_fn_0090cfc0(25);
            if (r != 0) {
                dl = 8;
                done = 1;
            } else {
                dl = blk[6];
            }
        }
    }
    if (!done) {
        if ((s8)dl < 1) {
            dl = 1;
        } else if ((s8)dl > 10) {
            dl = 10;
        }
    }

    blk[6] = dl;
    x = *(u32 *)(param_1 + 0xf5b6);
    if (x == 0) {
        v = 0xffffffffU;
    } else {
        v = *(u32 *)x;
    }
    *(u32 *)blk = v;
    g = CM_GLOBAL(volatile u32, 0x00b74340);
    cm_fn_0079ce10((void *)g, blk);
}
