#include "cmre.h"

/* Original frame: sub esp, 0x204; push ebx, esi, edi. Frame offset 0 (esp+0x30
   after the error-path calls) holds the stack word whose low 16 bits become
   the length on that path; no original instruction writes it before the read.
   Scratch buffer A is [esp+0x10] (frame + 4). Scratch buffer B is [esp+0x110]
   (frame + 0x104). */

extern void cm_fn_005e8290(const char *message, const char *detail, int flags);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                  void *arg5);

u32 CM_THISCALL cm_fn_00567ff0(u32 self, u32 dest)
{
    u8 frame[0x204];
    u32 cur = *(volatile u32 *)(self + 4);
    u32 limit = *(volatile u32 *)(self + 8);
    u32 next = cur + 2;
    s32 len;

    if (next <= limit) {
        u32 base = *(volatile u32 *)self;
        len = (s16)*(volatile u16 *)(base + cur);
        *(volatile u32 *)(self + 4) = next;
    } else {
        (void)cm_fn_00944cff(0x009c3734u, 0, 0, frame + 0x104, frame + 4);
        /* Pushes 0x90, edx and eax are not consumed by cm_fn_0090d130 (void).
           They are the varargs of the following cm_fn_00944c9f call. */
        u32 ret_0090d130 = cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, ret_0090d130,
                             frame + 0x104, frame + 4, 0x90u);
        cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
        len = (s16)*(volatile u16 *)frame;
    }

    {
        /* Copy len bytes from object base + this->+4 to dest: len >> 2 dwords
           (rep movsd), then len & 3 bytes (rep movsb), forward in order. */
        u32 count = (u32)len;
        u32 dwords = count >> 2;
        u32 i;
        u32 src = *(volatile u32 *)self;
        src += *(volatile u32 *)(self + 4);
        for (i = 0; i < dwords; i++) {
            *(volatile u32 *)(dest + i * 4u) = *(volatile u32 *)(src + i * 4u);
        }
        for (i = 0; i < (count & 3u); i++) {
            *(volatile u8 *)(dest + dwords * 4u + i) =
                *(volatile u8 *)(src + dwords * 4u + i);
        }
        *(volatile u32 *)(self + 4) = *(volatile u32 *)(self + 4) + count;
    }

    /* EAX still holds the sign-extended length at the return. */
    return (u32)len;
}
