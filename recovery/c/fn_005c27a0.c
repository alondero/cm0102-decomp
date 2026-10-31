#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32);
extern u32 CM_CDECL cm_fn_0053b900(u32);
extern u32 CM_CDECL cm_fn_00945333(u32, u32, u32, u32);
extern void CM_THISCALL cm_fn_005c2d40(u32, u32, u32);
extern void CM_THISCALL cm_fn_005c3040(u32, u32, u32, u32);
extern void CM_THISCALL cm_fn_005c4770(u32, u32, u32, u32);
extern void CM_FASTCALL cm_fn_0068d040(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_FASTCALL cm_fn_005c27a0(u32 self)
{
    u8 scratch[0x208];
    u32 local = (u32)scratch;
    u32 total, records, entries, selected, person, row, value, nation;
    u32 i, count, pointer, cursor, object, context, text;
    s32 rating;
    total = D(0x00ae235c);
    D(local + 4) = self;
    selected = 0;
    records = (u32)cm_fn_00944e46(total * 22);
    if (records == 0) {
        cm_fn_00944cff(0x009cd0a0, 0, 0, local + 0x108, local + 8);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                      local + 0x108, local + 8, 0x18a);
        value = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return value;
    }
    total = D(0x00ae235c);
    person = D(0x00ae23b4);
    D(local) = 0;
    if ((s32)total > 0) {
        do {
            if (cm_fn_0053b900(person) == 0 && D(person + 0x53) != 0) {
                row = records + (u32)(s32)(s16)selected * 22;
                value = D(person);
                D(row) = value;
                B(row + 4) = 0;
                rating = (s16)W(person + 0x80);
                /* Signed high multiply10624dd3, SAR5 and sign correction
                   implement truncation toward zero of the signed WORD /500. */
                B(row + 5) = (u8)(rating / 500);
                nation = D(person + 0x53);
                value = D(nation);
                D(row + 6) = value;
                D(row + 0xa) = 0;
                D(row + 0x12) = 0;
                W(row + 0xe) = 0xffff;
                W(row + 0x10) = 0xffff;
                ++selected;
            }
            i = D(local);
            person += 0x245;
            ++i;
            D(local) = i;
            total = D(0x00ae235c);
        } while ((s32)(s16)i < (s32)total);
    }
    count = (u32)(s32)(s16)selected;
    cm_fn_00945333(records, count, 22, 0x005c5270);
    entries = (u32)cm_fn_00944e46(count * 4);
    if ((s16)selected > 0) {
        pointer = entries;
        row = records;
        i = count;
        do {
            D(pointer) = row;
            row += 22;
            pointer += 4;
            --i;
        } while (i != 0);
    }
    cm_fn_00945333(entries, count, 4, 0x005c5290);
    context = D(local + 4);
    cm_fn_005c2d40(context, entries, count);
    context = D(local + 4);
    cm_fn_005c3040(context, records, entries, count);
    context = D(local + 4);
    cm_fn_005c4770(context, records, entries, count);
    if ((s16)selected > 0) {
        cursor = records + 0x12;
        D(local) = count;
        do {
            object = D(cursor - 8);
            if (object != 0) {
                cm_fn_0068d040(object);
                cm_fn_00944c94(object);
            }
            object = D(cursor);
            if (object != 0) {
                cm_fn_0068d040(object);
                cm_fn_00944c94(object);
            }
            value = D(local);
            cursor += 22;
            --value;
            D(local) = value;
        } while (value != 0);
    }
    cm_fn_009452ca(records);
    return (u32)cm_fn_009452ca(entries);
}
