#include "cmre.h"

/* Preferred-image VA 0x00522e00, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Thiscall, fifteen stack arguments, returns self even after reporting errors.
   Schedule copies retain the original forward DWORD-then-byte access order. */
extern u32 CM_FASTCALL cm_fn_00518640(u32);
extern s32 CM_FASTCALL cm_fn_00518790(u32);
extern s32 CM_FASTCALL cm_fn_005241e0(u32);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern void *CM_CDECL cm_fn_00945501(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);

#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_THISCALL cm_fn_00522e00(u32 self, u32 owner, s16 count, u32 clubs,
                             s16 rounds, u32 competition, u32 schedule,
                             u16 year, u8 flag_a, u8 flag_b, u16 round_flag,
                             u8 flag_c, u32 builder, u32 numbered,
                             u32 value_38, u32 club_flags)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x20c];
    u8 *filename, *extension;
    u32 bytes, destination, words, tail, value, base, index, offset, limit, line;
    s32 built;
    char *version;

    frame.state = 0xffffffffu;
    frame.handler = 0x0095783bu;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_00518640(self);
    D(self + 4u) = competition;
    W(self + 0x40u) = year;
    B(self + 0x42u) = flag_b;
    B(self + 0x44u) = flag_a;
    D(self + 0x30u) = 0xffffffffu;
    D(self + 0x2cu) = 0xffffffffu;
    B(self + 0x51u) = flag_c;
    D(self) = 0x00969468u;
    B(self + 0x43u) = 1;
    B(self + 0xb1u) = 0;
    B(self + 0xabu) = 0;
    B(self + 0xacu) = 0;
    W(self + 0x3cu) = (u16)rounds;
    W(self + 0x3au) = round_flag;
    value = B(owner + 0x49u);
    frame.state = 0;
    B(self + 0x49u) = (u8)value;
    value = B(owner + 0x4au);
    B(self + 0x4au) = (u8)value;
    bytes = (u32)(s32)rounds * 104u;
    D(self + 0x28u) = value_38;
    destination = (u32)cm_fn_00944e46(bytes);
    D(self + 0xa3u) = destination;
    if (destination == 0) {
        filename = scratch + 0x10c; extension = scratch + 12; line = 0x48;
        goto error;
    }
    words = bytes >> 2;
    tail = bytes & 3u;
    while (words != 0) {
        value = D(schedule);
        D(destination) = value;
        schedule += 4u; destination += 4u; --words;
    }
    while (tail != 0) {
        value = B(schedule);
        B(destination) = (u8)value;
        ++schedule; ++destination; --tail;
    }
    W(self + 0x3eu) = (u16)count;
    destination = (u32)cm_fn_00945501((u32)(s32)count, 6);
    D(self + 0xa7u) = destination;
    if (destination == 0) {
        filename = scratch + 12; extension = scratch + 0x10c; line = 0x53;
        goto error;
    }
    limit = (u32)(s32)(s16)W(self + 0x3eu);
    index = 0;
    offset = 0;
    if ((s32)limit > 0) {
        do {
            base = D(self + 0xa7u);
            value = D(clubs);
            D(base + offset) = value;
            base = D(self + 0xa7u);
            B(base + offset + 5u) = 0;
            if (club_flags != 0) {
                base = D(self + 0xa7u);
                value = B(club_flags + index);
                B(base + offset + 4u) = (u8)value;
            } else {
                base = D(self + 0xa7u);
                B(base + offset + 4u) = numbered != 0 ? (u8)index : 0;
            }
            ++index;
            clubs += 4u;
            offset += 6u;
        } while ((s32)index < (s32)limit);
    }
    if (builder != 0) {
        built = cm_fn_005241e0(self);
        line = 0x62;
    } else {
        built = cm_fn_00518790(self);
        line = 0x6a;
    }
    if (built >= 0) goto done;
    filename = scratch + 12; extension = scratch + 0x10c;
error:
    cm_fn_00944cff(0x009b5944u, 0, 0, filename, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version,
                  filename, extension, line);
    cm_fn_005e8290((const char *)0x009870e0u,
                  (const char *)0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
done:
    CM_FS(u32, 0) = frame.prev;
    return self;
}
