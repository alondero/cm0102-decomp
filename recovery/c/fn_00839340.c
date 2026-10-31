#include "cmre.h"

extern u32 *CM_CDECL cm_fn_00945a60(u32 *first, char *second);

s32 CM_CDECL cm_fn_00839340(volatile u32 *object, u32 text)
{
    u32 value;
    u32 reference;

    if (cm_fn_00945a60((u32 *)text, (char *)0x00a82514) != 0)
        return 1;
    if (cm_fn_00945a60((u32 *)text, (char *)0x00a82508) != 0)
        goto group_a;
    if (cm_fn_00945a60((u32 *)text, (char *)0x00a82500) != 0)
        goto group_a;
    if (cm_fn_00945a60((u32 *)text, (char *)0x009d6814) == (u32 *)text)
        goto group_a;

    if (cm_fn_00945a60((u32 *)text, (char *)0x00a824f0) != 0) {
        reference = CM_GLOBAL(volatile u32, 0x009cf448);
        value = *(volatile u32 *)object;
        if (value == reference)
            return 1;
        reference = CM_GLOBAL(volatile u32, 0x009cf2ac);
        if (value == reference)
            return 1;
        reference = CM_GLOBAL(volatile u32, 0x009cf430);
        if (value == reference)
            return 1;
        return 0;
    }
    if (cm_fn_00945a60((u32 *)text, (char *)0x00a824e4) != 0) {
        value = *(volatile u32 *)object;
        reference = CM_GLOBAL(volatile u32, 0x009cf4a8);
        if (value == reference)
            return 1;
        reference = CM_GLOBAL(volatile u32, 0x009cf408);
        if (value == reference)
            return 1;
        reference = CM_GLOBAL(volatile u32, 0x009cf2fc);
        if (value == reference)
            return 1;
        reference = CM_GLOBAL(volatile u32, 0x009cf2c0);
        if (value == reference)
            return 1;
        return 0;
    }
    if (cm_fn_00945a60((u32 *)text, (char *)0x00a824d8) != 0) {
        reference = CM_GLOBAL(volatile u32, 0x009cf260);
        value = *(volatile u32 *)object;
        if (value == reference)
            return 1;
        reference = CM_GLOBAL(volatile u32, 0x009cf218);
        if (value == reference)
            return 1;
        reference = CM_GLOBAL(volatile u32, 0x009cf4f8);
        if (value == reference)
            return 1;
        return 0;
    }
    if (cm_fn_00945a60((u32 *)text, (char *)0x009d681c) != 0) {
        reference = CM_GLOBAL(volatile u32, 0x009cf224);
        value = *(volatile u32 *)object;
        if (value == reference)
            return 1;
        reference = CM_GLOBAL(volatile u32, 0x009cf36c);
        if (value == reference)
            return 1;
        return 0;
    }
    return 0;

group_a:
    value = *(volatile u32 *)object;
    reference = CM_GLOBAL(volatile u32, 0x009cf2e4);
    if (value == reference)
        return 1;
    reference = CM_GLOBAL(volatile u32, 0x009cf45c);
    if (value == reference)
        return 1;
    reference = CM_GLOBAL(volatile u32, 0x009cf3e4);
    if (value == reference)
        return 1;
    reference = CM_GLOBAL(volatile u32, 0x009cf2dc);
    if (value == reference)
        return 1;
    reference = CM_GLOBAL(volatile u32, 0x009cf510);
    if (value == reference)
        return 1;
    reference = CM_GLOBAL(volatile u32, 0x009cf300);
    if (value == reference)
        return 1;
    reference = CM_GLOBAL(volatile u32, 0x009cf488);
    if (value == reference)
        return 1;
    reference = CM_GLOBAL(volatile u32, 0x009cf360);
    if (value == reference)
        return 1;
    reference = CM_GLOBAL(volatile u32, 0x009cf314);
    if (value == reference)
        return 1;
    reference = CM_GLOBAL(volatile u32, 0x009cf338);
    if (value == reference)
        return 1;
    reference = CM_GLOBAL(volatile u32, 0x009cf244);
    if (value == reference)
        return 1;
    reference = CM_GLOBAL(volatile u32, 0x009cf434);
    if (value == reference)
        return 1;
    reference = CM_GLOBAL(volatile u32, 0x009cf4dc);
    if (value == reference)
        return 1;
    reference = CM_GLOBAL(volatile u32, 0x009cf31c);
    if (value == reference)
        return 1;
    return 0;
}
