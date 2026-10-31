#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_007fee80(u32);
extern void CM_THISCALL cm_fn_007feeb0(u32, u32);
extern u32 CM_CDECL cm_fn_0041cad0(u32, u32);
extern u32 CM_CDECL cm_fn_0041cdd0(u32, u32);
extern u32 CM_CDECL cm_fn_0041cbd0(u32, u32);
extern u32 CM_CDECL cm_fn_0041c9d0(u32, u32);
extern u32 CM_CDECL cm_fn_0090bde0(u32, u32);
extern void CM_CDECL cm_fn_0041bfb0(void);

u32 CM_CDECL cm_fn_0041c250(u32 query, u32 lane, u32 only)
{
    u32 text, value;
    text = cm_fn_007fee80(0x00b74450);
    if (B(text) != 0x5c) {
        text = cm_fn_007fee80(0x00b74450);
        if (B(text) != 0)
            return 0;
    }
    value = D(0x00989858);
    if (value == 0) {
        cm_fn_007feeb0(0x00b74450, 0x00989adc);
        return 1;
    }
    if (query != 0) {
        text = cm_fn_0041cad0(query, lane);
        if (text != 0)
            goto found;
        if (only != 0)
            return 0;
        value = D(query + 0x69);
        if (value != 0) {
            text = cm_fn_0041cdd0(value, lane);
            if (text != 0)
                goto found;
        }
        value = D(query + 0x6e);
        if (value != 0) {
            text = cm_fn_0041cdd0(value, lane);
            if (text != 0)
                goto found;
        }
        value = D(query + 0x57);
        if (value != 0) {
            text = cm_fn_0041cbd0(value, lane);
            if (text != 0)
                goto found;
        }
        query = D(query + 0x53);
        if (query != 0) {
            text = cm_fn_0041c9d0(query, lane);
            if (text != 0)
                goto found;
        }
    } else if (only != 0) {
        return 0;
    }
    text = cm_fn_0041cad0(0, lane);
    if (text != 0)
        goto found;
    cm_fn_0041bfb0();
    return 1;
found:
    text = cm_fn_0090bde0(0x009899e0, text);
    cm_fn_007feeb0(0x00b74450, text);
    return 1;
}
