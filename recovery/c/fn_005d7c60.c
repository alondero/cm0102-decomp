#include "cmre.h"
/* Preferred-image VA005d7c60/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/RET4/full boolean EAX; SEH95a4ee/scratch204 allocationL0,
   diagnosticsL104/L4. Retain original byte-map and DWORD target reads,
   computed flags before sound-manager decisions, recursive failure cleanup,
   and every global reload. Unknown sound operations remain boundaries. */
#define D(a) (*(volatile u32 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32);
extern u32 CM_THISCALL cm_fn_0084ca80(u32);
extern u32 CM_THISCALL cm_fn_0084caf0(u32);
extern u32 CM_THISCALL cm_fn_0084cb10(u32);
extern u32 CM_THISCALL cm_fn_0084cbc0(u32,s8,u32,u32);
extern u32 CM_CDECL cm_fn_0090bde0(u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_005d7c60(u32 self,volatile u32 flag_arg)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x204];
    u32 l=(u32)scratch,flag,index,target,mask,value,manager,eax,result,version;
    frame.state=0xffffffffu;frame.handler=0x0095a4ee;
    frame.prev=CM_FS(volatile u32,0);flag=flag_arg;
    CM_FS(volatile u32,0)=(u32)&frame;
    if(flag<=0x100u){
        if(flag==0x100u) goto sound;
        index=flag-1u;
        if(index>0x7fu) goto invalid;
        index=B(0x005d7f94+index);target=D(0x005d7f80+index*4u);
        switch(target){
        case 0x005d7cd6:mask=0xfffffff8u;break;
        case 0x005d7cbb:mask=0xffffffe7u;break;
        case 0x005d7cc6:mask=0xffffff9fu;break;
        case 0x005d7ce1:goto sound;
        case 0x005d7eeb:goto invalid;
        default:__builtin_unreachable(); /* All targets in pinned table above. */
        }
    }else{
        switch(flag){
        case 0x200:case 0x400:case 0x800:mask=0xfffff1ffu;break;
        case 0x1000:case 0x2000:mask=0xffffcfffu;break;
        case 0x4000:case 0x8000:mask=0xffff3fffu;break;
        case 0x10000:case 0x20000:mask=0xfffcffffu;break;
        case 0x40000:case 0x80000:case 0x100000:mask=0xffe3ffffu;break;
        case 0x200000:case 0x400000:case 0x800000:mask=0xff1fffffu;break;
        case 0x1000000:case 0x2000000:case 0x4000000:mask=0xf8ffffffu;break;
        case 0x20000000:case 0x40000000:mask=0x9fffffffu;break;
        default:goto invalid;
        }
    }
    value=D(self+4);D(self+4)=(value&mask)|flag;
    goto success;
sound:
    value=D(self+4);value=(value&0xfffffe7fu)|flag;D(self+4)=value;
    manager=D(0x00dd7eec);
    if((value&0x80u)!=0 && manager==0){
        eax=(u32)cm_fn_00944cf1(0x30);D(l)=eax;frame.state=0;
        if(eax!=0) eax=cm_fn_0084ca80(eax);
        else eax=0;
        frame.state=0xffffffffu;D(0x00dd7eec)=eax;
        if(eax!=0){
            manager=D(eax+0x2c);
            if(manager!=0){
                eax=cm_fn_0090bde0(0x009c382c,0x009cd25c);
                manager=D(0x00dd7eec);
                cm_fn_0084cbc0(manager,2,eax,0);
                goto success;
            }
        }
        cm_fn_005e8290(0x009870e0,0x009cd2cc,0);
        cm_fn_005d7c60(self,0x100);
        manager=D(0x00dd7eec);
        if(manager==0) goto success;
        goto destroy;
    }
    if((value&0x100u)==0 || manager==0) goto success;
    cm_fn_0084cb10(manager);
    manager=D(0x00dd7eec);
    if(manager==0) goto clear;
destroy:
    cm_fn_0084caf0(manager);cm_fn_00944c94(manager);
clear:
    D(0x00dd7eec)=0;
success:
    result=1;goto done;
invalid:
    cm_fn_00944cff(0x009cd3a0,0,0,l+0x104,l+4);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,l+0x104,l+4,0x255);
    cm_fn_005e8290(0x009870e0,0x00ae24d0,0);D(0x00b67a34)=0;
    result=0;
done:
    CM_FS(volatile u32,0)=frame.prev;
    return result;
}
