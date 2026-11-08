/* CM0102 3.9.68; preferred-image VA00616f20, image base00400000.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL4. The output byte is written before pointer/argument validation.
   Native language/formatter/variant helpers remain explicit boundaries;
   the variant helper's AL alone is consumed here, not its physical EDX:EAX
   transport. Both original guarded target-table reads are retained. */
#include "cmre.h"
typedef u16 cm_alias_u16 __attribute__((may_alias));
#define B(a) CM_GLOBAL(volatile u8,(u32)(a))
#define W(a) CM_GLOBAL(volatile cm_alias_u16,(u32)(a))
#define D(a) CM_GLOBAL(volatile u32,(u32)(a))
extern u8 CM_CDECL cm_fn_00545a60(u32 style,u32 language);
extern u32 CM_CDECL cm_fn_0066f4d0(void);
extern u32 CM_CDECL cm_fn_0066f4e0(u32 output,u32 format,...);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 output,u32 format,...);
extern u32 CM_CDECL cm_fn_00944cff(u32 file,u32 zero1,u32 zero2,void *a,void *b);
extern u32 CM_CDECL cm_fn_005e8290(u32 title,u32 message,u32 zero);

u32 CM_CDECL cm_fn_00616f20(volatile u32 category,u32 output,
                           volatile u32 capacity,volatile u32 style)
{
    u8 initial=B((u32)&category);
    u32 destination=output,style_value,index,target,format=0,source=0;
    u32 line=0,first=0,second=0;
    u8 style_byte,extra=0;
    s8 variant;
    u8 scratch[0x200];

    B(destination)=0;
    if((s8)initial<1)return 0;
    if((s8)initial>14)return 0;
    if(destination==0)return 0;
    if((s16)W((u32)&capacity)<100)return 0;
    style_value=D((u32)&style);
    style_byte=(u8)style_value;
    if(style_byte!=0xffu) {
        if(cm_fn_0066f4d0()==4u) {
            variant=(s8)cm_fn_00545a60(style_value,4u);
            index=(u32)((s32)(s8)B((u32)&category)*4+(s32)variant);
            source=D(0x009d0518u+index*4u);
            goto copy_string;
        }
        if(cm_fn_0066f4d0()==5u) {
            variant=(s8)cm_fn_00545a60(style_value,5u);
            index=(u32)((s32)(s8)B((u32)&category)*5+(s32)variant);
            source=D(0x009d06d4u+index*4u);
            goto copy_string;
        }
        if(cm_fn_0066f4d0()==9u) {
            variant=(s8)cm_fn_00545a60(style_value,9u);
            index=(u32)((s32)(s8)B((u32)&category)*4+(s32)variant);
            source=D(0x009d05f8u+index*4u);
            goto copy_string;
        }
    }
    if(cm_fn_0066f4d0()==5u || cm_fn_0066f4d0()==2u ||
       cm_fn_0066f4d0()==4u || cm_fn_0066f4d0()==9u) {
        index=(u32)(s32)(s8)B((u32)&category)-1u;
        if(index>13u) {
            first=(u32)(scratch+0);second=(u32)(scratch+0x100);
            cm_fn_00944cff(0x009d93ccu,0,0,(void *)first,(void *)second);
            line=0x21dau;
            goto error_message;
        }
        target=D(0x0061773cu+index*4u);
    } else {
        index=(u32)(s32)(s8)B((u32)&category)-1u;
        if(index>13u) {
            first=(u32)(scratch+0x100);second=(u32)(scratch+0);
            cm_fn_00944cff(0x009d93ccu,0,0,(void *)first,(void *)second);
            line=0x217du;
            goto error_message;
        }
        target=D(0x00617704u+index*4u);
    }
    switch(target) {
    case 0x0061709fu:format=style_byte==0xffu?0x009d9458u:0x009e20bcu;break;
    case 0x006170c6u:format=style_byte==0xffu?0x009d9450u:0x009e20b4u;break;
    case 0x006170edu:format=style_byte==0xffu?0x009e2054u:0x009e2080u;extra=1;break;
    case 0x00617133u:format=style_byte==0xffu?0x009e1f84u:0x009e2040u;break;
    case 0x0061715au:format=style_byte==0xffu?0x009e1f74u:0x009e202cu;break;
    case 0x00617181u:format=style_byte==0xffu?0x009e1f64u:0x009e2018u;break;
    case 0x006171a8u:format=style_byte==0xffu?0x009e1f58u:0x009e2008u;break;
    case 0x006171cfu:format=style_byte==0xffu?0x009e1f48u:0x009e1ff8u;break;
    case 0x006171f6u:format=style_byte==0xffu?0x009d9438u:0x009e1fe8u;break;
    case 0x0061721du:format=style_byte==0xffu?0x009d9430u:0x009e1fe0u;break;
    case 0x00617244u:format=style_byte==0xffu?0x009e1f3cu:0x009e1fd0u;break;
    case 0x0061726bu:format=style_byte==0xffu?0x009d9420u:0x009e1fc0u;break;
    case 0x00617292u:format=style_byte==0xffu?0x009e1f2cu:0x009e1facu;break;
    case 0x006172b9u:format=style_byte==0xffu?0x009e1f1cu:0x009e1fa0u;break;
    case 0x0061732cu:format=style_byte==0xffu?0x009e20bcu:0x009d9458u;break;
    case 0x0061736du:format=style_byte==0xffu?0x009e20b4u:0x009d9450u;break;
    case 0x006173aeu:format=0x009e1f94u;break;
    case 0x006173cfu:format=style_byte==0xffu?0x009e2040u:0x009e1f84u;break;
    case 0x00617410u:format=style_byte==0xffu?0x009e202cu:0x009e1f74u;break;
    case 0x00617451u:format=style_byte==0xffu?0x009e2018u:0x009e1f64u;break;
    case 0x00617492u:format=style_byte==0xffu?0x009e2008u:0x009e1f58u;break;
    case 0x006174d3u:format=style_byte==0xffu?0x009e1ff8u:0x009e1f48u;break;
    case 0x00617514u:format=style_byte==0xffu?0x009e1fe8u:0x009d9438u;break;
    case 0x00617555u:format=style_byte==0xffu?0x009e1fe0u:0x009d9430u;break;
    case 0x00617596u:format=style_byte==0xffu?0x009e1fd0u:0x009e1f3cu;break;
    case 0x006175d7u:format=style_byte==0xffu?0x009e1fc0u:0x009d9420u;break;
    case 0x00617618u:format=style_byte==0xffu?0x009e1facu:0x009e1f2cu;break;
    case 0x00617659u:format=style_byte==0xffu?0x009e1fa0u:0x009e1f1cu;break;
    default:__builtin_unreachable();
    }
    if(extra)cm_fn_0066f4e0(destination,format,0x00ad9c64u);
    else cm_fn_0066f4e0(destination,format);
    return 1u;

copy_string:
    {
        u32 scan=source,remaining=0xffffffffu,length,count;
        do {
            u8 value=B(scan);
            ++scan;--remaining;
            if(value==0)break;
        } while(remaining);
        length=~remaining;
        source=scan-length;
        count=length>>2;
        while(count) {
            u32 word=D(source);
            D(destination)=word;
            source+=4u;destination+=4u;--count;
        }
        count=length&3u;
        while(count) {
            u8 value=B(source);
            B(destination)=value;
            ++source;++destination;--count;
        }
    }
    return 1u;

error_message:
    {
        u32 version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u,0x009870e8u,version,first,second,line);
        cm_fn_005e8290(0x009870e0u,0x00ae24d0u,0);
        D(0x00b67a34u)=0;
    }
    return 0;
}
