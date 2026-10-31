#include "cmre.h"

/* Preferred-image VA 00802a50, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Keep capacity errors which continue, partial AX port argument, late stage,
   per-slot player-index reloads and final EAX computed slot index. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_THISCALL cm_fn_0084cb10(u32);
extern u32 CM_THISCALL cm_fn_0084cb70(u32,u32);
extern u32 CM_THISCALL cm_fn_0077ac30(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

#define DIAG(path,name,extension,line) do { \
    cm_fn_00944cff((path),0,0,(name),(extension)); \
    version=cm_fn_0090d130(); \
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,(name),(extension),(line)); \
    cm_fn_005e8290(0x009870e0,0x00ae24d0,0); \
    D(0x00b67a34)=0; \
} while (0)

u32 CM_THISCALL cm_fn_00802a50(u32 self,volatile u32 stage_arg)
{
    u8 scratch[0x200];
    u32 local=(u32)scratch,index,stage,owner,offset,capacity,buffer,port,result,version,slot;
    index=(u32)(s32)(s16)W(self+0x382a);
    offset=index*768;
    if (W(self+offset+0x7d4)==0) {
        stage=stage_arg;
        owner=D(0x00dd7eec);
        if ((u8)stage==0xff) {
            if (owner!=0) cm_fn_0084cb10(owner);
        } else if (owner!=0) cm_fn_0084cb70(owner,stage);
    } else {
        offset=0;
        D(self+0x3802)=0;
        capacity=D(self+0x3806);
        if (offset+1>capacity) {
            DIAG(0x009c3734,local+0x100,local,0x85);
        } else {
            buffer=D(self+0x37fe);
            B(buffer+offset)=0xb;
            offset=D(self+0x3802)+1;
            D(self+0x3802)=offset;
        }
        offset=D(self+0x3802);
        capacity=D(self+0x3806);
        stage=stage_arg;
        if (offset+1>capacity) {
            DIAG(0x009c3734,local,local+0x100,0x85);
        } else {
            buffer=D(self+0x37fe);
            B(buffer+offset)=(u8)stage;
            offset=D(self+0x3802)+1;
            D(self+0x3802)=offset;
        }
        index=(u32)(s32)(s16)W(self+0x382a);
        offset=D(self+0x3802);
        buffer=D(self+0x37fe);
        port=index*768;
        port=(port&0xffff0000u)|W(self+port+0x7d4);
        owner=D(self+0x3840);
        result=cm_fn_0077ac30(owner,buffer,offset,port);
        if (result==0) {
            DIAG(0x00a7ddbc,local,local+0x100,0xbe3);
        }
    }
    if ((u8)stage==0xff) {
        slot=0;
        do {
            index=(u32)(s32)(s16)W(self+0x382a);
            result=slot+index*10;
            ++slot;
            D(self+result*4+0x26250a)=0;
        } while (slot!=10);
    } else {
        index=(u32)(s32)(s16)W(self+0x382a);
        result=(u32)(s32)(s8)stage+index*10;
        D(self+result*4+0x26250a)=0;
    }
    return result;
}
