#include "cmre.h"

/* Preferred-image VA 005f6ad0, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch210: four constructor bytes L4..7, round byte L3, temporary L8,
   array slot Lc. Keep signed BYTE stage/WORD loops and full EAX exits.
   68b100 reads its narrow arguments as BYTE/WORD before using them. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
typedef struct { u32 prev,handler,state; } frame_5f6ad0;
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_006826d0(u32,u32);
extern u32 CM_THISCALL cm_fn_0068b100(u32,u32,u16,u32,u16,u32,u32,u16,u8,u8,u8,u32,u32,u16,u8,u16,u8,u8,u16,u16,u16,u32,u8);

u32 CM_THISCALL cm_fn_005f6ad0(u32 self,volatile u32 stage_arg)
{
    volatile frame_5f6ad0 frame;
    u8 scratch[0x210];
    u32 local=(u32)scratch,count,array,result,base,scan,club,stage_raw,stage,key;
    u32 allocation,round,year,c4,c3,c2,competition,table,offset,inner,descriptor,version;
    frame.state=0xffffffffu;
    frame.handler=0x0095ad9e;
    frame.prev=CM_FS(volatile u32,0);
    CM_FS(volatile u32,0)=(u32)&frame;
    B(local+4)=0;
    B(local+5)=1;
    B(local+6)=0;
    count=(u32)(s32)(s16)W(self+0x3e);
    B(local+7)=0;
    array=cm_fn_00944e46(count*4);
    D(local+0xc)=array;
    if (array==0) {
        cm_fn_00944cff(0x009cee08,0,0,local+0x110,local+0x10);
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,local+0x110,local+0x10,0x300);
        result=cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        goto done;
    }
    scan=0;
    if ((s16)W(self+0x3e)>0) {
        do {
            base=D(self+0xb1);
            offset=(u32)(s32)(s16)scan;
            ++scan;
            club=D(base+offset*59);
            D(array+offset*4)=club;
        } while ((s16)scan<(s16)W(self+0x3e));
    }
    stage_raw=stage_arg;
    if ((u8)stage_raw==0) key=0x439;
    else if ((u8)stage_raw==1) key=0x43a;
    else if ((u8)stage_raw==2) {
        key=0x43b;
        B(local+3)=10;
        goto allocate;
    } else key=0x43c;
    B(local+3)=8;
allocate:
    allocation=cm_fn_00944cf1(0xee);
    D(local+8)=allocation;
    frame.state=0;
    if (allocation!=0) {
        round=(u32)(s32)(s8)B(local+3);
        year=W(self+0x40);
        c4=B(self+0xc4);
        c3=B(self+0xc3);
        c2=B(self+0xc2);
        competition=D(self+4);
        count=W(self+0x3e);
        allocation=cm_fn_0068b100(allocation,self,(u16)count,array,1,competition,0,0,
                                  (u8)c2,(u8)c3,(u8)c4,self+0xc5,local+4,(u16)year,
                                  (u8)stage_raw,(u16)key,0,1,0,0x22,(u16)round,0,2);
    }
    table=D(self+0xc);
    frame.state=0xffffffffu;
    stage=(u32)(s32)(s8)stage_raw;
    D(table+stage*4)=allocation;
    count=W(self+0x3e);
    D(local+8)=0;
    if ((s16)count>0) {
        scan=0;
        do {
            offset=(u32)(s32)(s16)scan*59;
            base=D(self+0xb1);
            if (B(base+offset+0x37)==1 && (s16)count>0) {
                inner=0;
                do {
                    table=D(self+0xc);
                    competition=D(table+stage*4);
                    descriptor=cm_fn_006826d0(competition,(u32)(s32)(s16)inner);
                    base=D(self+0xb1);
                    club=D(descriptor);
                    if (club==D(base+offset)) {
                        table=D(self+0xc);
                        competition=D(table+stage*4);
                        descriptor=cm_fn_006826d0(competition,(u32)(s32)(s16)inner);
                        B(descriptor+0x37)=0xfe;
                        break;
                    }
                    ++inner;
                } while ((s16)inner<(s16)W(self+0x3e));
            }
            scan=D(local+8);
            count=W(self+0x3e);
            ++scan;
            D(local+8)=scan;
        } while ((s16)scan<(s16)count);
        array=D(local+0xc);
    }
    D(self+0x30)=stage;
    result=cm_fn_009452ca(array);
done:
    CM_FS(volatile u32,0)=frame.prev;
    return result;
}
