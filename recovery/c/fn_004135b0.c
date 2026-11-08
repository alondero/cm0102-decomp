/* Preferred-image VA004135b0, base00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ECX receiver, one full DWORD stack input, RET4. Both indexed callers discard
   EAX; the immediate native527f20 overwrites EAX before any use. Native callees
   remain explicit. Incoming EBX is saved then replaced by DWORD -1 at413604.
   The stream is captured after the first two allocations, at original4135e4.
   Subsequent allocation saves overwrite the incoming input argument object;
   the original cached EDI continues to carry the complete stream DWORD.
   Linked SEH953d06: first allocation has a separate saved slot; later saves use
   the input object. Unwinding/native exception dispatch remains a boundary. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_0040a350(u32 self, u32 param_2);
extern u32 CM_THISCALL cm_fn_00412540(u32 self, u32 input);
extern u32 *CM_THISCALL cm_fn_00424480(u32 *self, u32 arg);
extern u32 *CM_THISCALL cm_fn_0043d2b0(u32 *self, u32 arg);
extern u32 *CM_THISCALL cm_fn_00517f10(u32 *self, u32 arg);
extern u32 *CM_THISCALL cm_fn_00554dc0(u32 *self, u32 arg);
extern u32 CM_THISCALL cm_fn_00578780(u32 self, u32 input);
extern u32 CM_THISCALL cm_fn_00583400(u32 self, u32 stream);
extern u32 CM_THISCALL cm_fn_005a8f70(u32 self, u32 input);
extern u32 CM_THISCALL cm_fn_005c0980(u8 *self, u32 argument);
extern u32 CM_THISCALL cm_fn_005df650(u32 self, u32 input);
extern u32 CM_THISCALL cm_fn_005ec850(u32 self, u32 param_2);
extern u32 CM_THISCALL cm_fn_005f8fe0(u32 self, u32 param_2);
extern u32 CM_THISCALL cm_fn_00632ac0(u32 self, u32 stream);
extern u32 CM_THISCALL cm_fn_0063b640(u32 self, volatile u32 input);
extern u32 CM_THISCALL cm_fn_0065fe30(u32 self, u32 param_2);
extern u32 CM_THISCALL cm_fn_00666690(u32 self, u32 param_2);
extern u32 CM_CDECL cm_fn_006699d0(u32 key);
extern u32 CM_THISCALL cm_fn_0066e030(u32 self, u32 input);
extern u32 CM_THISCALL cm_fn_00792d80(u32 self, u32 input);
extern u32 CM_THISCALL cm_fn_00793940(u32 self, u32 input);
extern u32 CM_THISCALL cm_fn_0079d540(u32 self, u32 input);
extern u32 *CM_THISCALL cm_fn_0079dcf0(u32 *self, u32 argument);
extern u32 CM_THISCALL cm_fn_0079e580(u32 self, u32 stream);
extern u32 CM_THISCALL cm_fn_0079ea00(u32 self, u32 input);
extern u32 *CM_THISCALL cm_fn_007cd130(u32 *self, u32 argument);
extern u32 *CM_THISCALL cm_fn_007d3630(u32 *self, u32 argument);
extern u32 *CM_THISCALL cm_fn_007ec210(u32 *self, u32 argument);
extern u32 CM_THISCALL cm_fn_007f5170(u32 self, u32 input);
extern u32 *CM_THISCALL cm_fn_00858ea0(u32 *self, u32 arg);
extern u32 *CM_THISCALL cm_fn_00893720(u32 *self, u32 arg);
extern u32 *CM_THISCALL cm_fn_00901c70(u32 *self, u32 arg);
extern u32 CM_THISCALL cm_fn_00906980(u32 self, u32 input);
extern u32 CM_THISCALL cm_fn_0090ead0(u32 self, u32 input);
extern u32 CM_THISCALL cm_fn_0092fc10(u32 self, u32 stream);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern void *CM_CDECL cm_fn_00945501(u32 arg1, u32 arg2);

void CM_THISCALL cm_fn_004135b0(u32 self, volatile u32 input)
{
    volatile struct { u32 prev, handler, state; } frame;
    volatile u32 first_allocation __attribute__((unused));
    u32 allocated, stream, value, table, selector, index, base;
    u8 enabled;

    frame.state = 0xffffffffu;
    frame.handler = 0x00953d06u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    value = (u32)cm_fn_00945501(0x1eu, 4u);
    D(self + 8u) = value;
    allocated = (u32)cm_fn_00944cf1(0x17u);
    first_allocation = allocated;
    stream = input;
    frame.state = 0;
    if (allocated != 0) value = cm_fn_0092fc10(allocated, stream);
    else value = 0;
    table = D(self + 8u);
    D(table) = value;
    selector = D(0x009cff70u);
    frame.state = 0xffffffffu;
    if (selector != 0xffffffffu) {
        allocated = (u32)cm_fn_00944cf1(0x17u);
        input = allocated;
        frame.state = 1u;
        if (allocated != 0) value = (u32)cm_fn_00632ac0(allocated, stream);
        else value = 0;
        table = D(self + 8u);
        frame.state = 0xffffffffu;
        D(table + 0x04u) = value;
    } else {
        allocated = (u32)cm_fn_00944cf1(0x17u);
        input = allocated;
        frame.state = 2u;
        if (allocated != 0) value = (u32)cm_fn_0079e580(allocated, stream);
        else value = 0;
        table = D(self + 8u);
        frame.state = 0xffffffffu;
        D(table + 0x04u) = value;
    }

    /* 00413680..004136d2: helper predicate, before byte-gated records. */
    index = D(0x009cfa10u);
    base = D(0x00ae23a4u);
    value = cm_fn_006699d0(base + index * 0xc6u);
    if (value != 0) {
        allocated = (u32)cm_fn_00944cf1(0x17u);
        input = allocated;
        frame.state = 3u;
        if (allocated != 0) value = (u32)cm_fn_00583400(allocated, stream);
        else value = 0;
        table = D(self + 8u);
        frame.state = 0xffffffffu;
        D(table + 0x08u) = value;
    }

    /* Original table slot +0c, SEH state 4. */
    index = D(0x009cf218u);
    base = D(0x00ae23a8u);
    enabled = B(base + index * 0x122u + 0x11cu);
    if (enabled != 0) {
        allocated = (u32)cm_fn_00944cf1(0x17u);
        input = allocated;
        frame.state = 4u;
        if (allocated != 0) value = (u32)cm_fn_0040a350(allocated, stream);
        else value = 0;
        table = D(self + 8u);
        frame.state = 0xffffffffu;
        D(table + 0x0cu) = value;
    }

    /* Original table slot +10, SEH state 5. */
    index = D(0x009cf244u);
    base = D(0x00ae23a8u);
    enabled = B(base + index * 0x122u + 0x11cu);
    if (enabled != 0) {
        allocated = (u32)cm_fn_00944cf1(0x17u);
        input = allocated;
        frame.state = 5u;
        if (allocated != 0) value = (u32)cm_fn_00424480((u32 *)allocated, stream);
        else value = 0;
        table = D(self + 8u);
        frame.state = 0xffffffffu;
        D(table + 0x10u) = value;
    }

    /* Original table slot +14, SEH state 6. */
    index = D(0x009cf260u);
    base = D(0x00ae23a8u);
    enabled = B(base + index * 0x122u + 0x11cu);
    if (enabled != 0) {
        allocated = (u32)cm_fn_00944cf1(0x17u);
        input = allocated;
        frame.state = 6u;
        if (allocated != 0) value = (u32)cm_fn_0043d2b0((u32 *)allocated, stream);
        else value = 0;
        table = D(self + 8u);
        frame.state = 0xffffffffu;
        D(table + 0x14u) = value;
    }

    /* Original table slot +18, SEH state 7. */
    index = D(0x009cf2c0u);
    base = D(0x00ae23a8u);
    enabled = B(base + index * 0x122u + 0x11cu);
    if (enabled != 0) {
        allocated = (u32)cm_fn_00944cf1(0x17u);
        input = allocated;
        frame.state = 7u;
        if (allocated != 0) value = (u32)cm_fn_00554dc0((u32 *)allocated, stream);
        else value = 0;
        table = D(self + 8u);
        frame.state = 0xffffffffu;
        D(table + 0x18u) = value;
    }

    /* Original table slot +1c, SEH state 8. */
    index = D(0x009cf2e4u);
    base = D(0x00ae23a8u);
    enabled = B(base + index * 0x122u + 0x11cu);
    if (enabled != 0) {
        allocated = (u32)cm_fn_00944cf1(0x17u);
        input = allocated;
        frame.state = 8u;
        if (allocated != 0) value = (u32)cm_fn_00578780(allocated, stream);
        else value = 0;
        table = D(self + 8u);
        frame.state = 0xffffffffu;
        D(table + 0x1cu) = value;
    }

    /* Original table slot +20, SEH state 9. */
    index = D(0x009cf2fcu);
    base = D(0x00ae23a8u);
    enabled = B(base + index * 0x122u + 0x11cu);
    if (enabled != 0) {
        selector = D(0x009cff7cu);
        if (selector != 0xffffffffu) {
            allocated = (u32)cm_fn_00944cf1(0x17u);
            input = allocated;
            frame.state = 9u;
            if (allocated != 0) value = (u32)cm_fn_005a8f70(allocated, stream);
            else value = 0;
            table = D(self + 8u);
            frame.state = 0xffffffffu;
            D(table + 0x20u) = value;
        } else {
            allocated = (u32)cm_fn_00944cf1(0x17u);
            input = allocated;
            frame.state = 10u;
            if (allocated != 0) value = (u32)cm_fn_0079d540(allocated, stream);
            else value = 0;
            table = D(self + 8u);
            frame.state = 0xffffffffu;
            D(table + 0x20u) = value;
        }
    }

    /* Original table slot +24, SEH state 11. */
    index = D(0x009cf300u);
    base = D(0x00ae23a8u);
    enabled = B(base + index * 0x122u + 0x11cu);
    if (enabled != 0) {
        selector = D(0x009cff58u);
        if (selector != 0xffffffffu) {
            allocated = (u32)cm_fn_00944cf1(0x17u);
            input = allocated;
            frame.state = 11u;
            if (allocated != 0) value = (u32)cm_fn_005c0980((u8 *)allocated, stream);
            else value = 0;
            table = D(self + 8u);
            frame.state = 0xffffffffu;
            D(table + 0x24u) = value;
        } else {
            allocated = (u32)cm_fn_00944cf1(0x17u);
            input = allocated;
            frame.state = 12u;
            if (allocated != 0) value = (u32)cm_fn_0079dcf0((u32 *)allocated, stream);
            else value = 0;
            table = D(self + 8u);
            frame.state = 0xffffffffu;
            D(table + 0x24u) = value;
        }
    }

    /* Original table slot +28, SEH state 13. */
    index = D(0x009cf314u);
    base = D(0x00ae23a8u);
    enabled = B(base + index * 0x122u + 0x11cu);
    if (enabled != 0) {
        allocated = (u32)cm_fn_00944cf1(0x17u);
        input = allocated;
        frame.state = 13u;
        if (allocated != 0) value = (u32)cm_fn_005df650(allocated, stream);
        else value = 0;
        table = D(self + 8u);
        frame.state = 0xffffffffu;
        D(table + 0x28u) = value;
    }

    /* Original table slot +2c, SEH state 14. */
    index = D(0x009cf338u);
    base = D(0x00ae23a8u);
    enabled = B(base + index * 0x122u + 0x11cu);
    if (enabled != 0) {
        allocated = (u32)cm_fn_00944cf1(0x17u);
        input = allocated;
        frame.state = 14u;
        if (allocated != 0) value = (u32)cm_fn_005f8fe0(allocated, stream);
        else value = 0;
        table = D(self + 8u);
        frame.state = 0xffffffffu;
        D(table + 0x2cu) = value;
    }

    /* Original table slot +30, SEH state 15. */
    index = D(0x009cf2dcu);
    base = D(0x00ae23a8u);
    enabled = B(base + index * 0x122u + 0x11cu);
    if (enabled != 0) {
        selector = D(0x009cff70u);
        if (selector != 0xffffffffu) {
            allocated = (u32)cm_fn_00944cf1(0x17u);
            input = allocated;
            frame.state = 15u;
            if (allocated != 0) value = (u32)cm_fn_0063b640(allocated, stream);
            else value = 0;
            table = D(self + 8u);
            frame.state = 0xffffffffu;
            D(table + 0x30u) = value;
        } else {
            allocated = (u32)cm_fn_00944cf1(0x17u);
            input = allocated;
            frame.state = 16u;
            if (allocated != 0) value = (u32)cm_fn_0079ea00(allocated, stream);
            else value = 0;
            table = D(self + 8u);
            frame.state = 0xffffffffu;
            D(table + 0x30u) = value;
        }
    }

    /* Original table slot +34, SEH state 17. */
    index = D(0x009cf360u);
    base = D(0x00ae23a8u);
    enabled = B(base + index * 0x122u + 0x11cu);
    if (enabled != 0) {
        allocated = (u32)cm_fn_00944cf1(0x17u);
        input = allocated;
        frame.state = 17u;
        if (allocated != 0) value = (u32)cm_fn_0065fe30(allocated, stream);
        else value = 0;
        table = D(self + 8u);
        frame.state = 0xffffffffu;
        D(table + 0x34u) = value;
    }

    /* Original table slot +38, SEH state 18. */
    index = D(0x009cf36cu);
    base = D(0x00ae23a8u);
    enabled = B(base + index * 0x122u + 0x11cu);
    if (enabled != 0) {
        allocated = (u32)cm_fn_00944cf1(0x17u);
        input = allocated;
        frame.state = 18u;
        if (allocated != 0) value = (u32)cm_fn_00666690(allocated, stream);
        else value = 0;
        table = D(self + 8u);
        frame.state = 0xffffffffu;
        D(table + 0x38u) = value;
    }

    /* Original table slot +3c, SEH state 19. */
    index = D(0x009cf408u);
    base = D(0x00ae23a8u);
    enabled = B(base + index * 0x122u + 0x11cu);
    if (enabled != 0) {
        allocated = (u32)cm_fn_00944cf1(0x17u);
        input = allocated;
        frame.state = 19u;
        if (allocated != 0) value = (u32)cm_fn_00793940(allocated, stream);
        else value = 0;
        table = D(self + 8u);
        frame.state = 0xffffffffu;
        D(table + 0x3cu) = value;
    }

    /* Original table slot +40, SEH state 20. */
    index = D(0x009cf434u);
    base = D(0x00ae23a8u);
    enabled = B(base + index * 0x122u + 0x11cu);
    if (enabled != 0) {
        allocated = (u32)cm_fn_00944cf1(0x17u);
        input = allocated;
        frame.state = 20u;
        if (allocated != 0) value = (u32)cm_fn_007d3630((u32 *)allocated, stream);
        else value = 0;
        table = D(self + 8u);
        frame.state = 0xffffffffu;
        D(table + 0x40u) = value;
    }

    /* Original table slot +44, SEH state 21. */
    index = D(0x009cf45cu);
    base = D(0x00ae23a8u);
    enabled = B(base + index * 0x122u + 0x11cu);
    if (enabled != 0) {
        allocated = (u32)cm_fn_00944cf1(0x17u);
        input = allocated;
        frame.state = 21u;
        if (allocated != 0) value = (u32)cm_fn_007f5170(allocated, stream);
        else value = 0;
        table = D(self + 8u);
        frame.state = 0xffffffffu;
        D(table + 0x44u) = value;
    }

    /* Original table slot +48, SEH state 22. */
    index = D(0x009cf488u);
    base = D(0x00ae23a8u);
    enabled = B(base + index * 0x122u + 0x11cu);
    if (enabled != 0) {
        allocated = (u32)cm_fn_00944cf1(0x17u);
        input = allocated;
        frame.state = 22u;
        if (allocated != 0) value = (u32)cm_fn_00858ea0((u32 *)allocated, stream);
        else value = 0;
        table = D(self + 8u);
        frame.state = 0xffffffffu;
        D(table + 0x48u) = value;
    }

    /* Original table slot +4c, SEH state 23. */
    index = D(0x009cf4a8u);
    base = D(0x00ae23a8u);
    enabled = B(base + index * 0x122u + 0x11cu);
    if (enabled != 0) {
        allocated = (u32)cm_fn_00944cf1(0x17u);
        input = allocated;
        frame.state = 23u;
        if (allocated != 0) value = (u32)cm_fn_00893720((u32 *)allocated, stream);
        else value = 0;
        table = D(self + 8u);
        frame.state = 0xffffffffu;
        D(table + 0x4cu) = value;
    }

    /* Original table slot +50, SEH state 24. */
    index = D(0x009cf4f8u);
    base = D(0x00ae23a8u);
    enabled = B(base + index * 0x122u + 0x11cu);
    if (enabled != 0) {
        allocated = (u32)cm_fn_00944cf1(0x17u);
        input = allocated;
        frame.state = 24u;
        if (allocated != 0) value = (u32)cm_fn_00906980(allocated, stream);
        else value = 0;
        table = D(self + 8u);
        frame.state = 0xffffffffu;
        D(table + 0x50u) = value;
    }

    /* Original table slot +54, SEH state 25. */
    index = D(0x009cf430u);
    base = D(0x00ae23a8u);
    enabled = B(base + index * 0x122u + 0x11cu);
    if (enabled != 0) {
        allocated = (u32)cm_fn_00944cf1(0x17u);
        input = allocated;
        frame.state = 25u;
        if (allocated != 0) value = (u32)cm_fn_007cd130((u32 *)allocated, stream);
        else value = 0;
        table = D(self + 8u);
        frame.state = 0xffffffffu;
        D(table + 0x54u) = value;
    }

    /* Original table slot +58, SEH state 26. */
    index = D(0x009cf31cu);
    base = D(0x00ae23a8u);
    enabled = B(base + index * 0x122u + 0x11cu);
    if (enabled != 0) {
        allocated = (u32)cm_fn_00944cf1(0x17u);
        input = allocated;
        frame.state = 26u;
        if (allocated != 0) value = (u32)cm_fn_005ec850(allocated, stream);
        else value = 0;
        table = D(self + 8u);
        frame.state = 0xffffffffu;
        D(table + 0x58u) = value;
    }

    /* Original table slot +5c, SEH state 27. */
    index = D(0x009cf2acu);
    base = D(0x00ae23a8u);
    enabled = B(base + index * 0x122u + 0x11cu);
    if (enabled != 0) {
        allocated = (u32)cm_fn_00944cf1(0x17u);
        input = allocated;
        frame.state = 27u;
        if (allocated != 0) value = (u32)cm_fn_00517f10((u32 *)allocated, stream);
        else value = 0;
        table = D(self + 8u);
        frame.state = 0xffffffffu;
        D(table + 0x5cu) = value;
    }

    /* Original table slot +60, SEH state 28. */
    index = D(0x009cf224u);
    base = D(0x00ae23a8u);
    enabled = B(base + index * 0x122u + 0x11cu);
    if (enabled != 0) {
        allocated = (u32)cm_fn_00944cf1(0x17u);
        input = allocated;
        frame.state = 28u;
        if (allocated != 0) value = (u32)cm_fn_00412540(allocated, stream);
        else value = 0;
        table = D(self + 8u);
        frame.state = 0xffffffffu;
        D(table + 0x60u) = value;
    }

    /* Original table slot +64, SEH state 29. */
    index = D(0x009cf3e4u);
    base = D(0x00ae23a8u);
    enabled = B(base + index * 0x122u + 0x11cu);
    if (enabled != 0) {
        allocated = (u32)cm_fn_00944cf1(0x17u);
        input = allocated;
        frame.state = 29u;
        if (allocated != 0) value = (u32)cm_fn_00792d80(allocated, stream);
        else value = 0;
        table = D(self + 8u);
        frame.state = 0xffffffffu;
        D(table + 0x64u) = value;
    }

    /* Original table slot +68, SEH state 30. */
    index = D(0x009cf4dcu);
    base = D(0x00ae23a8u);
    enabled = B(base + index * 0x122u + 0x11cu);
    if (enabled != 0) {
        allocated = (u32)cm_fn_00944cf1(0x17u);
        input = allocated;
        frame.state = 30u;
        if (allocated != 0) value = (u32)cm_fn_00901c70((u32 *)allocated, stream);
        else value = 0;
        table = D(self + 8u);
        frame.state = 0xffffffffu;
        D(table + 0x68u) = value;
    }

    /* Original table slot +6c, SEH state 31. */
    index = D(0x009cf448u);
    base = D(0x00ae23a8u);
    enabled = B(base + index * 0x122u + 0x11cu);
    if (enabled != 0) {
        allocated = (u32)cm_fn_00944cf1(0x17u);
        input = allocated;
        frame.state = 31u;
        if (allocated != 0) value = (u32)cm_fn_007ec210((u32 *)allocated, stream);
        else value = 0;
        table = D(self + 8u);
        frame.state = 0xffffffffu;
        D(table + 0x6cu) = value;
    }

    /* Original table slot +70, SEH state 32. */
    index = D(0x009cf510u);
    base = D(0x00ae23a8u);
    enabled = B(base + index * 0x122u + 0x11cu);
    if (enabled != 0) {
        allocated = (u32)cm_fn_00944cf1(0x17u);
        input = allocated;
        frame.state = 32u;
        if (allocated != 0) value = (u32)cm_fn_0090ead0(allocated, stream);
        else value = 0;
        table = D(self + 8u);
        frame.state = 0xffffffffu;
        D(table + 0x70u) = value;
    }

    /* Original table slot +74, SEH state 33. */
    index = D(0x009cf484u);
    base = D(0x00ae23a8u);
    enabled = B(base + index * 0x122u + 0x11cu);
    if (enabled != 0) {
        allocated = (u32)cm_fn_00944cf1(0x17u);
        input = allocated;
        frame.state = 33u;
        if (allocated != 0) value = (u32)cm_fn_0066e030(allocated, stream);
        else value = 0;
        table = D(self + 8u);
        D(table + 0x74u) = value;
    }

    CM_FS(u32, 0) = frame.prev;
}

