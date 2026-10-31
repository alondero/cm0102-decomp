#include "cmre.h"

/* Preferred-image VA 0x005fe500, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The 79-byte record is copied/cleared as 19 DWORDs, one WORD, one BYTE.
   VA 005fe700 consumes two stack arguments and no incoming ECX. */
extern u32 CM_STDCALL cm_fn_005fe700(u32, u32);
extern u32 CM_FASTCALL cm_fn_006b5040(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_THISCALL cm_fn_005fe500(u32 self, u32 packet, u32 person)
{
    u8 scratch[512];
    u32 index, value, threshold, limit, record, destination, source, i, receiver;
    char *version;

    if (!person) {
        index = (u32)(s32)(s16)W(0x00b77c7a);
        person = D(0x00b74c26 + index * 768u);
        if (!person) return 0;
    }
    value = D(person);
    threshold = D(0x00ae2364);
    index = value - threshold + 16u;
    if ((s32)index < 0) goto error;
    limit = D(self + 4) - 1u;
    if ((s32)index > (s32)limit) goto error;
    if (packet) {
        if (!cm_fn_005fe700(packet, person)) return 0;
        threshold = (u32)(s32)(s16)W(0x00ae2c90);
        value = W(packet + 0x2a);
        if (value == threshold) {
            value = W(packet + 0x28);
            threshold = W(0x00ae2c92);
            if (value == threshold) {
                value = B(packet + 0x3f);
                threshold = B(0x00ae2c88);
                if (value == threshold && D(0x00b67a50) && !D(0x00b67a48)) return 0;
            }
        }
        value = D(self);
        destination = value + index * 42735u + 0x9ad4;
        source = packet;
        for (i = 0; i < 19; ++i) {
            value = D(source);
            D(destination) = value;
            source += 4;
            destination += 4;
        }
        value = W(source);
        W(destination) = (u16)value;
        value = B(source + 2);
        B(destination + 2) = (u8)value;
    } else {
        value = D(self);
        record = value + index * 42735u;
        threshold = (u32)(s32)(s16)W(0x00ae2c90);
        value = W(record + 0x9afe);
        if (value == threshold) {
            value = W(record + 0x9afc);
            threshold = W(0x00ae2c92);
            if (value == threshold) {
                value = B(record + 0x9b13);
                threshold = B(0x00ae2c88);
                if (value == threshold && D(0x00b67a50) && !D(0x00b67a48)) return 0;
            }
        }
        destination = record + 0x9ad4;
        for (i = 0; i < 19; ++i) {
            D(destination) = 0;
            destination += 4;
        }
        W(destination) = 0;
        B(destination + 2) = 0;
    }
    receiver = D(0x00b67a50);
    if (receiver) cm_fn_006b5040(receiver);
    return 1;
error:
    cm_fn_00944cff(0x009cf11c, 0, 0, scratch + 256, scratch);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, scratch + 256, scratch, 0x6d9);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
