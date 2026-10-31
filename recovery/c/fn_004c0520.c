/* Preferred-image VA 004c0520, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_FASTCALL cm_fn_0068d1b0(u32);
extern void CM_FASTCALL cm_fn_0068d080(u32);
extern unsigned long long CM_THISCALL cm_fn_0068d0d0(u32, u32, s16);
extern void CM_CDECL cm_fn_00945333(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_CDECL cm_fn_004c0520(u32 list)
{
    u8 scratch[0x20c];
    u32 line;
    u8 *name = scratch + 0xc;
    u8 *extension = scratch + 0x10c;
    if (!list) {
        line = 0x2782;
        name = scratch + 0x10c;
        extension = scratch + 0xc;
        goto error;
    }
    s32 count = (s16)W(list);
    if (count == 0) { line = 0x278d; goto error; }
    u32 buffer = (u32)cm_fn_00944e46((u32)count * 79u);
    D((u32)scratch + 8) = buffer;
    if (!buffer) { line = 0x2796; goto error; }
    u32 head = D(list + 2);
    D(list + 0xa) = head;
    if (count > 0) {
        D((u32)scratch) = buffer;
        D((u32)scratch + 4) = (u32)count;
        u32 remaining;
        do {
            u32 source = cm_fn_0068d1b0(list);
            u32 destination = D((u32)scratch);
            remaining = D((u32)scratch + 4);
            for (u32 i = 0; i < 19; ++i, source += 4, destination += 4) {
                u32 value = D(source);
                D(destination) = value;
            }
            u32 next = D((u32)scratch) + 79;
            u16 word = W(source);
            W(destination) = word;
            --remaining;
            u8 byte = B(source + 2);
            B(destination + 2) = byte;
            D((u32)scratch) = next;
            D((u32)scratch + 4) = remaining;
        } while (remaining);
        buffer = D((u32)scratch + 8);
    }
    cm_fn_00945333(buffer, (u32)count, 79, 0x004c06c0);
    cm_fn_0068d080(list);
    if (count > 0) {
        u32 source = buffer;
        do {
            cm_fn_0068d0d0(list, source, 79);
            source += 79;
            --count;
        } while (count);
    }
    cm_fn_009452ca(buffer);
    return 1;
error:
    cm_fn_00944cff(0x0099ec20, 0, 0, name, extension);
    u32 text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
