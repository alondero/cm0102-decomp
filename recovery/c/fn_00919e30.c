#include "cmre.h"
/* Preferred-image VA00919e30/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/fullEAX0or1; scratch20c IDsL0,L4/lookupL8/diagnosticsLc,L10c.
   Both IDs start -1, including the manager-null diagnostic continuation.
   5fa730 consumes only lowWORD year (5fa745), returns signed lowBYTE count;
   stop on first -1 ID. Keep signed BYTE exclusions, WORD loop wrap and
   dynamic nation count. Qsort retains actual EAX plus four stack words. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u8 CM_THISCALL cm_fn_005fa730(u32,u32,u16,u32,u32,u32);
extern u32 CM_CDECL cm_fn_004afd80(u32,u32);
extern u32 CM_CDECL cm_fn_0053b3d0(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL __attribute__((regparm(1))) cm_fn_00945333(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_00919e30(u32 self)
{
    u8 scratch[0x20c];
    u32 l=(u32)scratch,manager,eax,id,base,pointer,nation,tag,continent;
    u32 lookup,count=0,index,limit,array,version;
    u16 year;
    u8 available,byte,excluded;
#define REPORT(first,second,line) do { \
        cm_fn_00944cff(0x00ad4a6c,0,0,first,second); \
        version=cm_fn_0090d130(); \
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,first,second,line); \
        cm_fn_005e8290(0x009870e0,0x00ae24d0,0); \
        D(0x00b67a34)=0; \
    } while(0)
    D(self+0xf2)=50;
    B(self+0xf6)=0;
    W(self+0xa7)=0xffff;
    manager=D(0x00b5e134);
    D(l)=0xffffffffu;D(l+4)=0xffffffffu;
    if(manager==0){
        REPORT(l+0x10c,l+0xc,0x4c1);
    }else{
        year=(u16)(W(self+0x40)+2u);
        id=D(0x009cf79c);
        available=cm_fn_005fa730(manager,id,year,l,l+4,1);
        byte=0;
        if((s8)available>0){
            do{
                id=D(l+(u32)(s32)(s8)byte*4u);
                if(id==0xffffffffu) break;
                base=D(0x00ae23a8);
                pointer=D(base+id*290u+0x71);
                if(pointer!=0){
                    tag=D(pointer);continent=D(0x009cfa10);
                    if(tag==continent) B(self+0xf6)=(u8)(B(self+0xf6)+1u);
                }
                byte=(u8)(byte+1u);
            }while((s8)byte<(s8)available);
        }
    }
    id=D(0x009cf79c);base=D(0x00ae23d0);
    eax=cm_fn_004afd80(base+id*107u,0);
    D(l+8)=eax;
    if(eax!=0){
        nation=D(eax+0x53);
        if(nation!=0){
            pointer=D(nation+0x71);
            if(pointer!=0){
                tag=D(pointer);continent=D(0x009cfa10);
                if(tag==continent){
                    id=D(nation);eax=D(l);
                    if(id!=eax && id!=D(l+4))
                        B(self+0xf6)=(u8)(B(self+0xf6)+1u);
                }
            }
        }
    }
    excluded=B(self+0xf6);
    D(self+0xf2)=50u-(u32)(s32)(s8)excluded;
    eax=D(self+0xee);
    if(eax!=0) cm_fn_009452ca(eax);
    eax=(u32)cm_fn_00944e46(200);
    D(self+0xee)=eax;
    if(eax==0){
        REPORT(l+0xc,l+0x10c,0x4f4);
        return 0;
    }
    limit=D(0x00ae2350);nation=D(0x00ae23a8);index=0;
    if((s32)limit>0){
        do{
            pointer=D(nation+0x71);
            if(pointer!=0){
                tag=D(pointer);continent=D(0x009cfa10);
                if(tag==continent && (s32)count<50){
                    eax=cm_fn_0053b3d0(nation);
                    id=D(nation);lookup=D(l);
                    if(id!=lookup && id!=D(l+4) && eax!=D(l+8)){
                        array=D(self+0xee);
                        ++count;D(array+count*4u-4u)=eax;
                    }
                }
            }
            limit=D(0x00ae2350);
            ++index;nation+=290u;
        }while((s32)(s16)(u16)index<(s32)limit);
    }
    if((s32)count<(s32)D(self+0xf2)) return 0;
    array=D(self+0xee);
    cm_fn_00945333(array,array,count,4,0x004b07d0);
    excluded=B(self+0xf6);
    if(excluded==0){
        B(self+0xff)=8;B(self+0x100)=9;B(self+0x101)=10;
    }else if(excluded==1 || excluded==2){
        B(self+0xff)=7;B(self+0x100)=8;B(self+0x101)=9;
    }else{
        B(self+0xff)=6;B(self+0x100)=7;B(self+0x101)=8;
    }
#undef REPORT
    return 1;
}
