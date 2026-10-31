#include "cmre.h"

extern s32 CM_CDECL cm_fn_0094fe81(u32 path, u32 open_flags, u32 share_flags,
    u32 mode);

volatile u32 *CM_CDECL cm_fn_0094b070(u32 path, volatile s8 *mode_text,
    u32 share_arg, volatile u32 *out)
{
    u32 share = CM_GLOBAL(volatile u32, 0x00de3290u);
    u32 open_flags;
    s8 mode = mode_text[0];
    if (mode == 'a') {
        open_flags = 0x109;
        share |= 2;
    } else if (mode == 'r') {
        open_flags = 0;
        share |= 1;
    } else if (mode == 'w') {
        open_flags = 0x301;
        share |= 2;
    } else {
        return 0;
    }

    s32 allow = 1;
    s32 once_rs = 0;
    s32 once_co = 0;
    u32 cursor = 0;
    for (;;) {
        s8 ch = mode_text[cursor + 1];
        cursor++;
        if (ch == 0 || allow == 0) break;

        if (ch == 'T') {
            if ((open_flags & 0x1000u) != 0) allow = 0;
            else open_flags |= 0x1000u;
        } else if (ch == '+') {
            if ((open_flags & 2u) != 0) allow = 0;
            else {
                open_flags &= 0xfffffffeu;
                share &= 0xfffffffcu;
                open_flags |= 2;
                share |= 0x80;
            }
        } else if (ch == 'D') {
            if ((open_flags & 0x40u) != 0) allow = 0;
            else open_flags |= 0x40;
        } else if (ch == 'R' || ch == 'S') {
            if (once_rs != 0) allow = 0;
            else {
                once_rs = 1;
                if (ch == 'R') open_flags |= 0x10;
                else open_flags |= 0x20;
            }
        } else if (ch == 'b') {
            if ((open_flags & 0xc000u) != 0) allow = 0;
            else open_flags |= 0x8000;
        } else if (ch == 'c' || ch == 'n') {
            if (once_co != 0) allow = 0;
            else {
                once_co = 1;
                if (ch == 'c') share |= 0x4000;
                else share &= 0xffffbfffu;
            }
        } else if (ch == 't') {
            if ((open_flags & 0xc000u) != 0) allow = 0;
            else open_flags |= 0x4000;
        } else {
            allow = 0;
        }
    }

    s32 result = cm_fn_0094fe81(path, open_flags, share_arg, 0x1a4);
    if (result < 0) return 0;
    CM_GLOBAL(volatile u32, 0x00de3144u)++;
    out[3] = share;
    out[1] = 0;
    out[0] = 0;
    out[2] = 0;
    out[7] = 0;
    out[4] = (u32)result;
    return out;
}
