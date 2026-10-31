#include "cmre.h"

extern u32 CM_THISCALL cm_fn_005601e0(u32 self, u32 length);
/* RET 12. Transport the ECX supplied by the caller even though the callee
   does not read it locally before its own calls. */
extern u32 CM_THISCALL cm_fn_00564080(u32 ecx, u32 network, u32 zero, u32 message);
extern u32 CM_THISCALL cm_fn_0077ac30(u32 self, u32 base, u32 length, u32 port);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 text, u32 zero);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944cff(u32 file, u32 zero1, u32 zero2, void *first, void *second);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);

#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))

static __attribute__((always_inline)) inline u32 report(u32 file, u8 *first,
                                                       u8 *second, u32 line)
{
    u32 version, result;
    cm_fn_00944cff(file, 0, 0, first, second);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0U, 0x009870e8U, version,
                  (u32)first, (u32)second, line);
    result = cm_fn_005e8290(0x009870e0U, 0x00ae24d0U, 0);
    D(0x00b67a34U) = 0;
    return result;
}

/* Preferred-image VA 00802f80, pinned 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS4 / RET 16. Preserve full incidental EAX, including partial AX writes.
   Scratch L+0 is the broadcast counter; diagnostic buffers are L+4/L+104. */
u32 CM_THISCALL cm_fn_00802f80(u32 self, volatile u32 text_arg,
                              volatile u32 field2, volatile u32 field3,
                              volatile u16 selector_arg)
{
    u32 scratch[0x204 / 4];
    u8 *local = (u8 *)scratch;
    u32 message = self + 0x37feU;
    u32 offset, limit, base, value, source, scan, remaining, length;
    u32 copy_length, destination, count, eax, selector, scale, port;
    u32 network, port_pointer, active_pointer;
    u8 byte;

    D(self + 0x3802U) = 0;
    offset = D(message + 4);
    limit = D(message + 8);
    if (offset + 1U <= limit) {
        base = D(message);
        B(base + offset) = 7;
        offset = D(message + 4);
        D(message + 4) = offset + 1U;
    } else {
        report(0x009c3734U, local + 0x104, local + 4, 0x85);
    }
    offset = D(message + 4);
    limit = D(message + 8);
    if (offset + 4U <= limit) {
        base = D(message);
        value = field2;
        D(base + offset) = value;
        offset = D(message + 4);
        D(message + 4) = offset + 4U;
    } else {
        report(0x009c3734U, local + 4, local + 0x104, 0x88);
    }
    offset = D(message + 4);
    limit = D(message + 8);
    if (offset + 4U <= limit) {
        base = D(message);
        value = field3;
        D(base + offset) = value;
        offset = D(message + 4);
        D(message + 4) = offset + 4U;
    } else {
        report(0x009c3734U, local + 4, local + 0x104, 0x88);
    }

    source = text_arg;
    scan = source;
    remaining = 0xffffffffU;
    while (remaining != 0) {
        byte = B(scan);
        ++scan;
        --remaining;
        if (byte == 0)
            break;
    }
    length = ~remaining;
    cm_fn_005601e0(message, length);
    base = D(message);
    copy_length = (u32)(s32)(s16)length;
    offset = D(message + 4);
    destination = base + offset;
    for (count = copy_length >> 2; count != 0; --count) {
        value = D(source);
        D(destination) = value;
        source += 4;
        destination += 4;
    }
    for (count = copy_length & 3; count != 0; --count) {
        byte = B(source);
        B(destination) = byte;
        ++source;
        ++destination;
    }
    offset = D(message + 4);
    offset += copy_length;
    selector = selector_arg;
    eax = (copy_length & 0xffff0000U) | selector;
    D(message + 4) = offset;

    if (selector != 0xffffU) {
        eax = (u32)(s32)(s16)selector;
        scale = eax * 0x300U;
        port = W(self + scale + 0x7d4U);
        port |= scale & 0xffff0000U;
        if ((u16)port == 0) {
            network = D(self + 0x3840U);
            return cm_fn_00564080(self + 0x3844U, network, 0, message);
        }
        if ((s16)W(self + eax * 2U + 0x380aU) <= 0)
            return eax;
        length = D(self + 0x3802U);
        base = D(message);
        network = D(self + 0x3840U);
        eax = cm_fn_0077ac30(network, base, length, port);
        if (eax == 0)
            eax = report(0x00a7ddbcU, local + 4, local + 0x104, 0xc74);
        return eax;
    }

    port_pointer = self + 0x7d4U;
    active_pointer = self + 0x380aU;
    D((u32)local) = 16;
    do {
        if ((s16)W(active_pointer) > 0) {
            port = W(port_pointer);
            eax = (eax & 0xffff0000U) | port;
            if ((u16)eax == 0) {
                network = D(self + 0x3840U);
                eax = cm_fn_00564080(self + 0x3844U, network, 0, message);
            } else {
                length = D(self + 0x3802U);
                base = D(message);
                network = D(self + 0x3840U);
                eax = cm_fn_0077ac30(network, base, length, eax);
                if (eax == 0)
                    eax = report(0x00a7ddbcU, local + 4, local + 0x104, 0xc8c);
            }
        }
        eax = D((u32)local);
        active_pointer += 2;
        port_pointer += 0x300U;
        --eax;
        D((u32)local) = eax;
    } while (eax != 0);
    return eax;
}
