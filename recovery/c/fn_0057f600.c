#include "cmre.h"
/* Preferred-image VA0057f600/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/full EAX0or1; scratch208 IDs L0/L4 initialized -1, diagnostics
   L8/L108. Manager-null error continues. Keep signed BYTE exclusions,
   wrapping WORD scan index, dynamic bounds and post-free capacity reload.
   5fa730 consumes WORD year; qsort receives four stack arguments. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u8 CM_THISCALL cm_fn_005fa730(u32,u32,u16,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0053b3d0(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);
extern u32 CM_CDECL __attribute__((regparm(1))) cm_fn_00945333(u32,u32,u32,u32,u32);

u32 CM_THISCALL cm_fn_0057f600(u32 self)
{
    u8 scratch[0x208];
    u32 l=(u32)scratch,manager,eax,version,id,nation,pointer,tag,continent;
    u32 index,count=0,limit,old,array,excluded;
    u16 year;
    u8 found,slot,byte;
#define REPORT(first,second,line) do { \
    cm_fn_00944cff(0x009c4674,0,0,(first),(second)); \
    version=cm_fn_0090d130(); \
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,(first),(second),(line)); \
    cm_fn_005e8290(0x009870e0,0x00ae24d0,0);D(0x00b67a34)=0; \
} while(0)
    D(self+0xf2)=51;B(self+0xf6)=0;
    manager=D(0x00b5e134);D(l)=0xffffffffu;D(l+4)=0xffffffffu;
    if(manager==0){
        REPORT(l+0x108,l+8,0x4c3);
    }else{
        year=W(self+0x40);year=(u16)(year+2u);id=D(0x009cf7a4);
        found=cm_fn_005fa730(manager,id,year,l,l+4,1);
        slot=0;
        if((s8)found>0){
            do{
                id=D(l+(u32)(s32)(s8)slot*4u);
                if(id==0xffffffffu) break;
                nation=D(0x00ae23a8);pointer=D(nation+id*290u+0x71);
                if(pointer!=0){
                    tag=D(pointer);continent=D(0x009cfa10);
                    if(tag==continent) B(self+0xf6)=(u8)(B(self+0xf6)+1u);
                }
                slot=(u8)(slot+1u);
            }while((s8)slot<(s8)found);
        }
    }
    byte=B(self+0xf6);
    if((s8)byte<0 || (s8)byte>2){
        REPORT(l+8,l+0x108,0x4dd);B(self+0xf6)=1;
    }
    excluded=(u32)(s32)(s8)B(self+0xf6);limit=51u-excluded;
    old=D(self+0xee);D(self+0xf2)=limit;
    if(old!=0) cm_fn_009452ca(old);
    limit=D(self+0xf2);
    eax=(u32)cm_fn_00944e46(limit*4u);D(self+0xee)=eax;
    if(eax==0){REPORT(l+8,l+0x108,0x4ec);return 0;}
    eax=D(0x00ae2350);nation=D(0x00ae23a8);index=0;
    if((s32)eax>0){
        do{
            pointer=D(nation+0x71);
            if(pointer!=0){
                tag=D(pointer);continent=D(0x009cfa10);
                if(tag==continent && (s32)count<(s32)D(self+0xf2)){
                    eax=cm_fn_0053b3d0(nation);
                    id=D(nation);limit=D(l);
                    if(id!=limit && id!=D(l+4)){
                        array=D(self+0xee);++count;D(array+count*4u-4u)=eax;
                    }
                }
            }
            limit=D(0x00ae2350);++index;
            eax=(u32)(s32)(s16)(u16)index;nation+=290u;
        }while((s32)eax<(s32)limit);
    }
    if((s32)count<(s32)D(self+0xf2)){
        REPORT(l+8,l+0x108,0x506);return 0;
    }
    array=D(self+0xee);cm_fn_00945333(eax,array,count,4,0x004b07d0);
#undef REPORT
    return 1;
}
