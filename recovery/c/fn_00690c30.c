#include "cmre.h"

/* Stack arguments (stdcall, ret 0xc): arg1 is the request pointer X, arg2 is
   output A (one byte), arg3 is output B (one byte). No return value is set.

   X fields by byte offset: 0x57 pointer to a dword index. The index selects a
   record pointer from the table whose base is the dword at 0x00adadfc.
   Record fields: 0x08 dword passed as ECX to 0x004a4850 (with X as its stack
   argument); 0x19, 0x2d, 0x41, 0x55 dwords compared with 1 (signed); 0x25/0x29,
   0x39/0x3d, 0x4d/0x51, 0x61/0x65 dword pairs passed to 0x0054cdb0 as its two
   stack arguments (ECX is the address of the global at 0x00ae2c90); byte
   fields 0x19, 0x2d, 0x41, 0x55 are copied to B when the test passes.
   0x0054cdb0 returns an int; the caller pops its two stack arguments (ret 8).
   The brief's prototype shows one stack parameter; the listing pushes two. */
#define AT_U8(p, off) (*(volatile u8 *)((p) + (off)))
#define AT_U32(p, off) (*(volatile u32 *)((p) + (off)))
#define GLOB_U32(va) CM_GLOBAL(volatile u32, va)

extern int CM_THISCALL cm_fn_004a4850(u32 self, u32 arg);
extern int CM_THISCALL cm_fn_0054cdb0(u32 self, u32 arg2, u32 arg3);

void CM_STDCALL cm_fn_00690c30(u32 arg1, u32 arg2, u32 arg3)
{
    u32 x = arg1;
    u32 index_ptr;
    u32 index;
    u32 record;
    u32 cnt;
    u32 esi;
    u32 e25;
    u32 e29;
    u32 e39;
    u32 e3d;
    u32 e61;
    u32 e65;
    u32 e4d;
    u32 e51;
    u8 b;
    int ret;

    index_ptr = AT_U32(x, 0x57);
    if (index_ptr == 0) {
        goto fail;
    }
    index = AT_U32(index_ptr, 0x00);
    record = AT_U32(GLOB_U32(0x00adadfc), index * 4u);
    if (record == 0) {
        goto fail;
    }
    cnt = AT_U32(record, 0x08);
    if (cnt == 0) {
        goto fail;
    }

    esi = (u32)cm_fn_004a4850(cnt, x);
    if (esi == 0) {
        goto fail;
    }

    /* Block 1: dword at +0x19 compared with 1. */
    if ((s32)AT_U32(esi, 0x19) > 1) {
        e29 = AT_U32(esi, 0x29);
        e25 = AT_U32(esi, 0x25);
        ret = cm_fn_0054cdb0((u32)&CM_GLOBAL(s16, 0x00ae2c90), e25, e29);
        if (ret < 10) {
            AT_U8(arg2, 0x00) = 1;
            b = AT_U8(esi, 0x19);
            AT_U8(arg3, 0x00) = b;
            return;
        }
    }

    /* Block 2: dword at +0x2d compared with 1. */
    if ((s32)AT_U32(esi, 0x2d) > 1) {
        e3d = AT_U32(esi, 0x3d);
        e39 = AT_U32(esi, 0x39);
        ret = cm_fn_0054cdb0((u32)&CM_GLOBAL(s16, 0x00ae2c90), e39, e3d);
        if (ret < 10) {
            AT_U8(arg2, 0x00) = 3;
            b = AT_U8(esi, 0x2d);
            AT_U8(arg3, 0x00) = b;
            return;
        }
    }

    /* Block 3: dword at +0x55 compared with 1. */
    if ((s32)AT_U32(esi, 0x55) > 1) {
        e65 = AT_U32(esi, 0x65);
        e61 = AT_U32(esi, 0x61);
        ret = cm_fn_0054cdb0((u32)&CM_GLOBAL(s16, 0x00ae2c90), e61, e65);
        if (ret < 10) {
            AT_U8(arg2, 0x00) = 4;
            b = AT_U8(esi, 0x55);
            AT_U8(arg3, 0x00) = b;
            return;
        }
    }

    /* Block 4: dword at +0x41 compared with 1. */
    if ((s32)AT_U32(esi, 0x41) > 1) {
        e51 = AT_U32(esi, 0x51);
        e4d = AT_U32(esi, 0x4d);
        ret = cm_fn_0054cdb0((u32)&CM_GLOBAL(s16, 0x00ae2c90), e4d, e51);
        if (ret < 10) {
            AT_U8(arg2, 0x00) = 2;
            b = AT_U8(esi, 0x41);
            AT_U8(arg3, 0x00) = b;
            return;
        }
    }

    AT_U8(arg2, 0x00) = 5;
    AT_U8(arg3, 0x00) = 0;
    return;

fail:
    AT_U8(arg2, 0x00) = 5;
    AT_U8(arg3, 0x00) = 0;
}
