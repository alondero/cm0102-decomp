#include "cmre.h"

/* Preferred-image VA 0x00582ef0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The record at scratch+8 has untouched gaps and an inherited flag nibble;
   neither is initialized. Callback-visible accesses retain listing order. */
extern u32 CM_THISCALL cm_fn_00549f70(u32, s16, s8, u16, u32);
extern u32 CM_FASTCALL cm_fn_0054a3f0(u32);
extern u32 CM_CDECL cm_fn_0053b3d0(u32);
extern void CM_THISCALL cm_fn_0085c260(u32, u32, u32, u32);
extern u8 CM_THISCALL cm_fn_005aa680(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_006827d0(u32, u32);
extern u32 CM_FASTCALL cm_fn_0057fa10(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(const char *, const char *, u32);

#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_THISCALL cm_fn_00582ef0(u32 self, s8 stage, u32 day, u32 month,
                             u32 year, u32 nation_a, u8 flag_a,
                             u32 nation_b, u8 flag_b)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x464];
    u8 *filename, *extension;
    u32 work = (u32)scratch, array, group, value, nibble, packed, parent;
    u32 first, second, receiver, table, target, eax, stamp, high, i, line;
    u16 date_first, date_second, saved;
    s8 selected;
    char *version;

    eax = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x009589bbu;
    frame.prev = eax;
    CM_FS(u32, 0) = (u32)&frame;
    array = D(self + 12u);
    if (array == 0) goto invalid;
    value = D(self + 0x30u);
    if ((s32)stage > (s32)value) goto invalid;
    if (stage != -1 && D(array + (u32)(s32)stage * 4u) == 0) goto invalid;
    if ((u16)year < 1998 || (u16)year > 1999 || stage < -1 || stage > 7) {
        filename = scratch + 0x264; extension = scratch + 0x64;
        line = 0x93d;
        goto error;
    }
    value = B(self + 0x49u) & 15u;
    nibble = D(work + 0x55u) & 0xfff0u;
    packed = value | nibble;
    value = B(self + 0x4au) & 15u;
    packed = (packed & 0xff0fu) | (value << 4);
    parent = D(self + 4u);
    W(work + 0x55u) = (u16)packed;
    first = D(parent);
    D(work + 0x1cu) = parent;
    saved = W(self + 0x40u);
    D(work + 8u) = first;
    W(work + 0x34u) = saved;
    W(work + 0x38u) = 0;
    if (stage == -1) {
        saved = W(self + 0x3au);
    } else {
        array = D(self + 12u);
        group = D(array + (u32)(s32)stage * 4u);
        saved = W(group + 0x3au);
    }
    W(work + 0x3au) = saved;
    value = B(self + 0xc4u);
    B(work + 0x42u) = (u8)value;
    saved = W(self + 0xdbu);
    W(work + 0x3eu) = saved;
    saved = W(self + 0xabu);
    W(work + 0x40u) = saved;
    B(work + 0x4bu) = flag_a;
    B(work + 0x4au) = (u8)stage;
    W(work + 0x55u) = (u16)(packed & 0xf0ffu);
    B(work + 0x4cu) = flag_b;
    for (i = 0x4d; i <= 0x54; ++i) B(work + i) = 0xff;
    D(work + 0x10u) = 0xffffffffu;
    B(work + 0x45u) = 0;
    B(work + 0x46u) = 0;
    B(work + 0x43u) = 0xff;
    B(work + 0x48u) = 1;
    B(work + 0x49u) = 1;
    cm_fn_00549f70(work, (s16)day, (s8)month, (u16)year, 0xffffffffu);
    date_first = W(work);
    date_second = W(work + 2u);
    frame.state = 0;
    W(work + 0x32u) = date_first;
    W(work + 0x30u) = date_second;
    eax = cm_fn_0054a3f0(work);
    if ((u16)eax == 5) {
        B(work + 0x47u) = 1;
    } else {
        eax = cm_fn_0054a3f0(work);
        B(work + 0x47u) = 2;
        if ((u16)eax == 6) B(work + 0x47u) = 1;
    }
    date_first = W(work + 0x32u);
    date_second = W(work + 0x30u);
    W(work + 0x2eu) = date_first;
    W(work + 0x2cu) = date_second;
    value = D(0x00ae23a8u);
    W(work + 0x3cu) = 0;
    first = cm_fn_0053b3d0(value + nation_a * 290u);
    D(work + 0x24u) = first;
    value = D(0x00ae23a8u);
    second = cm_fn_0053b3d0(value + nation_b * 290u);
    first = D(work + 0x24u);
    D(work + 0x28u) = second;
    value = D(first);
    D(work + 0x14u) = value;
    value = D(second);
    receiver = D(0x00dd7ef4u);
    D(work + 0x18u) = value;
    cm_fn_0085c260(receiver, 0, work + 8u, 0xffffffffu);
    stamp = D(0x00dd6ec8u);
    high = D(0x00dd6eccu);
    D(work + 0x5cu) = stamp;
    date_first = W(work + 0x5eu);
    D(work + 0x60u) = high;
    date_second = W(work + 2u);
    if (date_first < date_second ||
        (date_first == date_second && (s16)stamp < (s16)W(work))) {
        B(work + 0x4bu) = 0xff;
        B(work + 0x4cu) = 0xff;
    }
    receiver = D(0x00ae2a58u);
    cm_fn_005aa680(receiver, work + 8u, 1);
    if (B(work + 0x4bu) != 0xff) {
        table = D(self);
        target = D(table + 4u);
        ((u32 (CM_THISCALL *)(u32, u32, u32))target)(self, work + 8u, 0);
        selected = (s8)B(work + 0x4au);
        if (selected == -1) {
            cm_fn_006827d0(self, 0);
        } else {
            array = D(self + 12u);
            group = D(array + (u32)(s32)selected * 4u);
            cm_fn_006827d0(group, 0);
        }
    }
    eax = cm_fn_0057fa10(self);
    goto done;
invalid:
    filename = scratch + 0x364; extension = scratch + 0x164; line = 0x936;
error:
    cm_fn_00944cff(0x009c4674u, 0, 0, filename, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version,
                  filename, extension, line);
    eax = cm_fn_005e8290((const char *)0x009870e0u,
                        (const char *)0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
done:
    CM_FS(u32, 0) = frame.prev;
    return eax;
}
