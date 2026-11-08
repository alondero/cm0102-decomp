/* fn_007fc7c0: thiscall (ECX read before written = object pointer), one stack
   argument at [esp+4] popped by `ret 4`.
   Preferred-image VA 0x007fc7c0, image base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: mov eax,[esp+4]; mov ecx,[ecx+0x3840]; push eax; call 0x77a980; ret 4.
   The stack argument is forwarded unchanged; the callee takes the object
   stored at this+0x3840 as its ECX receiver and pops that argument itself. */
#include "cmre.h"

extern int CM_THISCALL cm_fn_0077a980(u32 self, u32 param_1);

int CM_THISCALL cm_fn_007fc7c0(u32 self, u32 param_1)
{
    u32 argument = param_1;
    u32 receiver = CM_GLOBAL(volatile u32, self + 0x3840u);

    return cm_fn_0077a980(receiver, argument);
}
