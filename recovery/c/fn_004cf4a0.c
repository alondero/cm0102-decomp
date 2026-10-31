#include "cmre.h"

/* Thiscall, `ret 4`: ECX = self; stack arg1 = key (ebx). Returns the match count (EAX = ebp).
   Field offsets are from self (edi) and from self+0x38 (the object list head) and the
   matched entry (esi). The 0x4cc490 call receives 7 stack arguments after ECX = self. */
#define MEM_U32(addr) (*(volatile u32 *)(addr))
#define MEM_U8(addr) (*(volatile u8 *)(addr))

extern u32 CM_FASTCALL cm_fn_0068d1b0(u32 p);
extern u32 CM_THISCALL cm_fn_0068d1f0(u32 self, u32 target);
extern u32 CM_THISCALL cm_fn_008c61e0(u32 self, u32 key);
extern void CM_THISCALL cm_fn_004cc490(u32 self, u32 p2, u32 p3, u32 p4,
                                       u32 p5, u32 p6, u32 p7, u32 p8);

u32 CM_THISCALL cm_fn_004cf4a0(u32 self, u32 key)
{
    u32 count;
    u32 obj;
    u32 esi;
    u32 f59;
    u32 f5d;
    u32 b61;
    u32 iv;
    u32 r;
    u32 merged;
    u32 recv;
    u32 edx_v;
    u32 ecx_v;

    count = 0u;
    obj = MEM_U32(self + 0x38u);
    if (obj == 0u) {
        return count;
    }
    MEM_U32(obj + 0x0au) = MEM_U32(obj + 0x02u);
    esi = cm_fn_0068d1b0(MEM_U32(self + 0x38u));
    if (esi == 0u) {
        return count;
    }

    for (;;) {
        if (MEM_U32(esi) == key) {
            f59 = MEM_U32(esi + 0x59u);
            if (f59 == 0xffffffffu) {
                b61 = MEM_U8(esi + 0x61u);
                merged = 0xffffff00u | b61;
                cm_fn_004cc490(self, esi, 0xffffffffu, 0xffffffffu, merged, 1u, 1u, 0u);
            } else {
                recv = MEM_U32(0x00de1f54u);
                r = cm_fn_008c61e0(recv, f59);
                if (r != 0u) {
                    iv = MEM_U32(r);
                    if (iv != 0u) {
                        edx_v = MEM_U32(iv + 0x24u);
                        ecx_v = MEM_U32(esi + 0x5du);
                        if (edx_v == ecx_v) {
                            if (MEM_U8(iv + 0x2cu) == 0x0fu) {
                                MEM_U8(iv + 0x2cu) = 0x08u;
                            }
                            b61 = MEM_U8(esi + 0x61u);
                            f5d = MEM_U32(esi + 0x5du);
                            f59 = MEM_U32(esi + 0x59u);
                            merged = (iv & 0xffffff00u) | b61;
                            cm_fn_004cc490(self, esi, f59, f5d, merged, 1u, 1u, 0u);
                        }
                    }
                }
            }
            recv = MEM_U32(self + 0x38u);
            cm_fn_0068d1f0(recv, esi);
            count = count + 1u;
        }
        esi = cm_fn_0068d1b0(MEM_U32(self + 0x38u));
        if (esi == 0u) {
            break;
        }
    }
    return count;
}
