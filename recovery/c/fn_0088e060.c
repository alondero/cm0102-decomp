#include "cmre.h"
/* Preferred-image VA0088e060, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch414: stage BYTE L3, heap L4, count/allocation L8,
   virtual outputs Lc/L10, diagnostics L14/L114 and L214/L314.
   Keep fixed64 constructor count and original error-path allocation leaks. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_THISCALL cm_fn_00522e00(u32,u32,s16,u32,s16,u32,u32,u16,u8,u8,u16,u8,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0051c410(u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);
typedef u32 (CM_THISCALL *rank_fn)(u32,u8,u32,u32,u32);

u32 CM_THISCALL cm_fn_0088e060(u32 self)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x414];
    u32 l=(u32)scratch,eax,group,index,offset,collected=0,array,row,value,parent,table,rank,rounds,flags,competition,name,extension,line,version;
    u16 year;
    u8 stage;
    frame.state=0xffffffff; frame.handler=0x009630ce;
    frame.prev=CM_FS(volatile u32,0);
    CM_FS(volatile u32,0)=(u32)&frame;
    D(l+8)=0;
    eax=cm_fn_00944e46(0x100);
    D(l+4)=eax;
    if (!eax) {
        name=l+0x14; extension=l+0x114; line=0x380;
        goto diagnostic;
    }
    stage=0xff;
    B(l+3)=stage;
    do {
        if (stage==0xff) group=self;
        else {
            parent=D(self+0xc);
            group=D(parent+(u32)(s32)(s8)stage*4);
        }
        index=0;
        if ((s16)W(group+0x3e)>0) {
            offset=0;
            do {
                row=D(group+0xa7)+offset;
                if (B(row+5)==1) {
                    if ((u16)collected==64) {
                        name=l+0x114; extension=l+0x14; line=0x393;
                        goto diagnostic;
                    }
                    value=D(row);
                    array=D(l+4);
                    D(array+(u32)(s32)(s16)collected*4)=value;
                    collected=D(l+8);
                    stage=B(l+3);
                    ++collected;
                    D(l+8)=collected;
                }
                value=(u32)(s32)(s16)W(group+0x3e);
                ++index; offset+=6;
            } while ((s32)index<(s32)value);
        }
        ++stage;
        B(l+3)=stage;
    } while ((s8)stage<7);
    table=D(self);
    rank=((rank_fn)D(table+0x3c))(self,7,l+0xc,l+0x10,0);
    eax=cm_fn_00944cf1(0xb2);
    D(l+8)=eax;
    frame.state=0;
    if (eax) {
        flags=D(l+0x10);
        year=W(self+0x40);
        competition=D(self+4);
        rounds=D(l+0xc);
        array=D(l+4);
        eax=cm_fn_00522e00(eax,self,64,array,(s16)rounds,competition,rank,year,7,1,(u16)flags,0x14,0,0,0,0);
    } else eax=0;
    parent=D(self+0xc);
    frame.state=0xffffffff;
    D(parent+0x1c)=eax;
    eax=cm_fn_009452ca(rank);
    parent=D(self+0xc);
    value=D(parent+0x1c);
    if (!value) {
        name=l+0x314; extension=l+0x214; line=0x3a8;
        goto diagnostic;
    }
    eax=cm_fn_0051c410(value,0);
    array=D(l+4);
    eax=cm_fn_009452ca(array);
    goto done;
diagnostic:
    cm_fn_00944cff(0x00a8b4a0,0,0,name,extension);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,name,extension,line);
    eax=cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
    D(0x00b67a34)=0;
done:
    CM_FS(volatile u32,0)=frame.prev;
    return eax;
}
