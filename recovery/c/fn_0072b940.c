#include "cmre.h"

/* Object fields by byte offset from the object pointer (the argument):
   0x57 dword key, 0x80 signed word, 0xb3 / 0xb7 / 0xbb link pointers,
   0xcf pointer to a sub-record whose 0x57 and 0x5c are signed bytes and whose
   own first dword is returned. Sub-record 0x69 points to a record with a signed
   word at 0x0a. Candidate records are 581 (0x245) bytes apart from VA 0x00ae23b4. */
#define REC_U32(addr, off) (*(volatile u32 *)((u32)(addr) + (u32)(off)))
#define REC_U8(addr, off) (*(volatile u8 *)((u32)(addr) + (u32)(off)))
#define REC_S16(addr, off) (*(volatile s16 *)((u32)(addr) + (u32)(off)))
#define REC_S8(addr, off) (*(volatile s8 *)((u32)(addr) + (u32)(off)))

extern u32 CM_CDECL cm_fn_0053b900(u32 input);
extern void CM_CDECL cm_fn_005480f0(u32 p1, u32 p2, u32 p3);
extern s32 CM_CDECL cm_fn_0090cfc0(u32 value);
extern u8 CM_CDECL cm_fn_004b02e0(u32 p1, u32 p2);

u32 CM_STDCALL cm_fn_0072b940(u32 param_1)
{
    u32 scratch[20];
    u32 self;
    u32 p;
    u32 q;
    u32 e;
    u32 cand;
    u32 k;
    s32 w;
    s32 r;
    s32 r2;
    u8 bl;
    u8 al;

    self = param_1;
    if (REC_U32(self, 0xcf) == 0u) {
        return 0xffffffffu;
    }
    if (REC_U32(self, 0x57) == 0u) {
        if (cm_fn_0053b900(self) == 0u) {
            return 0xffffffffu;
        }
    }
    if (REC_U32(REC_U32(self, 0xcf), 0x69) == 0u) {
        return 0xffffffffu;
    }
    /* scratch is the listing's local buffer at esp+0xc; &param_1 is the incoming
       argument slot, which the callee writes and the code reads back. */
    cm_fn_005480f0(REC_U32(self, 0xcf), (u32)scratch, (u32)&param_1);
    al = REC_U8((u32)&param_1, 0u);
    bl = 0u;
    if ((s8)al > 0) {
        do {
            e = scratch[(u32)(s32)(s8)bl];
            if (e != 0u) {
                q = REC_U32(e, 0x69);
                if (q != 0u) {
                    w = (s32)REC_S16(q, 0x0a);
                    if (w + 0x2ee < w) {
                        if (cm_fn_0090cfc0(3u) != 0) {
                            return REC_U32(e, 0);
                        }
                    }
                }
            }
            bl = (u8)(bl + 1u);
        } while ((s8)bl < (s8)REC_U8((u32)&param_1, 0u));
    }

    /* Three link candidates (0xb3, 0xb7, 0xbb), each with the same guard chain. */
    for (k = 0u; k < 3u; k = k + 1u) {
        p = REC_U32(self, 0xb3u + 4u * k);
        if (p != 0u && REC_U32(p, 0xcf) != 0u) {
            if (cm_fn_0090cfc0((s32)REC_S8(REC_U32(p, 0xcf), 0x5c)) == 0) {
                r2 = cm_fn_0090cfc0((s32)REC_S8(REC_U32(p, 0xcf), 0x57));
                if (r2 > 5) {
                    /* The listing reads p's field before self's for 0x57, and
                       self's before p's for 0x80. */
                    u32 pk = REC_U32(p, 0x57);
                    u32 sk = REC_U32(self, 0x57);
                    if (pk == sk) {
                        s32 sw = (s32)REC_S16(self, 0x80);
                        s32 pw = (s32)REC_S16(p, 0x80);
                        if (sw + 0x2ee < pw && cm_fn_0090cfc0(3u) != 0) {
                            return REC_U32(REC_U32(p, 0xcf), 0);
                        }
                    }
                }
            }
        }
    }

    for (bl = 0u; (s8)bl < 0x32; bl = (u8)(bl + 1u)) {
        r = cm_fn_0090cfc0(CM_GLOBAL(u32, 0x00ae235c) - 2u * CM_GLOBAL(u32, 0x00ae2350));
        cand = CM_GLOBAL(u32, 0x00ae23b4) + (u32)r * 581u;
        if (cm_fn_004b02e0(self, cand) != 0u && REC_U32(cand, 0xcf) != 0u) {
            if (cm_fn_0090cfc0((s32)REC_S8(REC_U32(cand, 0xcf), 0x5c)) == 0) {
                if (cm_fn_0090cfc0((s32)REC_S8(REC_U32(cand, 0xcf), 0x57)) > 5) {
                    return REC_U32(REC_U32(cand, 0xcf), 0);
                }
            }
        }
    }
    return 0xffffffffu;
}
