#include "cmre.h"
extern void CM_CDECL cm_fn_00944cff(u32 format,u32 zero1,u32 zero2,u32 buffer1,u32 buffer2);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination,u32 format,...);
extern void cm_fn_005e8290(u32 message,u32 buffer,u32 mode);
extern int CM_THISCALL cm_fn_008968f0(u32 context,u32 entry,u32 index,u32 argument);
extern u16 CM_CDECL cm_fn_006e1cf0(u32 candidate,u32 zero,u32 entry);
u8 CM_CDECL cm_fn_00727e20(u32 first,u32 second)
{
    u8 local_area[0x204];
    if(first==0u) {
        {u32 text;
         cm_fn_00944cff(0x00a161d4u,0u,0u,(u32)&local_area[0x104],(u32)&local_area[4]);
         text=cm_fn_0090d130();
         cm_fn_00944c9f(0x00ae24d0u,0x009870e8u,text,(u32)&local_area[0x104],(u32)&local_area[4],0x20e1u);}
        cm_fn_005e8290(0x009870e0u,0x00ae24d0u,0u);
        CM_GLOBAL(volatile u32,0x00b67a34u)=0u;
        return 0xffu;
    }
    {
        u16 best_score=0u;
        u8 best_index=0xffu;
        u32 index;
        for(index=0u;index<11u;++index) {
            u32 context=CM_GLOBAL(volatile u32,0x00de1d08u);
            u32 candidate=(u32)cm_fn_008968f0(context,first,index,second);
            if(candidate!=0u) {
                s16 score=(s16)cm_fn_006e1cf0(candidate,0u,first);
                if(best_index==0xffu || score>(s16)best_score) {
                    best_index=(u8)index;
                    best_score=(u16)score;
                }
            }
        }
        return best_index;
    }
}
