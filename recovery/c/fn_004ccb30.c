#include "cmre.h"

/* Thiscall, `ret 8`: ECX = self; stack arg1 = key pointer (ebx); stack arg2 = value (ebp).
   Return value is EAX at the exit: 0 on the early exits, the computed byte address
   on the found path (the last `lea eax`/`mov [eax], cl` leaves it in EAX).
   Field offsets are from self (edi) and from the object at self+0x34 (edi+0x34). */
#define MEM_U32(addr) (*(volatile u32 *)(addr))
#define MEM_U8(addr) (*(volatile u8 *)(addr))

extern u32 CM_FASTCALL cm_fn_0068d1b0(u32 p);
extern u32 CM_THISCALL cm_fn_0068d1f0(u32 self, u32 target);
extern void CM_THISCALL cm_fn_007a81a0(u32 self, u32 p2, u32 p3, u32 p4);
extern void CM_THISCALL cm_fn_008c5430(u32 self, u32 p2);

u32 CM_THISCALL cm_fn_004ccb30(u32 self, u32 key_ptr, u32 val)
{
    u32 obj;
    u32 esi;
    u32 edx_v;
    u32 eax_v;
    u32 a63;
    u32 a67;
    u32 a6b;
    u32 recv;
    u32 k;
    u32 g;
    u32 addr;

    obj = MEM_U32(self + 0x34u);
    if (obj == 0u) {
        return 0u;
    }
    MEM_U32(obj + 0x0au) = MEM_U32(obj + 0x02u);
    esi = cm_fn_0068d1b0(MEM_U32(self + 0x34u));
    if (esi == 0u) {
        return 0u;
    }

    for (;;) {
        edx_v = MEM_U32(esi);
        eax_v = MEM_U32(key_ptr);
        if (edx_v == eax_v && MEM_U32(esi + 0x59u) == val) {
            break;
        }
        esi = cm_fn_0068d1b0(MEM_U32(self + 0x34u));
        if (esi == 0u) {
            return 0u;
        }
    }

    if (esi == 0u) {
        return 0u;
    }
    a63 = MEM_U32(esi + 0x63u);
    if (a63 != 0xffffffffu) {
        a6b = MEM_U32(esi + 0x6bu);
        a67 = MEM_U32(esi + 0x67u);
        recv = MEM_U32(0x00b7434cu);
        cm_fn_007a81a0(recv, a63, a67, a6b);
    }
    if (MEM_U8(esi + 0x50u) == 0x02u) {
        recv = MEM_U32(0x00de1f54u);
        cm_fn_008c5430(recv, key_ptr);
    }
    recv = MEM_U32(self + 0x34u);
    cm_fn_0068d1f0(recv, esi);

    k = MEM_U32(key_ptr);
    g = MEM_U32(0x00ae2d0cu);
    addr = k * 79u + g + 8u;
    MEM_U8(addr) = (u8)(MEM_U8(addr) - 1u);
    return addr;
}
