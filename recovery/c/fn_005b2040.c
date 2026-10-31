#include "cmre.h"

extern u32 *CM_CDECL cm_fn_0090bde0(u32 param_1, u32 param_2);
extern u32 CM_CDECL cm_fn_005b2170(void);
extern char *CM_CDECL cm_fn_0066f4e0(u32 a, u32 b, u32 c);
extern u32 CM_CDECL cm_fn_009466d0(u32 destination, u32 source, u32 count);

/* Cdecl with three stack arguments (arg_04 = selector, arg_08 = destination,
   arg_0c = count), `ret 0`, no return value. Field and global layout:
   0x00ae2c68 is a one-time flag; when it is zero the routine looks up one of
   two strings (0x9c3718 or 0x9b5348 with 0x9cc2c8) through 0x90bde0, copies
   0x104 bytes of the result to 0x00ae2a64 through 0x9466d0 (clearing 0x00ae2b67
   first) and calls 0x5b2170; a non-zero result sets the flag. If the lookup
   yields zero the 0x00ae2a64 byte is cleared instead. Selector -2 or -1 calls
   0x66f4e0 with (0xde1f64, 0x9cc318 or 0x9cc33c, 0xad9c64) (three pushed
   dwords, cleaned by the caller), then copies arg_0c bytes from 0xde1f64 to
   arg_08 and clears byte arg_08 + arg_0c - 1. Any other selector reads a
   record of 10 bytes at 0x00ae2c6c + 10 * selector (from a count at 0x00ae2c70,
   low word): when the selector is in range and the record is non-null the
   copy source is dword +4 (or +0 when that is zero), otherwise the destination
   byte at arg_08 is cleared alone. */
void CM_CDECL cm_fn_005b2040(u32 arg_04, u32 arg_08, u32 arg_0c)
{
    u32 p;
    u32 r;
    u32 src;
    u32 rec;
    u32 count;
    s32 sel;
    u32 table_base;
    u16 limit;

    if (*(volatile u32 *)0x00ae2c68 == 0) {
        p = (u32)cm_fn_0090bde0(0x009c3718, 0x009cc2c8);
        if (p == 0) {
            p = (u32)cm_fn_0090bde0(0x009b5348, 0x009cc2c8);
            if (p == 0) {
                *(volatile u8 *)0x00ae2a64 = 0;
                goto dispatch;
            }
        }
        cm_fn_009466d0(0x00ae2a64, p, 0x104);
        *(volatile u8 *)0x00ae2b67 = 0;
        r = cm_fn_005b2170();
        if (r != 0)
            *(volatile u32 *)0x00ae2c68 = 1;
    }

dispatch:
    sel = (s32)arg_04;
    if (sel == -2 || sel == -1) {
        if (sel == -2)
            cm_fn_0066f4e0(0x00de1f64, 0x009cc318, 0x00ad9c64);
        else
            cm_fn_0066f4e0(0x00de1f64, 0x009cc33c, 0x00ad9c64);
        count = arg_0c;
        cm_fn_009466d0(arg_08, 0x00de1f64, count);
        *(volatile u8 *)(arg_08 + count - 1u) = 0;
        return;
    }

    table_base = *(volatile u32 *)0x00ae2c6c;
    limit = (u16)*(volatile u32 *)0x00ae2c70;
    if (table_base != 0 && sel >= 0 && sel < (s32)limit) {
        count = arg_0c;
        rec = table_base + (u32)sel * 10u;
        src = *(volatile u32 *)(rec + 4);
        if (src == 0)
            src = *(volatile u32 *)rec;
        cm_fn_009466d0(arg_08, src, count);
        *(volatile u8 *)(arg_08 + count - 1u) = 0;
    } else {
        *(volatile u8 *)arg_08 = 0;
    }
}
