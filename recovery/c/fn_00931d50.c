/* WIP: outcome mismatch, cases 122 and 206 (original fault, recovered escape).
   Cause: the original callee 0x00945333 calls back into code-as-data at 0x00931f90
   (flag 0) or 0x00932230 (flag 2), which the harness does not treat as a function
   entry. The original faults inside that blob (read at [entry+0x0e]); the recovered run
   escapes at the blob's first byte. All other cases match.
   Blocking question: should the blob addresses be recovered or bounded as boundaries
   before this ID can pass?

   Tier-2 C recovery for the 3.9.68 primary executable, preferred VA 0x00931d50.
   Thiscall: ECX is the object; the callee pops 0x18 bytes (six stack slots).
   Parameters are named by their offset from the first stack argument:
   a00 = [esp+4] at entry, a04 = signed word, a08 = dword compared with the global
   at 0x009cfa1c, a0c = 16-bit mask, a10 = byte flag at entry that later holds the
   loop index, a14 = optional pointer. */
#include "cmre.h"

/* Object fields: 0x00 base pointer of 0x15-byte entries, 0x04 signed entry count. */
#define OBJ_U32(off) (*(volatile u32 *)(self + (off)))
#define MEM_U32(addr) (*(volatile u32 *)(addr))
#define MEM_U8(addr) (*(volatile u8 *)(addr))

extern void CM_CDECL cm_fn_00945333(u32 arg1, u32 arg2, u32 arg3, u32 arg4);
extern u32 CM_CDECL cm_fn_005481a0(u32 object);
extern u32 CM_FASTCALL cm_fn_0068d1b0(u32 object);

u16 CM_THISCALL cm_fn_00931d50(u32 self, u32 a00, s16 a04, u32 a08, u16 a0c,
                               u32 a10, u32 a14)
{
    u32 local8;   /* [esp+0x10] after the prologue: entries accepted so far */
    u32 local4;   /* [esp+0x14]: starts at 0x19, drops by 5 per outer pass */
    u32 ptr;      /* ebx: write cursor a00 + 4 * local8 */
    u32 edi;      /* byte offset of the current entry, 0x15 per index */
    u32 count;
    u32 base;
    u32 k;
    u32 e;
    u32 e0;
    u32 e1;
    u32 p4;
    u32 p7;
    u32 f;
    u32 t;
    u32 kconst;
    u32 cnt;
    u32 gbase;
    u8 flag;
    s32 lim;
    int rej;
    int do_call;

    flag = (u8)a10;
    local8 = 0u;
    local4 = 0x19u;

    /* Pass 1: one call to the original 0x00945333 chosen by the flag byte. */
    do_call = 1;
    switch (flag) {
    case 0u:
        kconst = 0x00931f90u;
        break;
    case 1u:
        kconst = 0x009320e0u;
        break;
    case 2u:
        kconst = 0x00932230u;
        break;
    case 3u:
        kconst = 0x009323b0u;
        break;
    case 4u:
        kconst = 0x00932500u;
        break;
    case 5u:
        kconst = 0x00932590u;
        break;
    default:
        do_call = 0;
        kconst = 0u;
        break;
    }
    if (do_call) {
        count = OBJ_U32(0x04);
        base = OBJ_U32(0x00);
        cm_fn_00945333(base, count, 0x15u, kconst);
    }

    /* Outer pass (0x00931de0). */
    for (;;) {
        count = OBJ_U32(0x04);
        a10 = 0u;
        if ((s32)count > 0) {
            ptr = a00 + 4u * local8;
            edi = 0u;
            /* Inner pass over entries (0x00931dfc). */
            for (;;) {
                lim = (s32)a04;
                if ((s32)local8 >= lim) {
                    break;
                }
                if ((s32)a10 >= (s32)count) {
                    return (u16)OBJ_U32(0x04);
                }
                base = OBJ_U32(0x00);
                e = base + edi;
                rej = 0;
                if ((s32)(s8)MEM_U8(e + 0x0eu) >= (s32)(s16)local4) {
                    /* Reject an index already listed in the output array. */
                    for (k = 0u; (s32)k < lim; k++) {
                        if (MEM_U32(a00 + 4u * k) == e) {
                            rej = 1;
                            break;
                        }
                    }
                    if (!rej) {
                        p4 = a08;
                        if (p4 != 0xffffffffu) {
                            if (p4 == CM_GLOBAL(volatile u32, 0x009cfa1c)) {
                                /* Chain: entry index -> record -> record -> object,
                                   which must hold p4 at its first dword. */
                                e0 = MEM_U32(e);
                                gbase = CM_GLOBAL(volatile u32, 0x00ae23bc);
                                t = MEM_U32(gbase + 110u * e0 + 0x39u);
                                if (t == 0u) {
                                    rej = 1;
                                } else {
                                    t = MEM_U32(t + 0x53u);
                                    if (t == 0u) {
                                        rej = 1;
                                    } else {
                                        t = MEM_U32(t + 0x71u);
                                        if (t == 0u) {
                                            rej = 1;
                                        } else if (MEM_U32(t) != p4) {
                                            rej = 1;
                                        }
                                    }
                                }
                            }
                            if (!rej) {
                                e1 = MEM_U32(e + 4u);
                                if ((s32)e1 > -1) {
                                    u32 g2 = CM_GLOBAL(volatile u32, 0x00ae23a4);
                                    u32 g3 = CM_GLOBAL(volatile u32, 0x00ae23a8);
                                    if (MEM_U32(g3 + 0x71u + 290u * e1) != g2 + 198u * p4) {
                                        rej = 1;
                                    }
                                }
                            }
                        }
                    }
                    if (!rej) {
                        u16 mask = a0c;
                        if (mask != 0u) {
                            e0 = MEM_U32(e);
                            gbase = CM_GLOBAL(volatile u32, 0x00ae23bc);
                            if (MEM_U32(gbase + 110u * e0 + 0x61u) != 0u) {
                                if ((u16)(cm_fn_005481a0(gbase + 110u * e0) & (u32)mask) == 0u) {
                                    rej = 1;
                                }
                            }
                        }
                    }
                    if (!rej) {
                        /* Optional dedupe through the list at a14. */
                        p7 = a14;
                        if (p7 != 0u) {
                            MEM_U32(p7 + 10u) = MEM_U32(p7 + 2u);
                            f = cm_fn_0068d1b0(p7);
                            while (f != 0u) {
                                t = MEM_U32(f);
                                base = OBJ_U32(0x00);
                                gbase = MEM_U32(t);
                                e0 = MEM_U32(base + edi);
                                if (gbase == e0) {
                                    rej = 1;
                                    break;
                                }
                                f = cm_fn_0068d1b0(p7);
                            }
                        }
                    }
                    if (!rej) {
                        base = OBJ_U32(0x00);
                        cnt = local8 + 1u;
                        MEM_U32(ptr) = base + edi;
                        local8 = cnt;
                        ptr = ptr + 4u;
                    }
                }
                /* 0x00931f35: advance to the next entry. */
                a10 = a10 + 1u;
                edi = edi + 0x15u;
                count = OBJ_U32(0x04);
                if (!((s32)a10 < (s32)count)) {
                    break;
                }
            }
        }
        /* 0x00931f50 */
        if (local8 == (u32)(s32)a04) {
            break;
        }
        local4 = local4 - 5u;
        if (!((s16)local4 > 0)) {
            break;
        }
    }
    return (u16)local8;
}
