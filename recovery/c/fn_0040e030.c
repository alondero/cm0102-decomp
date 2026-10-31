#include "cmre.h"

/* Preferred-image VA 0040e030, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Original virtual slot and constructors remain boundaries. Both output
   locals are read as DWORDs, but 522e00 consumes their low WORDs only;
   its year argument also consumes only the original CX. No upper-bit init. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_THISCALL cm_fn_0051c410(u32, u32);
extern u32 CM_THISCALL cm_fn_00522e00(u32,u32,s16,u32,s16,u32,u32,u16,u8,u8,u16,u8,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

static u32 diagnostic(u32 name, u32 extension, u32 line)
{
    u32 version, result;
    cm_fn_00944cff(0x00987b60, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, name, extension, line);
    result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return result;
}

u32 CM_THISCALL cm_fn_0040e030(u32 self)
{
    volatile struct {u32 prev, handler, state;} frame;
    u8 scratch[0x40c];
    u32 local = (u32)scratch, clubs, used = 0, index, table, record, value;
    u32 first, second, vtable, target, schedule, object, result, competition;
    u32 output_round_flag, output_rounds;
    u16 year;
    frame.state = 0xffffffffu;
    frame.handler = 0x0095387e;
    frame.prev = CM_FS(u32,0);
    CM_FS(u32,0) = (u32)&frame;
    clubs = cm_fn_00944e46(0x10);
    if (clubs == 0) {
        result = diagnostic(local + 0x20c, local + 0xc, 0x34b);
        goto done;
    }
    index = 0;
    if ((s16)W(self + 0x3e) > 0) {
        do {
            if ((s16)used >= 4) break;
            table = D(self + 0xa7);
            record = table + (u32)(s32)(s16)index * 6;
            if (B(record + 5) == 1) {
                value = D(record);
                D(clubs + (u32)(s32)(s16)used * 4) = value;
                ++used;
            }
            ++index;
        } while ((s16)index < (s16)W(self + 0x3e));
    }
    index = 0;
    if ((s16)W(self + 0x3e) > 0) {
        do {
            if ((s16)used >= 4) break;
            table = D(self + 0xc);
            table = D(table);
            table = D(table + 0xa7);
            record = table + (u32)(s32)(s16)index * 6;
            if (B(record + 5) == 1) {
                value = D(record);
                D(clubs + (u32)(s32)(s16)used * 4) = value;
                ++used;
            }
            ++index;
        } while ((s16)index < (s16)W(self + 0x3e));
    }
    first = D(clubs + 4);
    second = D(clubs + 8);
    D(clubs + 8) = first;
    D(clubs + 4) = second;
    vtable = D(self);
    target = D(vtable + 0x3c);
    schedule = ((u32 (CM_THISCALL *)(u32,u32,u32,u32,u32))target)
        (self, 1, local, local + 4, 0);
    object = cm_fn_00944cf1(0xb2);
    D(local + 8) = object;
    frame.state = 0;
    if (object != 0) {
        output_round_flag = D(local + 4);
        year = W(self + 0x40);
        competition = D(self + 4);
        output_rounds = D(local);
        result = cm_fn_00522e00(object,self,4,clubs,(s16)output_rounds,
            competition,schedule,year,1,2,(u16)output_round_flag,0x14,0,0,0,0);
    } else result = 0;
    table = D(self + 0xc);
    frame.state = 0xffffffffu;
    D(table + 4) = result;
    result = cm_fn_009452ca(schedule);
    table = D(self + 0xc);
    object = D(table + 4);
    if (object == 0) {
        result = diagnostic(local + 0x30c, local + 0x10c, 0x375);
    } else {
        cm_fn_0051c410(object, 0);
        result = cm_fn_009452ca(clubs);
    }
done:
    CM_FS(u32,0) = frame.prev;
    return result;
}
