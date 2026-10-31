/* Tier-2 C recovery for the 3.9.68 primary executable, preferred VA 0x0085b820.
   Thiscall: ECX is the object; the callee pops 4 bytes (one stack slot, a00, a
   pointer to a 4-byte-aligned record of signed words). */
#include "cmre.h"

/* Object fields: 0x00 base pointer of 12-byte entries, 0x04 signed entry count. */
#define OBJ_U32(off) (*(volatile u32 *)(self + (off)))
#define MEM_U32(addr) (*(volatile u32 *)(addr))
#define MEM_U16(addr) (*(volatile u16 *)(addr))

extern u32 CM_CDECL cm_fn_0053b3d0(u32 object);
extern u32 CM_THISCALL cm_fn_00549f70(u32 object, u32 p2, u32 p3, u32 p4, u32 p5);
extern void CM_FASTCALL cm_fn_0085dfc0(u32 object);
extern void CM_CDECL cm_fn_00945333(u32 arg1, u32 arg2, u32 arg3, u32 arg4);

void CM_THISCALL cm_fn_0085b820(u32 self, u32 a00)
{
    /* Frame words by byte offset from the frame base L (esp after the prologue):
       fr[0] = [L+0x10] count of matched entries (local_14),
       fr[1] = [L+0x14] four-byte record copied from a00 (low word at +0, high at +2),
       fr[2] = [L+0x18] second dword of that record,
       fr[3..] = [L+0x1c..] scratch that the callee 0x00549f70 writes through ECX. */
    u32 fr[8];
    u32 g1;
    u32 g2;
    u32 g4;
    u32 g5;
    u32 slot;
    u32 slot_value;
    u32 r;
    u32 puVar4;
    u32 base;
    u32 count;
    u32 k;
    u32 cnt;
    u32 n;
    u32 src;
    u32 dst;
    u32 i;
    s32 p2_0;
    u32 dxv;
    u32 cxv;
    s32 cnt_s;
    u32 ecx_v;
    u32 esi_ptr;

    fr[0] = 0u;
    if (MEM_U16(a00) == 0u) {
        cm_fn_0085dfc0(self);
    }

    if (MEM_U16(a00 + 2u) < 0x7d3u) {
        g1 = CM_GLOBAL(volatile u32, 0x009cf2e4);
        g2 = CM_GLOBAL(volatile u32, 0x00ae23a8);
        slot = g2 + 290u * g1 + 0x80u;
        slot_value = MEM_U32(slot);
        if (slot_value != 0u) {
            g4 = MEM_U32(slot_value);
            CM_GLOBAL(volatile u32, 0x00a835a4) = g4;
            MEM_U32(slot) = 0u;
            g1 = CM_GLOBAL(volatile u32, 0x009cf2e4);
            g2 = CM_GLOBAL(volatile u32, 0x00ae23a8);
            r = cm_fn_0053b3d0(g2 + 290u * g1);
            MEM_U32(r + 0x69u) = 0u;
        }
    }

    /* Scratch call: ECX is the scratch at L+0x1c; the callee returns a pointer. */
    puVar4 = cm_fn_00549f70((u32)&fr[3], 1u, 6u, 0x7d3u, 0xffffffffu);
    fr[1] = MEM_U32(puVar4);
    fr[2] = MEM_U32(puVar4 + 4u);
    if ((u16)(fr[1] >> 16) == MEM_U16(a00 + 2u) && (u16)fr[1] == MEM_U16(a00)) {
        g4 = CM_GLOBAL(volatile u32, 0x00a835a4);
        if (g4 != 0xffffffffu) {
            g5 = CM_GLOBAL(volatile u32, 0x00ae23b0);
            ecx_v = g5 + 78u * g4;
            g1 = CM_GLOBAL(volatile u32, 0x009cf2e4);
            g2 = CM_GLOBAL(volatile u32, 0x00ae23a8);
            MEM_U32(g2 + 290u * g1 + 0x80u) = ecx_v;
            g1 = CM_GLOBAL(volatile u32, 0x009cf2e4);
            g2 = CM_GLOBAL(volatile u32, 0x00ae23a8);
            esi_ptr = g2 + 290u * g1;
            r = cm_fn_0053b3d0(esi_ptr);
            ecx_v = MEM_U32(esi_ptr + 0x80u);
            MEM_U32(r + 0x69u) = ecx_v;
            g4 = CM_GLOBAL(volatile u32, 0x00a835a4);
            g5 = CM_GLOBAL(volatile u32, 0x00ae23b0);
            MEM_U32(g5 + 78u * g4 + 0x3cu) = 0x15f90u;
            g4 = CM_GLOBAL(volatile u32, 0x00a835a4);
            g5 = CM_GLOBAL(volatile u32, 0x00ae23b0);
            MEM_U32(g5 + 78u * g4 + 0x40u) = 0x15f90u;
            CM_GLOBAL(volatile u32, 0x00a835a4) = 0xffffffffu;
        }
    }

    /* 0x0085b969: entries are used only when a00[0] is a multiple of 30. */
    p2_0 = (s32)(s16)MEM_U16(a00);
    if (p2_0 % 30 == 0) {
        base = OBJ_U32(0x00);
        if (base != 0u) {
            count = OBJ_U32(0x04);
            if ((s32)count > 0) {
                fr[1] = MEM_U32(a00);
                dxv = (u16)(fr[1] >> 16);
                fr[2] = MEM_U32(a00 + 4u);
                k = 0u;
                for (;;) {
                    /* 0x0085b9a8: entry k at base + 12*k; key compared on its word at +6. */
                    cxv = MEM_U16(base + (u32)((s32)(s16)k * 12) + 6u);
                    if (dxv > cxv) {
                        /* advance */
                    } else if (dxv < cxv) {
                        break;
                    } else {
                        if (!((s16)(u16)fr[1] > (s16)MEM_U16(base + (u32)((s32)(s16)k * 12) + 4u))) {
                            break;
                        }
                    }
                    fr[0] = fr[0] + 1u;
                    k = (u16)(k + 1u);
                    if (!((s32)(s16)k < (s32)OBJ_U32(0x04))) {
                        break;
                    }
                }
                /* 0x0085b9e2 */
                if ((u16)fr[0] != 0u) {
                    cnt = fr[0];
                    cnt_s = (s32)(s16)(u16)cnt;
                    count = OBJ_U32(0x04);
                    if (cnt_s < (s32)count) {
                        /* Copy 12*cnt bytes from base + 12*(count-cnt-1) down to base. */
                        n = (((u32)cnt_s * 3u) << 2) >> 2;
                        src = base + 12u * (count - (u32)cnt_s - 1u);
                        dst = base;
                        for (i = 0u; i < n; i++) {
                            MEM_U32(dst + 4u * i) = MEM_U32(src + 4u * i);
                        }
                    }
                    count = OBJ_U32(0x04);
                    base = OBJ_U32(0x00);
                    count = count - (u32)cnt_s;
                    OBJ_U32(0x04) = count;
                    cm_fn_00945333(base, count, 0xcu, 0x0085e380u);
                }
            }
        }
    }
}
