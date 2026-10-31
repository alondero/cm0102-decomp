#include "cmre.h"

/* Plain cdecl routine (ret 0). Stack arguments: arg1 is the object pointer
   (called "obj" below), arg2 is a pointer P whose block header sits at P-0x10,
   arg3 is the value S.

   Object fields by byte offset from obj: 0x04 signed count, 0x08 signed cap,
   0x0c array base, 0x10 jump buffer passed to the error report, 0x9c marker dword.
   Header fields by byte offset from base = P-0x10: 0x00 magic dword,
   0x04 length, 0x08 index dword.
   Result fields by byte offset from the 0x00945667 result: 0x00 magic dword,
   0x04 dword S, 0x10 + S byte 0x5a, 0x11 + S byte 0xf0, returned at +0x10.

   0x0094791c is _longjmp(obj+0x10, code). It does not return: the original
   leaves its arguments on the stack and the instructions after each call reuse
   registers the call does not preserve, so that code is never reached. */
#define U32_AT(p, off) (*(volatile u32 *)((p) + (off)))
#define U8_AT(p, off) (*(volatile u8 *)((p) + (off)))
#define REPORT(obj, code) \
    do { \
        U32_AT((obj), 0x9c) = 0x5500aaffu; \
        cm_fn_0094791c((obj) + 0x10u, (code)); \
    } while (0)

extern unsigned long long CM_CDECL cm_fn_00945667(u32 arg8, u32 argC);
extern void CM_CDECL cm_fn_0094791c(u32 buffer, u32 value) __attribute__((noreturn));

u32 CM_CDECL cm_fn_00944480(u32 arg1, u32 arg2, u32 arg3)
{
    u32 obj = arg1;
    u32 base;
    u32 len;
    u32 count;
    u32 arr;
    u32 index;
    u32 result;

    if (arg2 == 0) {
        return 0;
    }

    base = arg2 - 0x10u;
    if (U32_AT(base, 0) != 0xaa53c5aau) {
        REPORT(obj, 0x2719u);
    }

    len = U32_AT(base, 4);
    if (U8_AT(base, len + 0x10u) != 0x5au) {
        REPORT(obj, 0x2719u);
    }
    if (U8_AT(base, len + 0x11u) != 0xf0u) {
        REPORT(obj, 0x2719u);
    }

    count = U32_AT(obj, 0x04);
    arr = U32_AT(obj, 0x0c);
    if ((s32)count <= 0 || (s32)count > (s32)U32_AT(obj, 0x08)) {
        REPORT(obj, 0x271du);
    }

    index = U32_AT(base, 0x08);
    result = (u32)cm_fn_00945667(base, arg3 + 0x12u);
    U32_AT(arr, index * 4u) = result;

    if (result == 0) {
        REPORT(obj, 0x271eu);
    }
    if (U32_AT(result, 0) != 0xaa53c5aau) {
        REPORT(obj, 0x2719u);
    }

    U32_AT(result, 0x04) = arg3;
    U8_AT(result, arg3 + 0x10u) = 0x5au;
    U8_AT(result, arg3 + 0x11u) = 0xf0u;

    if ((s32)index >= (s32)U32_AT(obj, 0x08)) {
        REPORT(obj, 0x271fu);
    }
    return result + 0x10u;
}
