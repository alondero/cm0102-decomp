#include "cmre.h"

extern s32 CM_CDECL cm_fn_0090cfc0(s32 value);

void CM_CDECL cm_fn_00547f90(volatile u8 *object)
{
    u32 child;

    if (object == 0) {
        return;
    }
    child = *(volatile u32 *)(object + 0x65u);
    if (child == 0) {
        return;
    }
    if (*(volatile u32 *)(child + 0x1cu) != 0) {
        s32 index = (s32)*(volatile s8 *)(object + 0x59u) + 0x0f;
        if (cm_fn_0090cfc0(index) == 0) {
            child = *(volatile u32 *)(object + 0x65u);
            *(volatile u32 *)(child + 0x1cu) = 0;
        }
    }
    child = *(volatile u32 *)(object + 0x65u);
    if (*(volatile u32 *)(child + 0x20u) != 0) {
        s32 index = (s32)*(volatile s8 *)(object + 0x59u) + 0x0f;
        if (cm_fn_0090cfc0(index) == 0) {
            child = *(volatile u32 *)(object + 0x65u);
            *(volatile u32 *)(child + 0x20u) = 0;
        }
    }
    child = *(volatile u32 *)(object + 0x65u);
    if (*(volatile u32 *)(child + 0x24u) != 0) {
        s32 index = (s32)*(volatile s8 *)(object + 0x59u) + 0x0f;
        if (cm_fn_0090cfc0(index) == 0) {
            child = *(volatile u32 *)(object + 0x65u);
            *(volatile u32 *)(child + 0x24u) = 0;
        }
    }
    child = *(volatile u32 *)(object + 0x65u);
    if (*(volatile u32 *)(child + 0x28u) != 0) {
        s32 index = 0x23 - (s32)*(volatile s8 *)(object + 0x5du);
        if (cm_fn_0090cfc0(index) == 0) {
            child = *(volatile u32 *)(object + 0x65u);
            *(volatile u32 *)(child + 0x28u) = 0;
        }
    }
    child = *(volatile u32 *)(object + 0x65u);
    if (*(volatile u32 *)(child + 0x2cu) != 0) {
        s32 index = 0x23 - (s32)*(volatile s8 *)(object + 0x5du);
        if (cm_fn_0090cfc0(index) == 0) {
            child = *(volatile u32 *)(object + 0x65u);
            *(volatile u32 *)(child + 0x2cu) = 0;
        }
    }
    child = *(volatile u32 *)(object + 0x65u);
    if (*(volatile u32 *)(child + 0x30u) != 0) {
        s32 index = 0x23 - (s32)*(volatile s8 *)(object + 0x5du);
        if (cm_fn_0090cfc0(index) == 0) {
            child = *(volatile u32 *)(object + 0x65u);
            *(volatile u32 *)(child + 0x30u) = 0;
        }
    }
}
