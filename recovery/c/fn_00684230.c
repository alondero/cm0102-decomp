#include "cmre.h"
/* Preferred-image VA 00684230, base 00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch34: outer BYTE L2, next field BYTE L3, stride L4, allocation L8,
   dates Lc/L14/L1c and copy temporaries L24/L2c. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_THISCALL cm_fn_007875e0(u32,u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_00944c94(u32);
extern u32 CM_THISCALL cm_fn_0077bbe0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00549ef0(u32,u16,u16);
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054a2d0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00785790(u32,u8,u32);
extern u32 CM_THISCALL cm_fn_007861e0(u32,u32,u32);

u32 CM_THISCALL cm_fn_00684230(u32 self)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x34];
    u32 l=(u32)scratch,eax,object=0,base,row,stride,argument,value;
    u16 count,year;
    u8 outer=0,index,field;
    frame.prev=CM_FS(volatile u32,0);
    frame.handler=0x0095c9f3; frame.state=0xffffffff;
    CM_FS(volatile u32,0)=(u32)&frame;
    count=W(self+0x3e);
    eax=(frame.prev&0xffff0000)|count;
    if ((s16)count>8) goto done;
    B(l+2)=0;
    if ((s16)count<=0) goto done;
    do {
        stride=(u32)(s32)(s8)outer*59;
        base=D(self+0xb1);
        D(l+4)=stride;
        argument=D(base+stride);
        base=D(0x00b7431c);
        eax=cm_fn_007875e0(base,argument);
        if (eax!=0) {
            if (object==0) {
                eax=cm_fn_00944cf1(0xdf);
                D(l+8)=eax;
                frame.state=object;
                if (eax) object=cm_fn_0077bbe0(eax,0x15,object);
                else object=0;
                base=D(self+0xba);
                frame.state=0xffffffff;
                year=W(base+2);
                value=W(base);
                year=(u16)(year+W(self+0x40));
                eax=cm_fn_00549ef0(l+0x1c,(u16)value,year);
                frame.state=1;
                eax=cm_fn_0054a110(l+0xc);
                *(volatile u8 *)&frame.state=2;
                eax=cm_fn_0054a110(l+0x14);
                *(volatile u8 *)&frame.state=3;
                eax=cm_fn_00417c10(l+0xc,l+0x24,l+0x1c);
                index=0;
                do {
                    base=D(self+0xba);
                    row=base+(u32)(s32)(s8)index*7;
                    if (B(row+0xb)==0xff) break;
                    argument=(base&0xffff0000)|(u16)(s16)(s8)B(row+9);
                    eax=cm_fn_0054a2d0(l+0x1c,argument,l+0x14);
                    eax=D(l+0xe);
                    year=W(l+0x16);
                    if ((u16)eax>year || ((u16)eax==year && (s16)W(l+0xc)>(s16)W(l+0x14)))
                        eax=cm_fn_00417c10(l+0xc,l+0x2c,l+0x14);
                    ++index;
                } while ((s8)index<8);
                base=D(self+4); value=D(base);
                eax=cm_fn_00785790(object,0,value);
                value=(u32)(s32)(s16)W(self+0x3a);
                eax=cm_fn_00785790(object,1,value);
                value=(u32)(s32)(s16)W(l+0xc);
                eax=cm_fn_00785790(object,3,value);
                value=D(l+0xe)&0xffff;
                eax=cm_fn_00785790(object,4,value);
                value=(u32)(s32)(s16)W(self+0x3e)-1;
                eax=cm_fn_00785790(object,5,value);
                stride=D(l+4);
                frame.state=0xffffffff;
            }
            base=D(self+0xb1); row=D(base+stride); value=D(row);
            eax=cm_fn_00785790(object,2,value);
            index=0;
            count=W(self+0x3e);
            field=6;
            eax=(eax&0xffffff00)|6;
            if ((s16)count>0) {
                do {
                    if (index!=B(l+2)) {
                        u8 selector=field;
                        ++field; B(l+3)=field;
                        row=(u32)(s32)(s8)index*59;
                        base=D(self+0xb1); row=D(row+base); value=D(row);
                        eax=cm_fn_00785790(object,selector,value);
                        field=B(l+3);
                        eax=(eax&0xffffff00)|field;
                    }
                    ++index;
                } while ((s16)(s8)index<(s16)W(self+0x3e));
            }
            base=D(self+0xb1);
            argument=D(0x00b7431c);
            value=D(base+stride);
            eax=cm_fn_007861e0(argument,object,value);
            outer=B(l+2);
        }
        ++outer;
        count=W(self+0x3e);
        B(l+2)=outer;
    } while ((s16)(s8)outer<(s16)count);
    if (object) eax=cm_fn_00944c94(object);
done:
    CM_FS(volatile u32,0)=frame.prev;
    return eax;
}
