#include "cmre.h"

/* Preferred-image VA 00685d30, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Original callees remain boundaries. 685f60 writes WORD outputs; preserve
   the later DWORD reads without initializing their unwritten upper halves.
   49f570 selects one DWORD at 4a03ea/3f5 and consumes BX only. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_THISCALL cm_fn_00417c10(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0049f570(u32, u32, u32, u32, u32);
extern void CM_THISCALL cm_fn_00685f60(u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

static void diagnostic(u32 name, u32 extension, u32 line)
{
    u32 version;
    cm_fn_00944cff(0x009f15f4, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, name, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
}

u32 CM_THISCALL cm_fn_00685d30(u32 self, u32 record, volatile u32 option)
{
    u8 scratch[0x210];
    u32 local = (u32)scratch, table, child, vtable, target, count;
    u32 object, competition, key, out_a, out_b, option_value;
    u8 selector, flag;
    s32 round, index;
    if (record == 0) {
        diagnostic(local + 0x110, local + 0x10, 0xc7f);
        return 0xffffffffu;
    }
    cm_fn_00417c10(self + 0xc9, local + 8, 0x00ae2c90);
    selector = B(self + 0x44);
    if (selector == 0xff) {
        flag = B(record + 0x42);
        if (flag != 0xff) {
            round = (s32)D(self + 0x30);
            index = (s32)(s8)flag;
            if (index <= round && index >= 0) {
                table = D(self + 0xc);
                child = D(table + (u32)index * 4);
                option_value = option;
                vtable = D(child);
                target = D(vtable + 4);
                return ((u32 (CM_THISCALL *)(u32,u32,u32))target)
                    (child, record, option_value);
            }
            diagnostic(local + 0x10, local + 0x110, 0xc8e);
            diagnostic(local + 0x10, local + 0x110, 0xc8f);
            return 0;
        }
    }
    flag = B(record + 0x42);
    if (selector != flag && selector != 0xfe) return 0;
    /* 417c10 restores incoming ECX; MOV CL then MOV CX replace its low bits. */
    count = ((self + 0xc9) & 0xffff0000u) | W(self + 0x3e);
    table = D(self + 0xb1);
    cm_fn_00685f60(self, record, table, count, 1, local + 4, local);
    if (B(self + 0x44) == 0xfe) return 1;
    table = D(0x00adadfc);
    if (table == 0) return 1;
    competition = D(self + 4);
    key = D(competition);
    child = D(table + key * 4);
    if (child == 0) return 1;
    object = D(child + 8);
    if (object == 0) return 1;
    option_value = option;
    if (option_value == 0) return 1;
    out_a = D(local);
    out_b = D(local + 4);
    cm_fn_0049f570(object, record, option_value, out_b, out_a);
    return 1;
}
