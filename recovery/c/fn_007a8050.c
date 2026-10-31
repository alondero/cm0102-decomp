#include "cmre.h"

/* Stack arguments: arg1 is the record pointer S (ESI); arg2 is the output
   pointer OUT (read from the stack after the pushes at 0x8134).

   Record fields by byte offset from S: 0x00 dword copied to OUT+0x00,
   0x24 and 0x39 dwords tested for zero (0x39 is also a pointer whose target
   dword is copied to OUT+0x04), 0x40 unsigned word, 0x3e signed word,
   0x61 pointer (signed word at +0x0b is passed to 0x0090cfc0), 0x69 pointer
   (signed word at +0x0a is compared with 0x32).
   Output fields by byte offset from OUT: 0x00 dword, 0x04 dword,
   0x08/0x10 words from the global at 0x00ae2c90, 0x0a/0x12 words from the
   global at 0x00ae2c92, 0x18 byte 0x10, 0x19 byte 0.
   The original returns through AL only, so the declared width is u8. */
#define AT_U8(p, off) (*(volatile u8 *)((p) + (off)))
#define AT_U16(p, off) (*(volatile u16 *)((p) + (off)))
#define AT_U32(p, off) (*(volatile u32 *)((p) + (off)))
#define GLOB_U16(va) CM_GLOBAL(volatile u16, va)
#define GLOB_U32(va) CM_GLOBAL(volatile u32, va)

extern u8 CM_THISCALL cm_fn_00842080(u32 self, u32 arg2, u32 arg3);
extern int CM_CDECL cm_fn_0090cfc0(int param_1);

u8 CM_STDCALL cm_fn_007a8050(u32 arg1, u32 arg2)
{
    u32 s = arg1;
    u32 out = arg2;
    u8 flag = 0;     /* the original BL */
    u8 r;
    u32 rec;
    u32 v;
    u32 x40;
    u32 t;
    s32 g1;
    s32 x3e;
    s32 q;
    u32 ptr;
    int go_success = 0;

    r = cm_fn_00842080(AT_U32(GLOB_U32(0x00de1f54), 0x856), s, 0);
    if (r != 0) {
        return 0;
    }

    rec = AT_U32(s, 0x69);
    if (rec != 0) {
        if (AT_U32(s, 0x39) != 0) {
            return 0;
        }
        if (AT_U32(s, 0x24) != 0) {
            return 0;
        }
        if ((s16)AT_U16(rec, 0x0a) < 0x32) {
            if (cm_fn_0090cfc0(5) == 0) {
                go_success = 1;
            }
        }
    } else {
        if (AT_U32(s, 0x61) == 0) {
            flag = 1;
        }
        if (AT_U32(s, 0x61) != 0) {
            r = cm_fn_00842080(AT_U32(GLOB_U32(0x00de1f54), 0x856), s, 0);
            if (r != 0) {
                return 0;
            }
            if (AT_U32(s, 0x39) == 0 && AT_U32(s, 0x24) == 0) {
                x40 = AT_U16(s, 0x40);
                t = GLOB_U16(0x00ae2c92) - x40;
                g1 = (s16)GLOB_U16(0x00ae2c90);
                x3e = (s16)AT_U16(s, 0x3e);
                t = t + (t * 9u) * 8u;               /* eax = 73 * eax */
                v = (t * 5u) - (u32)x3e + (u32)g1;
                if ((s32)v > 0x16d) {
                    u32 e = AT_U32(s, 0x61);
                    q = cm_fn_0090cfc0((s16)AT_U16(e, 0x0b));
                    if (q < 0x2ee) {
                        go_success = 1;
                    }
                }
            }
        }
    }

    if (!go_success && flag == 0) {
        return 0;
    }

    /* 0x00008134: success path. */
    AT_U32(out, 0x00) = AT_U32(s, 0x00);
    AT_U8(out, 0x18) = 0x10;
    AT_U8(out, 0x19) = 0;
    ptr = AT_U32(s, 0x39);
    if (ptr != 0) {
        AT_U32(out, 0x04) = AT_U32(ptr, 0x00);
    } else {
        AT_U32(out, 0x04) = 0xffffffffu;
    }
    AT_U16(out, 0x08) = GLOB_U16(0x00ae2c90);
    AT_U16(out, 0x0a) = GLOB_U16(0x00ae2c92);
    AT_U16(out, 0x10) = GLOB_U16(0x00ae2c90);
    AT_U16(out, 0x12) = GLOB_U16(0x00ae2c92);
    return 1;
}
