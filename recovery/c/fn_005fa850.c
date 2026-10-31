#include "cmre.h"
/* Preferred-image VA005fa850/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/RET4/full final destructor EAX; SEH95af78/scratch4c. CursorL0,
   saved lastL4,dateL8/Lc/constructorL10,listL18,keyL28. Key writes only
   WORDs L2c/L47; actual comparator5fb3f0 reads only +4/+1f WORDs.
   Preserve FAST4 list-insert EDX: the masked key day in either scan.
   Original uninitialized high words loaded from key are masked before use. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u32 CM_THISCALL cm_fn_0068d020(u32);
extern u32 CM_THISCALL cm_fn_0068d040(u32);
extern u32 CM_THISCALL cm_fn_00549f70(u32,u16,u8,u16,u32);
extern u32 CM_THISCALL cm_fn_005fad20(u32);
extern u32 CM_STDCALL cm_fn_005faec0(u32);
extern unsigned long long CM_FASTCALL cm_fn_0068d0d0(u32,u32,u32,s16);
extern u32 CM_CDECL cm_fn_00946464(u32,u32,u32,u32,u32);
extern u32 CM_CDECL __attribute__((regparm(1))) cm_fn_00945333(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);

u32 CM_THISCALL cm_fn_005fa850(u32 self,volatile u32 date_arg)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x4c];
    u32 l=(u32)scratch,eax,edx,ecx,last,base,date,pointer,first,second;
    u32 cachedyear,cachedday,index,offset,result;
    u16 year,word;
    u8 kind;
    frame.state=0xffffffffu;frame.handler=0x0095af78;
    frame.prev=CM_FS(volatile u32,0);
    CM_FS(volatile u32,0)=(u32)&frame;
    eax=(u32)(s32)(s16)W(self+4);base=D(self);
    last=base+eax*34u-34u;D(l+4)=last;
    cm_fn_0068d020(l+0x18);
    date=date_arg;frame.state=0;year=W(date+2);
    if(year!=2001){
        pointer=cm_fn_00549f70(l+0x10,1,6,year,0xffffffffu);
        first=D(pointer);eax=D(pointer+4);D(l+8)=first;
        year=W(l+0xa);word=W(date+2);D(l+0xc)=eax;
        if(year==word && (u16)first==W(date)) cm_fn_005fad20(self);
    }
    year=W(date+2);base=D(self);W(l+0x2c)=(u16)(year+8u);
    word=W(date);W(l+0x47)=word;
    eax=(u32)(s32)(s16)W(self+4);
    eax=cm_fn_00946464(l+0x28,base,eax,34,0x005fb3f0);D(l)=eax;
    if(eax!=0){
        base=D(self);
        if(eax>base){
            for(;;){
                eax-=34u;D(l)=eax;word=W(eax+4);
                if(word!=W(l+0x2c)){
                    second=D(l+0x2c);ecx=word;
                }else{
                    second=D(l+0x47);ecx=W(eax+0x1f);
                }
                second&=0xffffu;ecx-=second;
                if(ecx!=0){eax+=34u;D(l)=eax;break;}
                if(eax<=base) break;
            }
        }
        if(eax<=last){
            for(;;){
                word=W(eax+4);
                if(word!=W(l+0x2c)){
                    edx=D(l+0x2c);ecx=word;
                }else{
                    edx=D(l+0x47);ecx=W(eax+0x1f);
                }
                edx&=0xffffu;ecx-=edx;
                if(ecx!=0) break;
                if(B(eax+0x1e)==0xfe){B(eax+0x1e)=0xff;eax=D(l);}
                if(B(eax+0x1e)==0xff){
                    cm_fn_0068d0d0(l+0x18,edx,l,4);eax=D(l);
                }
                eax+=34u;D(l)=eax;
                if(eax>last) break;
            }
        }
    }
    year=W(date+2);base=D(self);ecx=(u32)(s32)(s16)W(self+4);
    W(l+0x2c)=(u16)(year+5u);
    eax=cm_fn_00946464(l+0x28,base,ecx,34,0x005fb3f0);D(l)=eax;
    if(eax!=0){
        base=D(self);cachedday=D(l+0x47);cachedyear=D(l+0x2c);
        if(eax>base){
            for(;;){
                eax-=34u;D(l)=eax;word=W(eax+4);
                if(word!=W(l+0x2c)){ecx=word;second=cachedyear;}
                else{second=cachedday;ecx=W(eax+0x1f);}
                second&=0xffffu;ecx-=second;
                if(ecx!=0){eax+=34u;D(l)=eax;break;}
                if(eax<=base) break;
            }
            date=date_arg;
        }
        last=D(l+4);
        if(eax<=last){
            for(;;){
                word=W(eax+4);
                if(word!=W(l+0x2c)){
                    ecx=word;cachedyear&=0xffffu;ecx-=cachedyear;
                }else{
                    cachedday&=0xffffu;ecx=W(eax+0x1f);ecx-=cachedday;
                }
                if(ecx!=0) break;
                cm_fn_0068d0d0(l+0x18,cachedday,l,4);
                eax=D(l);kind=B(eax+0x1e);
                if(kind==0xff || kind==0xfe){
                    result=cm_fn_0090cfc0(3);pointer=D(l);
                    B(pointer+0x1e)=(u8)result;eax=D(l);
                }
                eax+=34u;D(l)=eax;
                if(eax>last) break;
                cachedday=D(l+0x47);cachedyear=D(l+0x2c);
            }
        }
    }
    if(W(l+0x47)==0){
        year=W(date+2);index=0;W(l+0x2c)=year;
        if((s16)W(self+4)>0){
            do{
                offset=(u32)(s32)(s16)(u16)index*34u;
                base=D(self);pointer=base+offset;word=W(pointer+4);
                if(word<W(l+0x2c) && word!=0){
                    second=(u32)(s32)(s8)B(pointer+0x21);
                    W(pointer+4)=(u16)(second+word);
                    base=D(self);kind=B(base+offset+0x1e);
                    pointer=base+offset+0x1e;
                    if(kind!=0xfd) B(pointer)=0xfe;
                }
                ++index;
            }while((s16)(u16)index<(s16)W(self+4));
        }
        eax=(u32)(s32)(s16)W(self+4);base=D(self);
        cm_fn_00945333(eax,base,eax,34,0x005fb3f0);
    }
    cm_fn_005faec0(l+0x18);
    frame.state=0xffffffffu;result=cm_fn_0068d040(l+0x18);
    CM_FS(volatile u32,0)=frame.prev;
    return result;
}
