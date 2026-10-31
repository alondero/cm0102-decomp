/* WIP: stack-probe helper 0x00945610 changes caller ESP/return-slot layout; plain C regparm plus alloca(0) is not a faithful transport. */
#include "cmre.h"

typedef s32 (CM_STDCALL *cm_map_a_fn)(u32, u32, const char *, s32, char *, s32);
typedef s32 (CM_STDCALL *cm_map_w_fn)(u32, u32, const u16 *, s32, u16 *, s32);
typedef s32 (CM_STDCALL *cm_mbtwc_fn)(u32, u32, const char *, s32, u16 *, s32);
typedef s32 (CM_STDCALL *cm_wctmb_fn)(u32, u32, const u16 *, s32, char *, s32,
                                      const char *, s32 *);
extern s32 CM_CDECL cm_fn_009519d3(const char *text, s32 count);
extern void __attribute__((regparm(1))) cm_fn_00945610(u32 size);

s32 CM_CDECL cm_fn_0094c0fe(u32 locale, u32 flags, const char *source,
                             s32 count, void *destination, s32 capacity,
                             u32 code_page, s32 option)
{
    volatile struct { u32 prev; u32 handler; s32 state; } frame;
    frame.state = -1;
    frame.handler = 0x0094bf5c;
    u32 previous = CM_FS(u32, 0);
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;
    s32 result = 0;
    s32 mode = CM_GLOBAL(volatile s32, 0x00de303c);
    if (mode == 0) {
        s32 ok = ((cm_map_w_fn)CM_GLOBAL(volatile u32, 0x009670a4))(
            0, 0x100, (const u16 *)0x009715f8, 1, 0, 0);
        if (ok != 0) {
            CM_GLOBAL(volatile s32, 0x00de303c) = 1;
            mode = 1;
        } else {
            ok = ((cm_map_a_fn)CM_GLOBAL(volatile u32, 0x009670a8))(
                0, 0x100, (const char *)0x009715f4, 1, 0, 0);
            if (ok == 0) goto finish;
            CM_GLOBAL(volatile s32, 0x00de303c) = 2;
            mode = 2;
        }
    }
    if (count > 0) count = cm_fn_009519d3(source, count);
    if (mode == 2) {
        result = ((cm_map_a_fn)CM_GLOBAL(volatile u32, 0x009670a8))(
            locale, flags, source, count, (char *)destination, capacity);
        goto finish;
    }
    if (mode != 1) goto finish;
    if (code_page == 0) code_page = CM_GLOBAL(volatile u32, 0x00de3058);
    u32 convert_flags = option != 0 ? 9u : 1u;
    s32 needed = ((cm_mbtwc_fn)CM_GLOBAL(volatile u32, 0x009670ac))(
        code_page, convert_flags, source, count, 0, 0);
    if (needed == 0) goto finish;

    u32 size = ((u32)needed * 2u + 3u) & ~3u;
    frame.state = 0;
    cm_fn_00945610(size);
    volatile u8 *wide_storage = (volatile u8 *)__builtin_alloca(0);
    frame.state = -1;
    volatile u16 *wide = (volatile u16 *)wide_storage;
    s32 converted = ((cm_mbtwc_fn)CM_GLOBAL(volatile u32, 0x009670ac))(
        code_page, 1, source, count, (u16 *)wide, needed);
    if (converted == 0) goto finish;
    s32 mapped = ((cm_map_w_fn)CM_GLOBAL(volatile u32, 0x009670a4))(
        locale, flags, (const u16 *)wide, needed, 0, 0);
    if (mapped == 0) goto finish;

    if ((flags & 0x400u) == 0) {
        size = ((u32)mapped * 2u + 3u) & ~3u;
        frame.state = 1;
        cm_fn_00945610(size);
        volatile u16 *mapped_storage = (volatile u16 *)__builtin_alloca(0);
        frame.state = -1;
        s32 remapped = ((cm_map_w_fn)CM_GLOBAL(volatile u32, 0x009670a4))(
            locale, flags, (const u16 *)wide, needed, (u16 *)mapped_storage, mapped);
        if (remapped == 0) goto finish;
        if (capacity == 0) destination = 0;
        result = ((cm_wctmb_fn)CM_GLOBAL(volatile u32, 0x009670d8))(
            code_page, 0x220, (const u16 *)mapped_storage, mapped,
            (char *)destination, capacity, 0, 0);
        if (result != 0) result = mapped;
    } else {
        if (capacity == 0) {
            result = mapped;
            goto finish;
        }
        if (capacity < mapped) goto finish;
        result = ((cm_map_w_fn)CM_GLOBAL(volatile u32, 0x009670a4))(
            locale, flags, (const u16 *)wide, needed,
            (u16 *)destination, capacity);
        if (result != 0) result = mapped;
    }
finish:
    CM_FS(u32, 0) = frame.prev;
    return result;
}
