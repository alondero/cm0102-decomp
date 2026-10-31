#include "cmre.h"

/* Stream fields by byte offset from the stream pointer (the second argument):
   0x00 cursor pointer, 0x04 counter (incremented on success), 0x08 limit pointer
   (when zero, 0x0094ec37 is called first), 0x0c flags dword. Success clears
   flag 0x10 and sets flag 0x01 in the low byte of the flags dword. Returns -1 on
   failure, otherwise the low byte of the first argument. */
#define STREAM_U32(s, off) (*(volatile u32 *)((s) + (off)))
#define STREAM_U8(s, off) (*(volatile u8 *)((s) + (off)))

extern u32 CM_CDECL cm_fn_0094ec37(u32 stream);

s32 CM_CDECL cm_fn_0094e3c4(u32 c, u32 stream)
{
    u32 flags;
    u32 ptr;

    if (c == 0xffffffffu) {
        return -1;
    }
    flags = STREAM_U32(stream, 0x0c);
    if ((flags & 1u) == 0u) {
        if ((flags & 0x80u) == 0u) {
            return -1;
        }
        if ((flags & 2u) != 0u) {
            return -1;
        }
    }
    if (STREAM_U32(stream, 0x08) == 0u) {
        cm_fn_0094ec37(stream);
    }
    ptr = STREAM_U32(stream, 0x00);
    if (ptr == STREAM_U32(stream, 0x08)) {
        if (STREAM_U32(stream, 0x04) != 0u) {
            return -1;
        }
        ptr = ptr + 1u;
        STREAM_U32(stream, 0x00) = ptr;
    }
    if ((STREAM_U8(stream, 0x0c) & 0x40u) == 0u) {
        ptr = STREAM_U32(stream, 0x00) - 1u;
        STREAM_U32(stream, 0x00) = ptr;
        ptr = STREAM_U32(stream, 0x00);
        STREAM_U8(ptr, 0x00) = (u8)c;
    } else {
        ptr = STREAM_U32(stream, 0x00) - 1u;
        STREAM_U32(stream, 0x00) = ptr;
        ptr = STREAM_U32(stream, 0x00);
        if (STREAM_U8(ptr, 0x00) != (u8)c) {
            ptr = ptr + 1u;
            STREAM_U32(stream, 0x00) = ptr;
            return -1;
        }
    }
    flags = STREAM_U32(stream, 0x0c);
    STREAM_U32(stream, 0x04) = STREAM_U32(stream, 0x04) + 1u;
    flags = (flags & 0xffffff00u) | ((flags & 0xefu) | 1u);
    STREAM_U32(stream, 0x0c) = flags;
    return (s32)(c & 0xffu);
}
