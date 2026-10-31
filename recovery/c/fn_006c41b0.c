#include "cmre.h"
extern u16 CM_FASTCALL cm_fn_00717410(u32 row);
u8 CM_THISCALL cm_fn_006c41b0(u32 self, u8 mode, u8 value, u8 threshold)
{
    s32 i; u32 row;
    if (mode == 0u) { if ((s8)threshold > 8) return 0u; }
    else if ((s8)threshold < 3) return 0u;
    row = self + ((mode == 0u) ? 8920u : 0u) + 0x4796u;
    for (i=0;i<20;++i,row+=0x1beu) {
        s8 active=*(volatile s8 *)(row+0x19u);
        if(active>=0) {
            s8 field102=*(volatile s8 *)(row+0x102u);
            if((s8)value==field102 && (s8)threshold==*(volatile s8 *)(row+0x103u)) return 0u;
            if(cm_fn_00717410(row)!=1u) {
                s8 field103;
                if(mode==1u) { field103=*(volatile s8 *)(row+0x103u); if((s8)threshold<=field103) continue; }
                else if(mode==0u) { field103=*(volatile s8 *)(row+0x103u); if((s8)threshold>=field103) continue; }
                else continue;
                { s32 f=(s32)*(volatile s8 *)(row+0x102u); s32 v=(s32)(s8)value;
                  if(v+1>f && v-1<f) return 0u; }
            }
        }
    }
    return 1u;
}
