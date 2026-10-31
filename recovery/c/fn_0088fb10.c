#include "cmre.h"
/* Preferred-image VA0088fb10, base00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS2/RET8/fullEAX self; SEH963169/scratch208 allocationL0/selfL4,
   diagnosticsL8/L108. Exact ordered remapping and dynamic count reloads.
   Original initialization helpers, allocation and failure paths retained. */
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))
#define G(a) CM_GLOBAL(volatile u32, a)
extern u32 CM_THISCALL cm_fn_00682200(u32);
extern u32 CM_THISCALL cm_fn_00687b10(u32, u32);
extern u32 CM_THISCALL cm_fn_00890550(u32);
extern u32 CM_THISCALL cm_fn_00890430(u32);
extern u32 CM_THISCALL cm_fn_006835c0(u32);
extern u32 CM_THISCALL cm_fn_006827d0(u32, u32);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_THISCALL cm_fn_0049ee70(u32, u32);
extern u32 CM_THISCALL cm_fn_0068a850(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_0088fb10(u32 self, volatile u16 year_arg,
                              volatile u32 context_arg)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x208];
    u32 l = (u32)scratch, context, year, base, limit, index, cursor;
    u32 pointer, tag, nation, expected, mapped, eax, first, second, line, version;
    frame.state = 0xffffffffu; frame.handler = 0x00963169;
    frame.prev = CM_FS(volatile u32, 0);
    CM_FS(volatile u32, 0) = (u32)&frame;
    D(l + 4) = self;
    cm_fn_00682200(self);
    context = context_arg; year = year_arg;
    D(self + 4) = context;
    frame.state = 0;
    D(self) = 0x00970198;
    W(self + 0x40) = (u16)year;
    B(self + 0x50) = 0x1c;
    if (cm_fn_00687b10(self, 1) != 0) goto done;
    year = G(0x00ae2350); base = G(0x00ae23b4);
    limit = G(0x00ae235c) - year * 2u;
    index = 0;
    if ((s32)limit > 0) {
        cursor = base + 0x57;
        do {
            pointer = D(cursor);
            if (pointer != 0) {
                tag = D(pointer);
                expected = G(0x009cf5f8);
                if (tag == expected || tag == G(0x009cf5fc) ||
                    tag == G(0x009cf604) || tag == G(0x009cf600) ||
                    tag == G(0x009cf608) || tag == G(0x009cf5f4)) {
                    pointer = D(cursor); D(cursor + 9) = pointer;
                    mapped = G(0x009cf7c8);
                } else {
                    nation = D(cursor - 4);
                    if (nation == 0) goto next_club;
                    nation = D(nation); expected = G(0x009cf4a8);
                    if (nation != expected) goto next_club;
                    if (tag == G(0x009cf5ec) || tag == G(0x009cf5f0) ||
                        tag == G(0x009cf7c8)) goto next_club;
                    mapped = G(0x009cf6ac);
                }
                mapped *= 107u;
                base = G(0x00ae23d0);
                D(cursor) = mapped + base;
            }
next_club:
            year = G(0x00ae2350);
            ++index; cursor += 581;
            limit = G(0x00ae235c) - year * 2u;
        } while ((s32)index < (s32)limit);
    }
    B(self + 0x44) = 0xff;
    D(self + 0x30) = 0xffffffffu;
    D(self + 0x2c) = 0; D(self + 0xc) = 0;
    if (cm_fn_00890550(self) == 0) {
        first = l + 0x108; second = l + 8; line = 0x82;
        goto error;
    }
    if (cm_fn_00890430(self) == 0) {
        first = l + 8; second = l + 0x108; line = 0x89;
        goto error;
    }
    if (cm_fn_006835c0(self) == 0) {
        first = l + 8; second = l + 0x108; line = 0x90;
        goto error;
    }
    if (cm_fn_006827d0(self, 0) == 0) {
        first = l + 8; second = l + 0x108; line = 0x98;
        goto error;
    }
    eax = (u32)cm_fn_00944cf1(0x5ce);
    D(l) = eax;
    B((u32)&frame.state) = 1;
    if (eax != 0) eax = cm_fn_0049ee70(eax, self);
    B((u32)&frame.state) = 0;
    D(self + 8) = eax;
    if (eax == 0) {
        first = l + 8; second = l + 0x108; line = 0x9f;
        goto error;
    }
    cm_fn_0068a850(self);
    goto done;
error:
    cm_fn_00944cff(0x00a8b578, 0, 0, first, second);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, first, second, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    G(0x00b67a34) = 0;
done:
    eax = frame.prev;
    CM_FS(volatile u32, 0) = eax;
    return self;
}
