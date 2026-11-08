/* Preferred-image VA 0x00521e00, base 0x00400000; CM 01/02 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   regparm(3) names actual EAX/EDX/ECX. The two trailing words are the
   physical stack arguments; all exits pop eight bytes. The full EAX
   carrier retains entry high24 or the actual last callback/load result.
   Only the exact owned self-entry is bound to this same C definition;
   every other target keeps its original indirect-call boundary. */
#include "cmre.h"

#define B(a) CM_GLOBAL(volatile u8, (a))
#define D(a) CM_GLOBAL(volatile u32, (a))

typedef u32 (CM_STDCALL __attribute__((regparm(3))) *predicate_callback)
    (u32, u32, u32, u32, u32);

u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_00521e00(
    u32 incoming_eax, u32 unused_edx, u32 self, u32 argument, u32 unused_argument)
{
    u32 carrier, index, table, child, vtable, target, count;
    u8 flag;
    (void)unused_edx;
    (void)unused_argument;
    flag = B(self + 0xb1u);
    carrier = (incoming_eax & 0xffffff00u) | flag;
    if (flag == 0)
        return carrier & 0xffffff00u;
    if (argument != 1u || B(self + 0x44u) != 0xffu)
        return (carrier & 0xffffff00u) | 1u;

    carrier = D(self + 0x30u);
    index = 0;
    if ((s32)carrier >= 0) {
        do {
            table = D(self + 0x0cu);
            child = D(table + index * 4u);
            vtable = D(child);
            target = D(vtable + 0x10u);
            /* Bind only this definition's exact original entry. Other targets
               retain their original value and ordinary indirect boundary. */
            if (target == 0x00521e00u)
                carrier = cm_fn_00521e00(table, vtable, child, 0u, 1u);
            else
                carrier = ((predicate_callback)target)(table, vtable, child, 0u, 1u);
            if ((u8)carrier == 0)
                return carrier & 0xffffff00u;
            carrier = D(self + 0x30u);
            index += 1u;
        } while ((s32)index <= (s32)carrier);
    }
    carrier = D(self + 0x2cu);
    count = D(self + 0x30u);
    carrier -= 1u;
    return (carrier & 0xffffff00u) | (u32)((s32)count >= (s32)carrier);
}
