#include "cmre.h"
extern s32 CM_CDECL cm_fn_0090cfc0(s32 value);
u32 CM_THISCALL cm_fn_006e9470(u32 self,u32 first,u32 object)
{
    u32 result=first; s16 input;
    *(volatile u32 *)(self)=first;
    *(volatile u32 *)(self+4u)=object;
    if(object==0u) { *(volatile u16 *)(self+8u)=0x1388u; return result; }
    result=(u32)cm_fn_0090cfc0(0x15e); input=*(volatile s16 *)(object+0x1eu);
    if((s32)input>(s32)result){*(volatile u16 *)(self+8u)=0x1e46u;return result;}
    result=(u32)cm_fn_0090cfc0(0x12c); input=*(volatile s16 *)(object+0x1eu);
    if((s32)input>(s32)result){*(volatile u16 *)(self+8u)=0x1c52u;return result;}
    result=(u32)cm_fn_0090cfc0(0xfa); input=*(volatile s16 *)(object+0x1eu);
    if((s32)input>(s32)result){*(volatile u16 *)(self+8u)=0x1a5eu;return result;}
    result=(u32)cm_fn_0090cfc0(0xc8); input=*(volatile s16 *)(object+0x1eu);
    if((s32)input>(s32)result){*(volatile u16 *)(self+8u)=0x186au;return result;}
    result=(u32)cm_fn_0090cfc0(0x96); input=*(volatile s16 *)(object+0x1eu);
    if((s32)input>(s32)result){*(volatile u16 *)(self+8u)=0x1676u;return result;}
    *(volatile u16 *)(self+8u)=0x1482u; return result;
}
