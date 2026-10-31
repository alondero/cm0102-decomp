#include "cmre.h"
extern u32 CM_FASTCALL cm_fn_00717410(u32 row);
extern u8 CM_THISCALL cm_fn_007165b0(u32 row,u32 zero);
extern s32 CM_CDECL cm_fn_0090cfc0(s32 value);

u8 CM_THISCALL cm_fn_006c2310(u32 self,s8 slot,u8 mode)
{
    volatile float best=0.0f;
    volatile u8 best_index=0xffu;
    u32 row_address=self+0x4796u+(u32)(s32)slot*8920u;
    u8 row=0u;
    do {
        s8 eligible=*(volatile s8 *)(row_address+0x19u);
        if (eligible>=0) {
            u32 flags=cm_fn_00717410(row_address);
            volatile float factor=(flags&1u)!=0u ? 0.2f : 1.0f;
            u8 random_value=(u8)cm_fn_007165b0(row_address,0u);
            s32 argument=(s32)(s8)random_value+(mode==2u ? 0x19 : 5);
            s32 result=cm_fn_0090cfc0(argument);
            long double score=(long double)result*(long double)factor;
            if (best_index==0xffu || score>(long double)best) {
                best=(float)score;
                best_index=row;
            }
        }
        ++row;
        row_address+=0x1beu;
    } while (row<0x14u);
    return best_index;
}
