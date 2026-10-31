#include "cmre.h"
/* Preferred-image VA0057fa10/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/full EAX forwarded by caller57f9ee. SEH958980/scratch230:
   recordoffL0/selectorL4/chosenL8/allocationL10/IDsL18/datesL20,L28,
   temporariesL8,L10,L18/diagnosticsL30,L130. Date constructor consumes
   low WORD inputs only; 88c6d0 consumes only BYTE flag inputs.
   Its original vtable96ff90[0] is88c970, THIS1/RET4. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_00549f70(u32,u32,u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00549ef0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054c770(u32,u32,u8);
extern u32 CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_006827d0(u32,u32);
extern u32 CM_THISCALL cm_fn_006826d0(u32,u32);
extern u32 CM_THISCALL cm_fn_0088c6d0(u32,u16,u32,u32,u32,u32,u32,u32,u8,u8,u32,u32,u32);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);
typedef u32 (CM_THISCALL *destroy_57fa10)(u32,u32);

u32 CM_THISCALL cm_fn_0057fa10(u32 self)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x230];
    u32 l=(u32)scratch,eax,year,day,base,span,pointer,first,second,child;
    u32 index,ids,allocation,count,list,selector,bytes,filter,object;
    u32 source,destination,recordoff,offset,target,version;
    u8 flag_a,flag_b,byte;
    frame.state=0xffffffffu;frame.handler=0x00958980;
    frame.prev=CM_FS(volatile u32,0);CM_FS(volatile u32,0)=(u32)&frame;
    cm_fn_0054a110(l+0x20);frame.state=0;
    cm_fn_0054a110(l+0x28);year=W(self+0x40);B((u32)&frame.state)=1;
    if(year==1998){
        eax=cm_fn_00549f70(l+0x10,1,5,1998,0xffffffffu);
        B((u32)&frame.state)=2;
    }else{
        base=D(self+0xba);second=W(base+2);second=(u16)(second+year);first=W(base);
        eax=(u32)cm_fn_00549ef0(l+0x10,first,second);
        B((u32)&frame.state)=3;
    }
    cm_fn_00417c10(l+0x20,l+8,eax);
    count=(u32)(s32)(s16)W(self+0xa9);B((u32)&frame.state)=1;
    span=count*65u;base=D(self+0xba);pointer=base+span;
    year=W(pointer-0x3f);day=W(pointer-0x41);year=(u16)(year+W(self+0x40));
    eax=(u32)cm_fn_00549ef0(l+0x10,day,year);
    B((u32)&frame.state)=4;eax=cm_fn_0054c770(eax,l+8,1);
    B((u32)&frame.state)=5;cm_fn_00417c10(l+0x28,l+0x18,eax);
    B((u32)&frame.state)=1;D(l)=0xffffffc5u;D(l+4)=0xfffffffcu;
    selector=0xfffffffcu;
    for(;;){
        if((s32)selector>-4){base=D(self+0xc);child=D(base+selector);}else child=self;
        cm_fn_006827d0(child,0);eax=cm_fn_006826d0(child,1);
        first=D(eax);D(l+8)=first;
        eax=(u32)cm_fn_00944e46(16);D(l+0x18)=eax;
        if(eax==0) goto error;
        ids=eax;index=0;destination=ids;
        do{
            eax=cm_fn_006826d0(child,index);pointer=D(eax);++index;destination+=4;
            first=D(pointer);D(destination-4)=first;
        }while((s32)index<4);
        eax=(u32)cm_fn_00944cf1(0xee);D(l+0x10)=eax;
        B((u32)&frame.state)=6;
        if(eax!=0){
            allocation=eax;count=(u32)(s32)(s16)W(child+0x3e);list=D(self+0x10);
            selector=(count-1u)*count*9u;bytes=self+0xc5;
            flag_b=B(self+0xc3);flag_a=B(self+0xc2);
            base=D(self+4);filter=D(base);ids=D(l+0x18);
            object=cm_fn_0088c6d0(allocation,4,ids,0xffffffffu,0xffffffffu,
                l+0x20,l+0x28,filter,flag_a,flag_b,bytes,selector,list);
        }else object=0;
        B((u32)&frame.state)=1;index=0;
        do{
            eax=cm_fn_006826d0(object,index);first=D(l+8);second=D(eax);
            if(second==first){
                source=D(object+0xb1)+index*59u;
                offset=(u32)(s32)(s8)B(self+0x100);base=D(self+0xc);
                child=D(base+offset*4u);recordoff=D(l);base=D(child+0xb1);
                destination=base+recordoff+59u;
                for(index=0;index<14;++index){
                    first=D(source);D(destination)=first;source+=4;destination+=4;
                }
                first=W(source);W(destination)=(u16)first;source+=2;destination+=2;
                byte=B(source);B(destination)=byte;
                offset=(u32)(s32)(s8)B(self+0x100);base=D(self+0xc);
                child=D(base+offset*4u);base=D(child+0xb1);D(base+recordoff+0x44)=0;
                base=D(self+0xc);offset=(u32)(s32)(s8)B(self+0x100);
                child=D(base+offset*4u);base=D(child+0xb1);D(base+recordoff+0x40)=0;
                break;
            }
            ++index;
        }while((s32)index<4);
        if(object!=0){base=D(object);target=D(base);((destroy_57fa10)target)(object,1);}
        pointer=D(l+0x18);cm_fn_009452ca(pointer);
        selector=D(l+4);recordoff=D(l);selector+=4;recordoff+=59;
        D(l+4)=selector;D(l)=recordoff;
        if((s32)selector>=32) break;
    }
    offset=(u32)(s32)(s8)B(self+0x100);base=D(self+0xc);child=D(base+offset*4u);
    eax=cm_fn_006827d0(child,0);goto done;
error:
    cm_fn_00944cff(0x009c4674,0,0,l+0x130,l+0x30);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,l+0x130,l+0x30,0x55b);
    eax=cm_fn_005e8290(0x009870e0,0x00ae24d0,0);D(0x00b67a34)=0;
done:
    CM_FS(volatile u32,0)=frame.prev;return eax;
}
