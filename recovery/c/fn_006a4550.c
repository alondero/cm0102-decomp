#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_00785830(void *, u32);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *, u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32, const char *, u32, u32);
extern u32 CM_CDECL cm_fn_005409b0(u32 *);
extern unsigned long long CM_CDECL cm_fn_006181d0(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);

u32 CM_STDCALL cm_fn_006a4550(u32 input, u32 flag, u32 output)
{
    u8 scratch[0x157c];
    u32 a, b, c, base, record, target, fmt, obj, vtable, result;
    s32 value;
    a = (u32)cm_fn_00785830((void *)input, 0);
    if (!flag) {
        cm_fn_0066f4e0((u32)(scratch + 0x1f4), 0xa0aaec);
        return (u32)cm_fn_004ae660((void *)output, (u32)(scratch + 0x1f4));
    }
    b = (u32)cm_fn_00785830((void *)input, 1);
    c = (u32)cm_fn_00785830((void *)input, 2);
    base = CM_GLOBAL(volatile u32, 0xae23b4);
    a *= 0x245;
    record = CM_GLOBAL(volatile u32, base + a + 0x53);
    if (record) {
        cm_fn_006181d0(record, (u32)scratch, 500, 9);
        base = CM_GLOBAL(volatile u32, 0xae23b4);
    }
    value = (s32)(s16)b + 4;
    target = 0x6a486a;
    if ((u32)value <= 0x9a) {
        u8 index = CM_GLOBAL(volatile u8, 0x6a4978 + (u32)value);
        target = CM_GLOBAL(volatile u32, 0x6a4950 + (u32)index * 4);
    }
    if ((target == 0x6a4619 || target == 0x6a486a) && c == 0xffffffff) {
        result = cm_fn_005409b0((u32 *)(base + a));
        fmt = result ? 0xa0aa5c : 0xa0a9c8;
        cm_fn_0066f4e0((u32)(scratch + 0x1f4), fmt, 100, 100, scratch);
    } else {
        fmt = 0xa0a400;
        obj = 0;
        if (target == 0x6a46c5) {
            obj = CM_GLOBAL(volatile u32, CM_GLOBAL(volatile u32, 0xadadfc) + c * 4);
            if (obj) {
                vtable = CM_GLOBAL(volatile u32, obj);
                obj = ((u32 (CM_THISCALL *)(u32))CM_GLOBAL(volatile u32, vtable + 0x78))(obj);
            }
            fmt = obj ? 0xa0a820 : 0xa0a8b0;
        } else if (target == 0x6a476c) fmt = 0xa0a6ec;
        else if (target == 0x6a4732) fmt = 0xa0a790;
        else if (target == 0x6a468b) fmt = 0xa0a940;
        else if (target == 0x6a479b) fmt = 0xa0a654;
        else if (target == 0x6a47d5) fmt = 0xa0a5b8;
        else if (target == 0x6a4804) fmt = 0xa0a51c;
        else if (target == 0x6a483e) fmt = 0xa0a494;
        if (!obj) obj = CM_GLOBAL(volatile u32, 0xae23d0) + c * 0x6b;
        value = CM_GLOBAL(volatile s8, obj + 0x52);
        cm_fn_0066f4e0((u32)(scratch + 0x1f4), fmt, 100, 100,
                       value, value, scratch, obj + 0x38);
    }
    cm_fn_004ae660((void *)output, (u32)(scratch + 0x1f4));
    base = CM_GLOBAL(volatile u32, 0xae23b4);
    cm_fn_004ae8a0(output, (char *)scratch, 0x7d5, base + a);
    base = CM_GLOBAL(volatile u32, 0xae23d0);
    record = base + c * 0x6b;
    return cm_fn_004ae8a0(output, (char *)(record + 0x38), 0x7d0, record);
}
