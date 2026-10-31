/* WIP: fault detail/address mismatch, case 0, after five failed iterations. */
#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 text);
extern char *CM_CDECL cm_fn_0066f4e0(char *out, const char *format, ...);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern void CM_CDECL cm_fn_00945610(void);

void CM_STDCALL cm_fn_0059f9c0(u32 initial, void *object, void *receiver)
{
    char buffer[0x1388];
    s32 selected;
    u32 amount;
    s32 state;
    u32 base;
    u32 entry;
    const char *format;

    (void)initial;
    cm_fn_00945610();
    state = (s32)cm_fn_00785830(object, 1);
    amount = (u32)cm_fn_00785830(object, 2);
    selected = (s8)cm_fn_00785830(object, 3);
    if (state != -1) {
        base = CM_GLOBAL(volatile u32, 0x00ae23b0u);
        entry = base + (u32)(state * 78);
        if (selected == 0) {
            if (entry == 0) {
                format = (const char *)0x009c9158u;
                (void)cm_fn_0066f4e0(buffer, format);
            } else {
                amount = *(volatile u32 *)(entry + 0x3c) - amount;
                (void)cm_fn_0066f4e0(buffer,
                    (const char *)0x009c91ccu, amount);
            }
        } else if (*(volatile u32 *)(entry + 0x40) == 0) {
            format = (const char *)0x009c9158u;
            (void)cm_fn_0066f4e0(buffer, format);
        } else {
            (void)cm_fn_0066f4e0(buffer,
                (const char *)0x009c9250u, amount);
        }
    } else if (selected == 0) {
        (void)cm_fn_0066f4e0(buffer, (const char *)0x009c8ff0u);
    } else {
        base = CM_GLOBAL(volatile u32, 0x00ae23b0u);
        if (*(volatile u32 *)(base - 0x0eu) != 0) {
            (void)cm_fn_0066f4e0(buffer,
                (const char *)0x009c90d0u, selected);
        } else {
            (void)cm_fn_0066f4e0(buffer,
                (const char *)0x009c9064u);
        }
    }
    (void)cm_fn_004ae660(receiver, (u32)(unsigned long)buffer);
}
