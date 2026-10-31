/* WIP: caller reads full EAX, but original AL-only returns preserve incoming upper EAX on early paths; no faithful C transport identified. */
#include "cmre.h"
extern s32 CM_CDECL cm_fn_0090cfc0(s32 value);
u8 CM_THISCALL cm_fn_006bba80(u32 self)
{
    u8 status=*(volatile u8 *)(self+0x475au);
    u32 target;
    u8 target_byte,cached_field;
    s8 month,day;
    s32 rng;
    if(status==0u) return 1u;
    target=*(volatile u32 *)(self+0x4761u);
    if(target==0u) return 1u;
    target_byte=*(volatile u8 *)(target+0x27u);
    cached_field=*(volatile u8 *)(self+0x8eaeu);
    if(target_byte==cached_field) return 2u;
    if(status==8u) goto check_rng;
    day=(s8)*(volatile u8 *)(self+0x475cu);
    month=(s8)*(volatile u8 *)(self+0x475bu);
    if(cached_field==0u) {
        if(month>=2 && month<=6 && day>=10 && day<=11) return 2u;
    } else {
        if(month>=2 && month<=6 && day>=0 && day<=1) return 2u;
    }
check_rng:
    rng=cm_fn_0090cfc0(0xc8);
    if(rng!=0) {
        *(volatile u16 *)(self+0x475fu)=0xffffu;
        *(volatile u32 *)(self+0x4761u)=0u;
        *(volatile u32 *)(self+0x4765u)=0u;
        *(volatile u8 *)(self+0x4769u)=0u;
        *(volatile u8 *)(self+0x475au)=0u;
        *(volatile u8 *)(self+0x475bu)=0xffu;
        *(volatile u8 *)(self+0x475cu)=0xffu;
        *(volatile u8 *)(self+0x475du)=0xffu;
        *(volatile u8 *)(self+0x475eu)=0xffu;
        return 1u;
    }
    return 2u;
}
