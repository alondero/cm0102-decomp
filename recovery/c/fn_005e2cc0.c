#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_009452ca(u32 pointer);

typedef int (CM_STDCALL *cm_close_cursor_fn)(u32 cursor);
typedef int (CM_STDCALL *cm_set_cursor_fn)(u32 cursor);
typedef int (CM_STDCALL *cm_show_window_fn)(u32 window, s32 command);
typedef int (CM_STDCALL *cm_destroy_window_fn)(u32 window);
typedef u32 (CM_STDCALL *cm_slot_with_arg_fn)(u32 object);

void CM_CDECL cm_fn_005e2cc0(void)
{
    u32 handle = CM_GLOBAL(volatile u32, 0x00aebe18);
    if (handle != 0) {
        cm_set_cursor_fn set_cursor =
            (cm_set_cursor_fn)CM_GLOBAL(volatile u32, 0x009671bc);
        (void)set_cursor(handle);
    }

    handle = CM_GLOBAL(volatile u32, 0x00aebe1c);
    cm_close_cursor_fn close_cursor =
        (cm_close_cursor_fn)CM_GLOBAL(volatile u32, 0x009671d0);
    CM_GLOBAL(volatile u32, 0x00aebe18) = 0;
    if (handle != 0)
        (void)close_cursor(handle);

    handle = CM_GLOBAL(volatile u32, 0x00aebe24);
    CM_GLOBAL(volatile u32, 0x00aebe1c) = 0;
    if (handle != 0)
        (void)close_cursor(handle);

    handle = CM_GLOBAL(volatile u32, 0x00aebe20);
    CM_GLOBAL(volatile u32, 0x00aebe24) = 0;
    if (handle != 0)
        (void)close_cursor(handle);

    handle = CM_GLOBAL(volatile u32, 0x00aebe2c);
    CM_GLOBAL(volatile u32, 0x00aebe20) = 0;
    if (handle != 0) {
        u32 vtable = *(volatile u32 *)handle;
        cm_slot_with_arg_fn slot = *(cm_slot_with_arg_fn volatile *)(vtable + 8);
        (void)slot(handle);
        CM_GLOBAL(volatile u32, 0x00aebe2c) = 0;
    }

    handle = CM_GLOBAL(volatile u32, 0x00ae3194);
    if (handle != 0) {
        u32 vtable = *(volatile u32 *)handle;
        cm_slot_with_arg_fn slot = *(cm_slot_with_arg_fn volatile *)(vtable + 0x4c);
        (void)slot(handle);
        handle = CM_GLOBAL(volatile u32, 0x00ae3194);
        vtable = *(volatile u32 *)handle;
        slot = *(cm_slot_with_arg_fn volatile *)(vtable + 8);
        (void)slot(handle);
        CM_GLOBAL(volatile u32, 0x00ae3194) = 0;
    }

    {
        u32 window = CM_GLOBAL(volatile u32, 0x00b67a2c);
        cm_show_window_fn show_window =
            (cm_show_window_fn)CM_GLOBAL(volatile u32, 0x00967204);
        (void)show_window(window, 0);
    }
    handle = CM_GLOBAL(volatile u32, 0x00b67a2c);
    if (handle != 0) {
        cm_destroy_window_fn destroy_window =
            (cm_destroy_window_fn)CM_GLOBAL(volatile u32, 0x009671cc);
        (void)destroy_window(handle);
        CM_GLOBAL(volatile u32, 0x00b67a2c) = 0;
    }

    {
        volatile u32 *cursor = (volatile u32 *)0x00ae3444;
        do {
            u32 pointer = *cursor;
            if (pointer != 0)
                (void)cm_fn_009452ca(pointer);
            cursor += 5;
        } while ((u32)cursor <= 0x00ae45b0u);
    }
    {
        volatile u32 *cursor = (volatile u32 *)0x00ae4848;
        do {
            u32 pointer = *cursor;
            if (pointer != 0)
                (void)cm_fn_009452ca(pointer);
            cursor += 5;
        } while ((u32)cursor <= 0x00ae59b4u);
    }
    {
        volatile u32 *cursor = (volatile u32 *)0x00ae5c4c;
        do {
            u32 pointer = *cursor;
            if (pointer != 0)
                (void)cm_fn_009452ca(pointer);
            cursor += 5;
        } while ((u32)cursor <= 0x00ae6db8u);
    }
    {
        volatile u32 *cursor = (volatile u32 *)0x00ae7050;
        do {
            u32 pointer = *cursor;
            if (pointer != 0)
                (void)cm_fn_009452ca(pointer);
            cursor += 5;
        } while ((u32)cursor <= 0x00ae81bcu);
    }
    {
        volatile u32 *cursor = (volatile u32 *)0x00ae8454;
        do {
            u32 pointer = *cursor;
            if (pointer != 0)
                (void)cm_fn_009452ca(pointer);
            cursor += 5;
        } while ((u32)cursor <= 0x00ae95c0u);
    }
    {
        volatile u32 *cursor = (volatile u32 *)0x00ae9858;
        do {
            u32 pointer = *cursor;
            if (pointer != 0)
                (void)cm_fn_009452ca(pointer);
            cursor += 5;
        } while ((u32)cursor <= 0x00aea9c4u);
    }
    {
        volatile u32 *cursor = (volatile u32 *)0x00aeac5c;
        do {
            u32 pointer = *cursor;
            if (pointer != 0)
                (void)cm_fn_009452ca(pointer);
            cursor += 5;
        } while ((u32)cursor <= 0x00aebdc8u);
    }
}
