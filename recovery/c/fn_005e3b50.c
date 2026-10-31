#include "cmre.h"

/* Callees. 0x005e3730 pops nothing itself (cdecl); the caller removes the
   pushed arguments in batches (add esp, 0x48 after three calls, add esp, 0x18
   after the fourth), and every call pushes six arguments. */
extern u32 CM_CDECL cm_fn_005e3680(s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 *arg5);
extern void cm_fn_005e3730(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6);

/* Indirect slot at [vtable + 0x14] called with ECX = the vtable value
   (`mov ecx, [eax]`) and six stack arguments that the callee pops. */
typedef void (CM_THISCALL *cm_slot_fn)(u32 ecx_value, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6);

/* Local frame L = entry ESP - 0x84 (after `sub esp, 0x74` and four pushes),
   byte offsets 0x00..0x73. Offsets used: 0x10 the 4-dword out-record passed
   to 0x005e3680 (its dwords 2 and 3 at 0x18 and 0x1c), 0x20 a dword written
   as 0x64 and passed by address, 0x70 the masked arg6 word. */
void cm_fn_005e3b50(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6)
{
    u32 frame[0x74 / 4];
    u32 flags;
    u32 g2;
    u32 r;
    u32 ebx_val;
    u32 edi_val;
    u32 vtable;
    u32 slot;

    /* A nonzero global at 0x00aebe0c returns before any work. */
    if (CM_GLOBAL(u32, 0x00aebe0c) != 0u) {
        return;
    }

    flags = (u8)arg5;
    if ((flags & 1u) != 0u) {
        cm_fn_005e3730(arg1, arg2, arg3, arg2, 1u, arg6);
        cm_fn_005e3730(arg3, arg2, arg3, arg4, 1u, arg6);
        cm_fn_005e3730(arg3, arg4, arg1, arg4, 1u, arg6);
        cm_fn_005e3730(arg1, arg4, arg1, arg2, 1u, arg6);
        return;
    }
    if ((flags & 2u) != 0u) {
        cm_fn_005e3730(arg1, arg2, arg3, arg2, 2u, arg6);
        cm_fn_005e3730(arg3, arg2, arg3, arg4, 2u, arg6);
        cm_fn_005e3730(arg3, arg4, arg1, arg4, 2u, arg6);
        cm_fn_005e3730(arg1, arg4, arg1, arg2, 2u, arg6);
        return;
    }

    g2 = CM_GLOBAL(u32, 0x00aebde4);
    if (g2 == 0u) {
        return;
    }
    r = cm_fn_005e3680((s32)arg1, (s32)arg2, (s32)arg3, (s32)arg4, (s32 *)&frame[4]);
    if (r == 0u) {
        return;
    }

    frame[0x70 / 4] = arg6 & 0xffffu;
    ebx_val = frame[0x18 / 4] + 1u;
    edi_val = frame[0x1c / 4] + 1u;
    frame[0x20 / 4] = 0x64u;
    frame[0x18 / 4] = ebx_val;
    frame[0x1c / 4] = edi_val;

    g2 = CM_GLOBAL(u32, 0x00aebde4);
    vtable = *(volatile u32 *)g2;
    slot = *(volatile u32 *)(vtable + 0x14u);
    ((cm_slot_fn)slot)(vtable, g2, (u32)&frame[4], 0u, 0u, 0x01000400u, (u32)&frame[8]);
}
