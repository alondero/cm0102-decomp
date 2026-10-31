#include "cmre.h"

/* Element records: 0x1be bytes apart, 20 per row; rows 8920 (0x22d8) bytes apart,
   indexed by the signed byte argument. Object fields by byte offset from `self`:
   0x4792 u32 pointer, 0x4796 start of the element table, 0x1ce u16 array (stride 2),
   0xaf u8 array (stride 1). Element fields by byte offset: 0x19 s8 flag,
   0x69 u32 value, 0x6d u32 pointer (its 0x2f s8, 0xb s16 and 0x5b s8 are
   read through it). */
#define MEM_U8(a)  (*(volatile u8 *)(a))
#define MEM_S8(a)  (*(volatile s8 *)(a))
#define MEM_U16(a) (*(volatile u16 *)(a))
#define MEM_S16(a) (*(volatile s16 *)(a))
#define MEM_U32(a) (*(volatile u32 *)(a))

extern u32 CM_FASTCALL cm_fn_00717410(u32 self);
extern u32 CM_STDCALL cm_fn_005b5a60(u32 arg);
extern int CM_THISCALL cm_fn_004cf5a0(u32 self, u32 arg);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32 self, u32 arg2, u32 arg3);
extern int CM_CDECL cm_fn_0090cfc0(int arg);
extern s8 CM_THISCALL cm_fn_00897590(u32 self, u32 arg2, u32 arg3, u32 arg4);
extern s8 CM_FASTCALL cm_fn_006ebd70(u32 self);
extern void CM_THISCALL cm_fn_006eecb0(u32 self, u32 arg);

void CM_THISCALL cm_fn_006bbb60(u32 self, u32 arg)
{
    u32 si = (u32)(s32)(s8)arg;
    u32 acc;
    u32 counter;
    u32 e;
    u32 rowbase;

    counter = 0x14u;
    MEM_U16(self + si * 2u + 0x1ceu) = 0u;
    acc = 0u;
    MEM_U8(self + si + 0xafu) = 0u;
    e = self + 0x4796u + si * 8920u;
    rowbase = e;

    do {
        if (MEM_S8(e + 0x19u) >= 0) {
            u32 r1 = cm_fn_00717410(e);
            u32 r2 = cm_fn_005b5a60(r1);
            u32 x;
            u32 ebx;
            u32 g;
            u32 r4;
            int r3;

            if ((acc & r2) != 0u) {
                MEM_U16(self + si * 2u + 0x1ceu) += 0xfe0cu;
            }
            acc = acc + r2;

            g = CM_GLOBAL(volatile u32, 0x00ae1050);
            x = MEM_U32(e + 0x69u);
            r3 = cm_fn_004cf5a0(g, x);
            CM_GLOBAL(volatile u32, 0x00ae1080) = (u32)r3;
            if (r3 != 0) {
                u32 m = MEM_U32((u32)r3 + 4u);
                u32 table = CM_GLOBAL(volatile u32, 0x00ae23b4);
                ebx = table + 581u * m;
            } else {
                ebx = 0u;
            }

            g = CM_GLOBAL(volatile u32, 0x00ae1050);
            r4 = cm_fn_004cf5f0(g, x, ebx);

            if (r4 != 0u && ebx != 0u) {
                u32 p;
                int r5 = cm_fn_0090cfc0(0x14);

                p = MEM_U32(e + 0x6du);
                if (!((s32)MEM_S8(p + 0x2fu) > r5)) goto join1;

                {
                    int r6 = cm_fn_0090cfc0(0x1b58);
                    p = MEM_U32(e + 0x6du);
                    if (!((s32)MEM_S16(p + 0xbu) > r6)) goto join1;
                }

                {
                    int r7 = cm_fn_0090cfc0(6);
                    if (!((s32)MEM_S8(x + 0x5bu) < r7)) goto join1;
                }

                {
                    u16 ax = (u16)(MEM_U16(ebx + 0x80u) - 0x2eeu);
                    p = MEM_U32(e + 0x6du);
                    if (!((s32)MEM_S16(p + 0xbu) > (s32)(s16)ax)) goto join1;
                }

                if ((MEM_U32(r4 + 0x45u) & 0x17a0bu) == 0u) goto join1;

                {
                    u8 al = (u8)arg;
                    u32 pp = MEM_U32(self + 0x4792u);
                    u8 dl;
                    u32 val;
                    u32 w;
                    s8 ret;
                    u32 ec;

                    if (al != 0u) {
                        dl = MEM_U8(pp + 0x41u);
                        val = MEM_U32(pp + 0x20u);
                    } else {
                        dl = MEM_U8(pp + 0x40u);
                        val = MEM_U32(pp + 0x1cu);
                    }
                    w = (dl == 0u) ? 1u : 0u;
                    ret = cm_fn_00897590(CM_GLOBAL(volatile u32, 0x00de1d08), val, w, 0u);
                    ec = self + 0x4796u + 446u * ((u32)(s32)ret + 20u * si);

                    if (e == ec) {
                        s8 r9 = cm_fn_006ebd70(e);
                        MEM_U16(self + si * 2u + 0x1ceu) += (u16)(0u - 3u * (u32)(s32)r9);
                    } else {
                        int r10 = cm_fn_0090cfc0((int)MEM_S8(x + 0x5bu));
                        if (r10 == 0) {
                            s8 r11 = cm_fn_006ebd70(e);
                            MEM_U16(self + si * 2u + 0x1ceu) += (u16)(0u - 2u * (u32)(s32)r11);
                        } else {
                            s8 r12 = cm_fn_006ebd70(e);
                            MEM_U16(self + si * 2u + 0x1ceu) -= (u16)(s16)r12;
                        }
                    }
                }
            }

join1:
            {
                u8 al = (u8)arg;
                u32 pp = MEM_U32(self + 0x4792u);
                u8 dl;
                u32 val;
                u32 w;
                s8 ret;
                u32 ec;

                if (al != 0u) {
                    dl = MEM_U8(pp + 0x41u);
                    val = MEM_U32(pp + 0x20u);
                } else {
                    dl = MEM_U8(pp + 0x40u);
                    val = MEM_U32(pp + 0x1cu);
                }
                w = (dl == 0u) ? 1u : 0u;
                ret = cm_fn_00897590(CM_GLOBAL(volatile u32, 0x00de1d08), val, w, 0u);
                ec = self + 0x4796u + 446u * ((u32)(s32)ret + 20u * si);

                if (e != ec) {
                    s8 r14 = cm_fn_006ebd70(e);
                    MEM_U16(self + si * 2u + 0x1ceu) += (u16)(s16)r14;
                }
            }
        }

        e = e + 0x1beu;
        counter = counter - 1u;
    } while (counter != 0u);

    e = rowbase;
    counter = 0x14u;
    do {
        if (MEM_S8(e + 0x19u) >= 0) {
            cm_fn_006eecb0(e, 1u);
        }
        e = e + 0x1beu;
        counter = counter - 1u;
    } while (counter != 0u);
}
