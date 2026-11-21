#include "cmre.h"

extern void CM_CDECL cm_fn_005e8290(const char *, const char *, int);
extern u32 CM_CDECL cm_fn_007615d0(u32);
extern u32 CM_CDECL cm_fn_007643d0(u32);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *, u32);
extern u32 CM_THISCALL cm_fn_00786b70(u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00786d30(u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_007ff010(u32, u32);
extern void CM_THISCALL cm_fn_009037b0(u32, const u32 *);
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);

u32 CM_CDECL cm_fn_0049ce50(u32 selection)
{
    volatile struct { void *prev; u32 handler; u32 state; } frame;
    u8 scratch[0x2e0];
    u32 first, second, result, value, base;
    s16 idx, group;
    u16 fallback;

    frame.state = 0xffffffff;
    frame.handler = 0x95639b;
    frame.prev = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&frame;
    cm_fn_0077bbe0(scratch, 0, 0);
    frame.state = 0;
    first = cm_fn_007ff010(0xb74450, 0);
    second = cm_fn_007ff010(0xb74450, 1);
    idx = (s16)selection;
    group = CM_GLOBAL(volatile s16, 0xb77c7a);
    if (idx == -1) value = 0;
    else {
        base = CM_GLOBAL(volatile u32, 0xb74c4c + (u32)(s32)group * 0x300);
        value = CM_GLOBAL(volatile u32, base + 0xba966 + (u32)(s32)idx * 0x18c);
    }
    fallback = CM_GLOBAL(volatile u16, 0xdd6bde);
    if ((u16)value == 0) value = (value & 0xffff0000u) | fallback;
    if ((u16)value == 2) {
        if (!cm_fn_00786b70(CM_GLOBAL(volatile u32, 0xb7431c), first, second, (u32)scratch)) {
            char *text;
            cm_fn_00944cff(0x99c15c, 0, 0, scratch + 0x1e0, scratch + 0xe0);
            text = cm_fn_0090d130();
            cm_fn_00944c9f(0xae24d0, 0x9870e8, text, scratch + 0x1e0, scratch + 0xe0, 0x1e7f);
            cm_fn_005e8290((const char *)0x9870e0, (const char *)0xae24d0, 0);
            CM_GLOBAL(volatile u32, 0xb67a34) = 0;
            result = 0;
        } else {
            u32 index = (u32)cm_fn_00785830(scratch, 1);
            u32 address = CM_GLOBAL(volatile u32, 0xae23b4) + index * 0x245;
            cm_fn_009037b0(CM_GLOBAL(volatile u32, 0xde1f58), (const u32 *)address);
            cm_fn_00786d30(CM_GLOBAL(volatile u32, 0xb7431c), first, second, 0, 1, 0);
            cm_fn_00786d30(CM_GLOBAL(volatile u32, 0xb7431c), first, second, 3, 1, 0);
            result = (u32)-9;
        }
    } else {
        if (idx == -1) value = 0;
        else {
            base = CM_GLOBAL(volatile u32, 0xb74c4c + (u32)(s32)group * 0x300);
            value = CM_GLOBAL(volatile u32, base + 0xba966 + (u32)(s32)idx * 0x18c);
        }
        if ((u16)value == 0) value = (value & 0xffff0000u) | fallback;
        if ((u16)value == 1) {
            cm_fn_00786d30(CM_GLOBAL(volatile u32, 0xb7431c), first, second, 0, 1, 0);
            cm_fn_00786d30(CM_GLOBAL(volatile u32, 0xb7431c), first, second, 3, 0, 0);
            result = (u32)-9;
        } else {
            result = cm_fn_007615d0(selection);
            if (!result) result = cm_fn_007643d0(selection);
        }
    }
    CM_FS(void *, 0) = frame.prev;
    return result;
}
