#include "cmre.h"

/* Preferred-image VA 00422cf0, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch208: tag/allocation L0, constructor flags L4..7, diagnostics L8/L108.
   Keep full EAX on AL-only lookup/error exits, cached lookup count/array,
   signed BYTE lookup and signed WORD row/rank loops, and recursive lookup. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_CDECL cm_fn_00945333(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0068b100(u32,u32,u16,u32,u16,u32,u32,u16,u8,u8,u8,u32,u32,u16,u8,u16,u8,u8,u16,u16,u16,u32,u8);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_00422cf0(u32 self,volatile u8 stage_arg,
                              volatile u16 competition_arg,volatile u32 create_arg)
{
    volatile struct { u32 prev,handler,state; } frame;
    u8 scratch[0x208];
    u32 local=(u32)scratch,result,stage,tag,count,array,index,club,owner,offset,value;
    u32 allocation,version,year,c4,c3,c2,competition,table,slot;
    frame.state=0xffffffffu;
    frame.handler=0x0095415e;
    frame.prev=CM_FS(volatile u32,0);
    result=frame.prev;
    CM_FS(volatile u32,0)=(u32)&frame;
    stage=stage_arg;
    result=(result&0xffffff00u)|stage;
    competition=competition_arg;
    B(local+4)=0;
    B(local+5)=1;
    B(local+6)=0;
    B(local+7)=0;
    if (competition==0x41f) tag=(stage==1)?0x45d:0x45e + (stage!=2);
    else tag=(stage==1)?0x460:0x461+(stage!=2);
    count=D(self+0x30);
    D(local)=tag;
    result=(result&0xffffff00u)|1;
    if ((s32)count>=1) {
        array=D(self+0xc);
        index=1;
        do {
            club=D(array+index*4);
            if (W(club+0x3a)==(u16)tag) goto done;
            result=(result&0xffffff00u)|(u8)(result+1);
            index=(u32)(s32)(s8)result;
        } while ((s32)index<=(s32)count);
    }
    if (create_arg==0) {
        result=(result&0xffffff00u)|0xfe;
        goto done;
    }
    count=(u32)(s32)(s16)W(self+0x3e);
    result=cm_fn_00944e46(count*4);
    array=result;
    if (array==0) {
        cm_fn_00944cff(0x00989d04,0,0,local+0x108,local+8);
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,local+0x108,local+8,0x378);
        result=cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        result=(result&0xffffff00u)|0xfe;
        goto done;
    }
    competition=competition_arg;
    if (competition==0x41f) owner=self;
    else {
        table=D(self+0xc);
        owner=D(table);
    }
    index=0;
    if ((s16)W(owner+0x3e)>0) {
        do {
            offset=(u32)(s32)(s16)index;
            table=D(owner+0xb1);
            ++index;
            value=D(table+offset*59);
            D(array+offset*4)=value;
        } while ((s16)index<(s16)W(owner+0x3e));
        tag=D(local);
    }
    count=D(self+0x30)+1;
    D(self+0x30)=count;
    allocation=cm_fn_00944cf1(0xee);
    D(local)=allocation;
    frame.state=0;
    if (allocation!=0) {
        slot=B(self+0x30);
        year=W(self+0x40);
        c4=B(self+0xc4);
        c3=B(self+0xc3);
        c2=B(self+0xc2);
        competition=D(self+4);
        count=W(self+0x3e);
        allocation=cm_fn_0068b100(allocation,self,(u16)count,array,1,competition,0,0,
                                  (u8)c2,(u8)c3,(u8)c4,self+0xc5,local+4,(u16)year,
                                  (u8)slot,(u16)tag,0,1,0,2,10,0,2);
    }
    count=D(self+0x30);
    table=D(self+0xc);
    frame.state=0xffffffffu;
    D(table+count*4)=allocation;
    cm_fn_009452ca(array);
    table=D(self+0xc);
    count=D(self+0x30);
    cm_fn_00945333(table+4,count,4,0x00424380);
    count=D(self+0x30);
    index=1;
    if ((s32)count>=1) {
        do {
            table=D(self+0xc);
            club=D(table+(u32)(s32)(s16)index*4);
            B(club+0x44)=(u8)index;
            count=D(self+0x30);
            ++index;
        } while ((s32)(s16)index<=(s32)count);
    }
    competition=competition_arg;
    stage=stage_arg;
    result=cm_fn_00422cf0(self,(u8)stage,(u16)competition,0);
done:
    CM_FS(volatile u32,0)=frame.prev;
    return result;
}
