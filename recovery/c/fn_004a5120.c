#include "cmre.h"
/* Preferred-image VA004a5120/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STD3/RET12/full boolean; scratch200 diagnosticsL0,L100. Keep signed
   BYTE selector, unsigned adjusted guard, DWORD target-table read, early
   output clear and the two reversed diagnostic-buffer argument orders. */
#define D(a) (*(volatile u32 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u32 CM_CDECL cm_fn_0066f4e0(u32,u32,...);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_STDCALL cm_fn_004a5120(volatile u32 selector_arg,volatile u32 output_arg,
                             volatile u32 capacity_arg)
{
    u8 scratch[0x200];
    u32 l=(u32)scratch,output=output_arg,index,target,format,first,second,line,version;
    if(output==0 || (s32)capacity_arg<50){
        first=l;second=l+0x100;line=0xe1d;goto error;
    }
    index=(u32)(s32)(s8)(u8)selector_arg-1u;B(output)=0;
    if(index>21){first=l+0x100;second=l;line=0xec3;goto error;}
    target=D(0x004a5464+index*4u);
    switch(target){
    case 0x004a515c:format=0x0099cd50;break;
    case 0x004a5178:format=0x0099cd3c;break;
    case 0x004a5194:format=0x0099cd2c;break;
    case 0x004a51b0:format=0x0099cd18;break;
    case 0x004a51cc:format=0x0099cd00;break;
    case 0x004a51e8:format=0x0098b9c8;break;
    case 0x004a5204:format=0x0098b99c;break;
    case 0x004a5220:format=0x0099cce4;break;
    case 0x004a523c:format=0x0098b97c;break;
    case 0x004a5258:format=0x0098b940;break;
    case 0x004a5274:format=0x0098b90c;break;
    case 0x004a5290:format=0x0098b8f4;break;
    case 0x004a52ac:format=0x0098b8d8;break;
    case 0x004a52c8:format=0x0098b8bc;break;
    case 0x004a52e4:format=0x0098b8ac;break;
    case 0x004a5300:format=0x0098b880;break;
    case 0x004a531c:format=0x0098b870;break;
    case 0x004a5338:format=0x0098b858;break;
    case 0x004a5354:format=0x0098b844;break;
    case 0x004a5370:format=0x0098b830;break;
    case 0x004a538c:format=0x0098b7e8;break;
    case 0x004a53a8:format=0x0098b7d8;break;
    default:__builtin_unreachable();
    }
    cm_fn_0066f4e0(output,format);return 1;
error:
    cm_fn_00944cff(0x0099ca94,0,0,first,second);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,first,second,line);
    cm_fn_005e8290(0x009870e0,0x00ae24d0,0);D(0x00b67a34)=0;
    return 0;
}
