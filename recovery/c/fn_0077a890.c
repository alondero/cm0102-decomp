#include "cmre.h"

/* Object fields by byte offset from the object pointer in ECX:
   0x00000 dword read after the second 0x008ae2a0 call; when non-zero it is
           passed to 0x00944c94 (cdecl, caller cleans up);
   0x0c3a5e dword tested before the string block; zero skips the block;
   0x0c3c60 50-byte record (12 dwords then one word) appended after the string
           copy in the scratch text buffer. */
#define OBJ_U32(off) (*(volatile u32 *)(self + (off)))
#define OBJ_BYTES(off) ((u8 *)(self + (off)))

extern void CM_THISCALL cm_fn_008ae2a0(u32 self);
extern int CM_THISCALL cm_fn_008aec90(u32 self, u32 param_2, s32 param_3, u32 param_4, u32 param_5, s32 param_6, u32 param_7);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32 arg4);

/* Scratch allocation: the ECX copy at offset 0 (stored, never read) and the
   2000-byte text buffer at offset 4. The text buffer's address is passed to
   0x008aec90 as its first stack argument. */
struct scratch_block {
    u32 slot_0;
    u8 text[2000];
};

/* repne scasb over the NUL-terminated string at 0x00a64980 with ECX=-1 and AL=0.
   Returns the ECX left behind; the string length is ~ECX - 1. */
static u32 scan_string_ecx(void)
{
    u32 count = 0xffffffffu;
    u8 *p = &CM_GLOBAL(u8, 0x00a64980);
    u8 c;

    do {
        if (count == 0) {
            break;
        }
        count = count - 1u;
        c = *p;
        p = p + 1;
    } while (c != 0);
    return count;
}

void CM_THISCALL cm_fn_0077a890(u32 self)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    struct scratch_block scratch;
    u8 *src;
    u8 *dst;
    u32 count;
    u32 length;
    u32 n;
    u32 flag;
    u32 field0;

    seh.state = -1;
    seh.handler = 0x0095e08bu;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    scratch.slot_0 = self;
    flag = OBJ_U32(0x0c3a5e);
    seh.state = 0;
    if (flag != 0) {
        /* Copy the string: dwords first (length / 4), then the remaining bytes. */
        count = scan_string_ecx();
        length = ~count - 1u;
        src = &CM_GLOBAL(u8, 0x00a64980);
        dst = scratch.text;
        for (n = length >> 2; n != 0; n = n - 1u) {
            *(u32 *)dst = *(u32 *)src;
            src = src + 4;
            dst = dst + 4;
        }
        for (n = length & 3u; n != 0; n = n - 1u) {
            *dst = *src;
            src = src + 1;
            dst = dst + 1;
        }

        /* Append the 50-byte record from the object after the copied string. */
        count = scan_string_ecx();
        length = ~count - 1u;
        dst = scratch.text + length;
        src = OBJ_BYTES(0x0c3c60);
        for (n = 12u; n != 0; n = n - 1u) {
            *(u32 *)dst = *(volatile u32 *)src;
            src = src + 4;
            dst = dst + 4;
        }
        *(u16 *)dst = *(volatile u16 *)src;

        /* Third scan: ECX = length + 0x64 is the second stack argument. */
        count = scan_string_ecx();
        length = ~count + 0x63u;
        cm_fn_008aec90(self, (u32)scratch.text, (s32)length, 0x1a0bu, 0u, 0, 0u);
    }

    cm_fn_008ae2a0(self);
    seh.state = -1;
    cm_fn_008ae2a0(self);

    field0 = OBJ_U32(0);
    if (field0 != 0) {
        cm_fn_00944c94(field0);
    }

    CM_FS(u32, 0) = seh.prev;
}
