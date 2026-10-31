#include "cmre.h"

/* Object fields by byte offset from the object pointer in ECX:
   0x04 signed 16-bit value passed to listen as the backlog,
   0x4b6 32-bit flag; zero returns 0 before any socket call.
   Scratch frame: frame[k] is the original stack byte at (entry esp - 0x2c + k)
   for the locals the listing addresses; bytes 0x00..0x03 hold the setsockopt
   flag/timeout value, 0x04..0x07 the linger value, 0x08..0x0b the ioctl argp,
   0x0c..0x18 the four parsed address bytes, 0x1c..0x2b one sockaddr (16 bytes). */

/* Winsock entries are stdcall: the callee pops its own arguments (no add esp
   follows any call in this routine). */
extern u32 CM_STDCALL cm_fn_00944c8e(s32 af, s32 type, s32 protocol);
extern s32 CM_STDCALL cm_fn_00944c88(u32 s, s32 level, s32 optname, u32 optval, s32 optlen);
extern s32 CM_STDCALL cm_fn_00944c3a(u32 s);
extern s32 CM_STDCALL cm_fn_00944c7c(u32 s, u32 cmd, u32 argp);
extern s32 CM_STDCALL cm_fn_00944c76(u32 s, u32 name, s32 namelen);
extern s32 CM_STDCALL cm_fn_00944c82(u32 s, u32 name, s32 namelen);
extern s32 CM_STDCALL cm_fn_00944c70(u32 s, s32 backlog);
extern u16 CM_STDCALL cm_fn_00944c64(u32 hostshort);
/* cdecl variadic sscanf-like: source, format, then four destination pointers. */
extern void CM_CDECL cm_fn_00946fd8(u32 destination, u32 format, ...);

u32 CM_THISCALL cm_fn_008af290(u32 self)
{
    u8 frame[0x2c];
    u32 s;
    u32 s2;
    s32 result;
    u16 port;

    frame[0] = 1;
    frame[1] = 0;
    frame[2] = 0;
    frame[3] = 0;
    if (*(volatile s32 *)(self + 0x4b6u) == 0) {
        return 0;
    }

    s = cm_fn_00944c8e(2, 1, 0);
    if (s == 0xffffffffu) {
        return 0;
    }
    result = cm_fn_00944c88(s, 0xffff, 4, (u32)&frame[0], 4);
    if (result == -1) {
        cm_fn_00944c3a(s);
        return 0;
    }

    frame[8] = 1;
    frame[9] = 0;
    frame[10] = 0;
    frame[11] = 0;
    result = cm_fn_00944c7c(s, 0x8004667eu, (u32)&frame[8]);
    if (result == -1) {
        cm_fn_00944c3a(s);
        return 0;
    }

    frame[4] = 0;
    frame[5] = 0;
    frame[6] = 0;
    frame[7] = 0;
    result = cm_fn_00944c88(s, 0xffff, 0x80, (u32)&frame[4], 4);
    if (result == -1) {
        cm_fn_00944c3a(s);
        return 0;
    }

    cm_fn_00944c88(s, 0xffff, 8, (u32)&frame[0], 4);
    cm_fn_00944c88(s, 0xffff, 1, (u32)&frame[0], 4);

    frame[0] = 0x98;
    frame[1] = 0x3a;
    frame[2] = 0;
    frame[3] = 0;
    cm_fn_00944c88(s, 0xffff, 0x1001, (u32)&frame[0], 4);
    cm_fn_00944c88(s, 0xffff, 0x1002, (u32)&frame[0], 4);

    cm_fn_00946fd8(0x00a8c6fcu, 0x00a695e4u,
                   (u32)&frame[0x0c], (u32)&frame[0x10],
                   (u32)&frame[0x14], (u32)&frame[0x18]);
    frame[0x20] = frame[0x0c];
    frame[0x21] = frame[0x10];
    frame[0x22] = frame[0x14];
    frame[0x23] = frame[0x18];
    frame[0x1c] = 2;
    frame[0x1d] = 0;
    port = cm_fn_00944c64(0x1a0bu);
    frame[0x1e] = (u8)port;
    frame[0x1f] = (u8)(port >> 8);

    result = cm_fn_00944c76(s, (u32)&frame[0x1c], 0x10);
    if (result == -1) {
        cm_fn_00944c3a(s);
        return 0;
    }

    result = cm_fn_00944c70(s, (s32)*(volatile s16 *)(self + 4u));
    if (result == -1) {
        cm_fn_00944c3a(s);
        return 0;
    }

    s2 = cm_fn_00944c8e(2, 1, 0);
    if (s2 == 0xffffffffu) {
        cm_fn_00944c3a(s);
        return 0;
    }

    result = cm_fn_00944c88(s2, 0xffff, 4, (u32)&frame[0], 4);
    if (result == -1) {
        cm_fn_00944c3a(s);
        cm_fn_00944c3a(s2);
        return 0;
    }

    cm_fn_00946fd8(0x00a8c6fcu, 0x00a695e4u,
                   (u32)&frame[0x18], (u32)&frame[0x14],
                   (u32)&frame[0x10], (u32)&frame[0x0c]);
    frame[0x20] = frame[0x18];
    frame[0x21] = frame[0x14];
    frame[0x22] = frame[0x10];
    frame[0x23] = frame[0x0c];
    frame[0x1c] = 2;
    frame[0x1d] = 0;
    port = cm_fn_00944c64(0x1a0bu);
    frame[0x1e] = (u8)port;
    frame[0x1f] = (u8)(port >> 8);

    result = cm_fn_00944c82(s2, (u32)&frame[0x1c], 0x10);
    if (result == -1) {
        cm_fn_00944c3a(s);
        cm_fn_00944c3a(s2);
        return 0;
    }

    cm_fn_00944c3a(s);
    cm_fn_00944c3a(s2);
    return 1;
}
