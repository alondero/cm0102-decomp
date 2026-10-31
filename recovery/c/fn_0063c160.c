/* Preferred-image VA 0063c160, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

extern void CM_THISCALL cm_fn_00767150(u32 self, u32 object, u32 mode);
/* The caller consumes EDX after this original call as well as EAX. The normal
   unsigned-64 ABI captures that physical register pair without an adapter. */
extern unsigned long long CM_THISCALL cm_fn_0086f950(u32 self, u32 id);
/* Preserve the original EAX on return, including the no-matching-slot path. */
extern u32 CM_THISCALL cm_fn_007671c0(u32 self, u32 first, u32 second,
                                    u32 kind_word, u32 date_word);

u32 CM_THISCALL cm_fn_0063c160(u32 self)
{
    u32 context, object, receiver, id, manager, result, edx;
    u32 date_word, kind_word, link, second, first;
    unsigned long long pair;

    context = D(self + 4u);
    object = D(self + 0xau);
    receiver = D(context + 0xcu);
    cm_fn_00767150(receiver, object, 0xffffffffu);
    id = D(0x009cf80cu);
    manager = D(0x00dd7f00u);
    pair = cm_fn_0086f950(manager, id);
    result = (u32)pair;
    edx = (u32)(pair >> 32);
    if (result != 0) {
        manager = D(0x00dd7f00u);
        date_word = (edx & 0xffff0000u) | W(manager + 0x16u);
        kind_word = (manager & 0xffffff00u) | B(manager + 0x14u);
        link = D(result + 0x39u);
        second = D(link);
        first = D(result);
        context = D(self + 4u);
        receiver = D(context + 0x28u);
        (void)cm_fn_007671c0(receiver, first, second, kind_word, date_word);
    }
    id = D(0x009cf810u);
    manager = D(0x00dd7f00u);
    pair = cm_fn_0086f950(manager, id);
    result = (u32)pair;
    edx = (u32)(pair >> 32);
    if (result != 0) {
        manager = D(0x00dd7f00u);
        date_word = (edx & 0xffff0000u) | W(manager + 0x16u);
        kind_word = (manager & 0xffffff00u) | B(manager + 0x14u);
        link = D(result + 0x39u);
        second = D(link);
        first = D(result);
        context = D(self + 4u);
        receiver = D(context + 0x44u);
        return cm_fn_007671c0(receiver, first, second, kind_word, date_word);
    }
    return result;
}
