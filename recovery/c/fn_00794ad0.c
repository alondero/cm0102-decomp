/* Translation at preferred-image VA 0x00794ad0, image base 0x00400000.
   Target SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Thiscall receiver (ECX) is the base of a 32-entry dword table written in order:
   two entries per outer pass, sixteen outer passes. Stack argument at [esp+4] is a
   dword compared against zero; ret 4. Returns the receiver in EAX.
   The SEH registration is one volatile frame {prev, handler, state} linked at
   fs:[0] and unlinked on both exits (ADR 0003). Callees stay explicit boundaries. */
#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_FASTCALL cm_fn_0068d020(u32 value);
extern void CM_THISCALL cm_fn_00794be0(u32 receiver, u32 arg);

struct seh_frame {
    u32 prev;     /* +0: fs:[0] before linking */
    u32 handler;  /* +4: original handler address 0x0095e98b */
    u32 state;    /* +8: registration state word */
};

u32 CM_THISCALL cm_fn_00794ad0(u32 receiver, u32 stack_4)
{
    volatile struct seh_frame frame;
    u32 this_save;
    u32 outer;
    u32 inner;
    u32 cursor;
    u32 tmp;
    u32 value;
    u32 g;
    u32 sign;
    u32 abs_value;

    frame.state = 0xffffffff;
    frame.handler = 0x0095e98b;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    this_save = receiver;
    cursor = receiver;
    outer = 0x10;
    do {
        inner = 2;
        do {
            tmp = (u32)cm_fn_00944cf1(0xe);
            frame.state = 0;
            if (tmp != 0)
                value = cm_fn_0068d020(tmp);
            else
                value = 0;
            *(volatile u32 *)cursor = value;
            cursor = cursor + 4;
            inner = inner - 1;
            frame.state = 0xffffffff;
        } while (inner != 0);
        outer = outer - 1;
    } while (outer != 0);

    if (stack_4 != 0) {
        g = CM_GLOBAL(volatile u32, 0x00dd7110);
        sign = (u32)((s32)g >> 31);
        abs_value = (g ^ sign) - sign;
        if ((s32)abs_value > 0x3e032) {
            cm_fn_00794be0(this_save, stack_4);
            CM_FS(u32, 0) = frame.prev;
            return this_save;
        }
    }

    CM_FS(u32, 0) = frame.prev;
    return this_save;
}
