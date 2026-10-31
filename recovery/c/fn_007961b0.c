/* WIP: memory page 0xae2000 mismatch, cases 65 and 125 (only mismatch; error-path harness limit). */
#include "cmre.h"

/* Original 3.9.68 VA 0x007961b0. Convention: cdecl (one stack argument, `ret`
   with no pops). The entry `push ecx` allocates a 4-byte scratch slot; the byte
   at [esp+0x13] is local_1 (its top byte), and the byte at [esp+0x18] is the low
   byte of the stack argument slot, used here as the flag.

   Globals: 0x00b77c7a signed 16-bit index (times 0x300) into a table based at
   0x00b74c26; 0x00b7433c dword passed as ECX to the 0x00795xxx helpers and
   re-read at each call site; 0x00b74450 is the object passed as ECX to the
   0x007ff270 and 0x007fe600 calls.

   Record fields by byte offset from the record pointer returned by 0x00795750:
   +0x04 passed to 0x00795a10, +0x103 and +0x107 dwords, +0x10b pointer,
   +0x113 byte. */

extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 param_2, u32 param_3, u32 param_4,
                                       u32 param_5, u32 param_6);
extern int *CM_THISCALL cm_fn_00795750(int param_1, int *param_2, int *param_3);
extern int CM_THISCALL cm_fn_007959e0(int param_1, int *param_2);
extern void CM_THISCALL cm_fn_00795a10(int param_1, int *param_2, char *param_3);
extern void CM_THISCALL cm_fn_00795a60(int param_1, int *param_2);
extern int CM_THISCALL cm_fn_0054cdb0(short *param_1, u32 param_2, u32 param_3);
extern void CM_THISCALL cm_fn_007ff270(int param_1, int param_2, char *param_3, int param_4);

void CM_CDECL cm_fn_007961b0(u32 *param_1)
{
    u32 rec;     /* esi: result of 0x00795750, then the record pointer */
    u32 table;   /* edi: table entry, then the result of 0x007959e0 */
    u32 found;   /* bl: 1 when a record was found, else 0 */
    u32 ok;      /* eax: result of 0x0054cdb0 */
    u32 edx;     /* dword read through the argument */
    s8 flag;     /* low byte of the argument slot ([esp+0x18]) */
    s8 local_1;  /* scratch byte ([esp+0x13]) */
    s16 index;

    if (cm_fn_007fe600(0x00b74450u, 0x00796340u, 0x00796b30u, 1u, 0x005a8f60u, 0u) == 0) {
        return;
    }

    index = CM_GLOBAL(s16, 0x00b77c7a);
    table = CM_GLOBAL(u32, 0x00b74c26u + (u32)(s32)index * 0x300u);
    rec = (u32)cm_fn_00795750(CM_GLOBAL(u32, 0x00b7433c), (int *)table, (int *)param_1);

    if (rec == 0) {
        found = 0;
        cm_fn_00795a60(CM_GLOBAL(u32, 0x00b7433c), (int *)table);
        flag = 0;
        local_1 = 0;
    } else {
        found = 1;
        cm_fn_00795a10(CM_GLOBAL(u32, 0x00b7433c), (int *)table, (char *)(rec + 4u));
        {
            u32 w107 = *(u32 *)(rec + 0x107u);
            u32 w103 = *(u32 *)(rec + 0x103u);
            ok = (u32)cm_fn_0054cdb0((short *)(rec + 0x10bu), w103, w107);
        }
        flag = (s8)((ok == 7u || ok == 0x1cu) ? -1 : 0);
        local_1 = (s8)*(u8 *)(rec + 0x113u);
    }

    table = (u32)cm_fn_007959e0(CM_GLOBAL(u32, 0x00b7433c), (int *)table);
    edx = *param_1;

    cm_fn_007ff270((int)0x00b74450u, 0, (char *)edx, 0);
    cm_fn_007ff270((int)0x00b74450u, 1, (char *)0x00ae2c90u, 0);
    cm_fn_007ff270((int)0x00b74450u, 2, (char *)(u32)(s32)flag, 0);
    cm_fn_007ff270((int)0x00b74450u, 3, (char *)(u32)(s32)local_1, 0);
    cm_fn_007ff270((int)0x00b74450u, 4, (char *)(u32)(s32)(s8)found, 0);
    cm_fn_007ff270((int)0x00b74450u, 5, (char *)table, 0);

    if (rec == 0) {
        cm_fn_007ff270((int)0x00b74450u, 6, (char *)0, 0);
        cm_fn_007ff270((int)0x00b74450u, 7, (char *)0, 0);
    } else {
        cm_fn_007ff270((int)0x00b74450u, 6, (char *)(rec + 0x10bu), 0);
        rec = rec + 0x103u;
        cm_fn_007ff270((int)0x00b74450u, 7, (char *)rec, 0);
    }

    cm_fn_007ff270((int)0x00b74450u, 8, (char *)(u32)(s32)flag, 0);
}
