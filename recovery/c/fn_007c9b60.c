#include "cmre.h"

/* Preferred-image VA 007c9b60, base 00400000; target SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Original callees remain explicit boundaries. */
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00682f70(u32,u32);
extern u32 CM_THISCALL cm_fn_0068b100(u32,u32,u16,u32,u16,u32,u32,u16,u8,u8,u8,u32,u32,u16,u8,u16,u8,u8,u16,u16,u16,u32,u8);
extern u32 CM_THISCALL cm_fn_0068ab70(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_006827d0(u32,u32);
extern u32 CM_CDECL cm_fn_009452ca(u32);
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
typedef u32 (CM_THISCALL *virtual_rows)(u32,u32,u32,u32,u32);
typedef u32 (CM_THISCALL *virtual_delete)(u32,u32);

u32 CM_THISCALL cm_fn_007c9b60(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x214];
    u32 table, object, eax, edx, ecx, offset, row, source, index, vtable;
    u32 selected;
    u8 count=0;
    frame.state=0xffffffffu;
    frame.handler=0x0095f7ba;
    frame.prev=CM_FS(u32,0);
    CM_FS(u32,0)=(u32)&frame;
    scratch[4]=0; scratch[5]=0; scratch[6]=2; scratch[7]=2;
    eax=cm_fn_00944e46(0x40);
    table=eax;
    if (!table) {
        cm_fn_00944cff(0x00a78598,0,0,scratch+0x114,scratch+0x14);
        eax=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,eax,scratch+0x114,scratch+0x14,0x277);
        eax=cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        CM_GLOBAL(volatile u32,0x00b67a34)=0;
        goto finish;
    }
    offset=0;
    do {
        ecx=D(self+0xb1); offset+=0x3b;
        ecx=D(ecx+offset-0x3b);
        index=(u32)(s32)(s8)count; count++;
        D(table+index*4)=ecx;
        edx=D(self+0xc); ecx=D(edx); edx=D(ecx+0xb1);
        index=(u32)(s32)(s8)count;
        edx=D(edx+offset-0x3b); count++;
        D(table+index*4)=edx;
        ecx=CM_GLOBAL(volatile u32,0x009cf814);
        edx=ecx*107u; ecx=CM_GLOBAL(volatile u32,0x00ae23d0); edx+=ecx;
        ecx=D(self+0xb1); ecx=D(ecx+offset-0x3b); D(ecx+0x60)=edx;
        ecx=CM_GLOBAL(volatile u32,0x009cf814);
        edx=ecx*107u; ecx=CM_GLOBAL(volatile u32,0x00ae23d0); edx+=ecx;
        ecx=D(self+0xc); ecx=D(ecx); ecx=D(ecx+0xb1);
        ecx=D(ecx+offset-0x3b); D(ecx+0x60)=edx;
    } while ((s32)offset<0xec);
    offset=0xec;
    do {
        edx=D(self+0xb1); offset+=0x3b;
        edx=D(edx+offset-0x3b);
        index=(u32)(s32)(s8)count; count++;
        D(table+index*4)=edx;
        ecx=D(self+0xc); edx=D(ecx); ecx=D(edx+0xb1);
        index=(u32)(s32)(s8)count;
        ecx=D(ecx+offset-0x3b); count++;
        D(table+index*4)=ecx;
        ecx=CM_GLOBAL(volatile u32,0x009cf818);
        edx=ecx*107u; ecx=CM_GLOBAL(volatile u32,0x00ae23d0); edx+=ecx;
        ecx=D(self+0xb1); ecx=D(ecx+offset-0x3b); D(ecx+0x60)=edx;
        ecx=CM_GLOBAL(volatile u32,0x009cf818);
        edx=ecx*107u; ecx=CM_GLOBAL(volatile u32,0x00ae23d0); edx+=ecx;
        ecx=D(self+0xc); ecx=D(ecx); ecx=D(ecx+0xb1);
        ecx=D(ecx+offset-0x3b); D(ecx+0x60)=edx;
    } while ((s32)offset<0x1d8);
    object=cm_fn_00944cf1(0xee);
    frame.state=0;
    if (object) {
        selected=cm_fn_00682f70(self,0x11170);
        /* The constructor consumes 22 stack dwords, with the first PUSH 1
           retained across the preceding ret-4 helper call. */
        ecx=W(self+0x40);
        edx=B(self+0xc4);
        eax=B(self+0xc3);
        index=B(self+0xc2);
        source=D(self+4);
        object=cm_fn_0068b100(object,self,16,table,2,source,0,0,
            (u8)index,(u8)eax,(u8)edx,self+0xc5,(u32)(scratch+4),
            (u16)ecx,1,0,0x14,1,0,3,0xe,selected,1);
    }
    ecx=D(self+0xc);
    frame.state=0xffffffffu;
    D(ecx+4)=object;
    eax=0;
    if ((s16)W(object+0x3e)>0) {
        offset=0; index=0;
        do {
            row=D(object+0xb1); row+=offset;
            ecx=D(row);
            source=cm_fn_0068ab70(self,ecx,0);
            if (!source) {
                edx=D(row); eax=D(self+0xc); ecx=D(eax);
                source=cm_fn_0068ab70(ecx,edx,0);
            }
            if (source) {
                eax=(u32)((s32)(s16)W(source+0x19)+1);
                eax=(u32)((s32)eax/2);
                W(row+0x19)=(u16)eax;
                eax=(u32)((s32)(s16)W(source+0x27)+1);
                edx=W(row+0x19);
                eax=(u32)((s32)eax/2); edx=(u16)(edx-eax);
                W(row+0x27)=(u16)eax; W(row+0x35)=(u16)edx;
                eax=B(source+0x12); B(row+0x12)=(u8)eax;
                edx=B(source+0x20); B(row+0x20)=(u8)edx;
                eax=B(source+0x2e); B(row+0x2e)=(u8)eax;
                edx=B(source+0x13); B(row+0x13)=(u8)edx;
                eax=B(source+0x21); B(row+0x21)=(u8)eax;
                edx=B(source+0x2f); B(row+0x2f)=(u8)edx;
                eax=B(source+0x14); B(row+0x14)=(u8)eax;
                edx=B(source+0x22); B(row+0x22)=(u8)edx;
                eax=B(source+0x30); B(row+0x30)=(u8)eax;
                edx=W(source+0x15); W(row+0x15)=(u16)edx;
                eax=W(source+0x23); W(row+0x23)=(u16)eax;
                edx=W(source+0x31); W(row+0x31)=(u16)edx;
                eax=W(source+0x17); W(row+0x17)=(u16)eax;
                edx=W(source+0x25); W(row+0x25)=(u16)edx;
                eax=W(source+0x33); W(row+0x33)=(u16)eax;
            }
            ecx=(u32)(s32)(s16)W(object+0x3e);
            index++; offset+=0x3b; eax=index;
        } while ((s32)index<(s32)ecx);
    }
    edx=D(self+0xc); eax=D(edx+4); D(eax+0xa3)=0x007ca450;
    ecx=D(self+0xc); ecx=D(ecx+4); eax=cm_fn_006827d0(ecx,0);
    vtable=D(self);
    D(self+0x30)=1;
    selected=((virtual_rows)D(vtable+0x3c))(self,1,(u32)(scratch+8),(u32)(scratch+12),0);
    eax=cm_fn_00944cf1(0xee);
    frame.state=1;
    if (eax) {
        edx=D((u32)(scratch+12)); ecx=W(self+0x40);
        u8 c4=B(self+0xc4); u8 c3=B(self+0xc3); u8 c2=B(self+0xc2);
        index=D((u32)(scratch+8));
        source=D(self+4);
        eax=cm_fn_0068b100(eax,self,8,table,2,source,selected,(u16)index,
            c2,c3,c4,self+0xc5,self+0xbe,(u16)ecx,1,(u16)edx,0x14,1,0,3,0xffff,0,2);
    }
    frame.state=0xffffffffu;
    if (eax) { edx=D(eax); eax=((virtual_delete)D(edx))(eax,1); }
    eax=cm_fn_009452ca(selected);
    vtable=D(self);
    selected=((virtual_rows)D(vtable+0x3c))(self,1,(u32)(scratch+8),(u32)(scratch+12),0);
    eax=cm_fn_00944cf1(0xee);
    frame.state=2;
    if (eax) {
        ecx=D((u32)(scratch+12)); edx=W(self+0x40);
        u8 c4=B(self+0xc4); u8 c3=B(self+0xc3); u8 c2=B(self+0xc2);
        index=D((u32)(scratch+8));
        source=D(self+4); table+=0x20;
        eax=cm_fn_0068b100(eax,self,8,table,2,source,selected,(u16)index,
            c2,c3,c4,self+0xc5,self+0xbe,(u16)edx,1,(u16)ecx,0x14,1,0,3,0xffff,0,2);
    }
    frame.state=0xffffffffu;
    if (eax) { edx=D(eax); eax=((virtual_delete)D(edx))(eax,1); }
finish:
    ecx=frame.prev;
    CM_FS(u32,0)=ecx;
    return eax;
}
