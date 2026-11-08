/* Preferred-image VA0051C410, base00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Original callees remain execution boundaries. Packed fields are deliberately
   reloaded after calls and overlapping stores. No FP operations in this body. */
#include "cmre.h"
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))
extern void CM_THISCALL cm_fn_0051d2d0(u32 self,u32 index);
extern void CM_THISCALL cm_fn_0051d9f0(u32 self,u32 index);
extern void CM_THISCALL cm_fn_0051e220(u32 self,u32 index);
extern void CM_THISCALL cm_fn_0051e870(u32 self,u32 index,u32 mode);
extern void CM_THISCALL cm_fn_00522610(u32 self,u32 index);
extern u32 CM_THISCALL cm_fn_00549ef0(void *self,u32 first,u32 second);
extern u32 CM_THISCALL cm_fn_0054c5b0(void *self,void *out,u32 count);
extern u32 CM_THISCALL cm_fn_00417c10(void *self,void *out,u32 source);
extern u32 CM_STDCALL cm_fn_0076d940(u32 club);
extern u32 CM_THISCALL cm_fn_005c6710(u32 self,u32 club);
extern void CM_THISCALL cm_fn_0077bbe0(void *self,u32 kind,u32 value);
extern void CM_THISCALL cm_fn_00785790(void *self,u32 slot,u32 value);
extern void CM_THISCALL cm_fn_007861e0(u32 self,void *message,u32 club);
extern u32 CM_THISCALL cm_fn_007875e0(u32 self,u32 club);

u32 CM_THISCALL cm_fn_0051c410(u32 self,u32 index)
{
    volatile struct { u32 prev,handler,state; } seh;
    /* Original S+10..S+107: two dates, a copy result, then message.
       Keep their original adjacency; original S is ESP after saved registers. */
    u8 scratch[0xf8];
    u32 date=(u32)scratch, next=date+8u, copy=date+16u, message=date+24u;
    u32 rows, record, cursor, offset, i, value, target;
    u16 flags;
    seh.prev=CM_FS(u32,0); seh.handler=0x009576b6u; seh.state=0xffffffffu;
    CM_FS(u32,0)=(u32)&seh;
    if (!D(self+0x45u)) goto done;
    cursor=D(D(self+0xadu)+index*4u);
    if (B(cursor+0x34u)) goto done;
    rows=D(self+0xa3u); offset=index*104u; record=rows+offset;
    D(date)=0;
    if ((s16)W(record+0x1au)>0) {
        do {
            value=W(cursor);
            if ((u16)value!=0xffffu && !D(D(self+0xa7u)+(u32)(s32)(s16)value*6u)) goto missing;
            value=W(cursor+2u);
            if ((u16)value!=0xffffu && !D(D(self+0xa7u)+(u32)(s32)(s16)value*6u)) goto missing;
            cursor+=0x35u;
            i=D(date); target=(u32)(s32)(s16)W(record+0x1au);
            ++i; D(date)=i;
        } while ((s32)i<(s32)target);
    }
    flags=W(record+0xdu);
    if (flags&0x400u) cm_fn_0051d2d0(self,index);
    else if (B(record+0xbu)&4u) cm_fn_0051d9f0(self,index);
    else if (!(flags&0x200u)) cm_fn_0051e220(self,index);
    cm_fn_0051e870(self,index,0xffffffffu);
    if (B(D(self+0xa3u)+offset+0x21u)==2u) cm_fn_00522610(self,index);
    i=0;
    if ((s16)W(D(self+0xa3u)+offset+0x1au)>0) {
        cursor=0;
        do {
            target=D(self+0xadu); ++i; cursor+=0x35u;
            target=D(target+index*4u); B(target+cursor-1u)=1;
        } while ((s32)i<(s32)(s16)W(D(self+0xa3u)+offset+0x1au));
    }
    record=D(self+0xa3u)+offset;
    if ((s16)W(record+0x1au)<2) goto done;
    if (W(record+0xdu)&0x200u) goto done;
    value=D(0x00b74318u);
    if (value && cm_fn_0076d940(D(self+4u))) goto done;
    value=D(0x00ae2c74u);
    if (value && cm_fn_005c6710(value,D(self+4u))) goto done;
    rows=D(self+0xa3u);
    if (W(rows+offset+7u)!=0x8cu) B(self+0xacu)=(u8)(B(self+0xacu)+1u);
    value=0;
    if ((s32)index>=0) {
        rows+=0x1cu; i=index+1u;
        do { value=(u16)(value+W(rows)); rows+=0x68u; --i; } while (i);
    }
    cm_fn_0077bbe0((void *)message,0x27u,0);
    target=D(self+4u);
    seh.state=1;
    target=D(target);
    cm_fn_00785790((void *)message,0,target);
    cm_fn_00785790((void *)message,1,(u32)(s32)(s16)W(self+0x3au));
    cm_fn_00785790((void *)message,2,(u32)(s32)(s16)W(D(self+0xa3u)+offset+7u));
    cm_fn_00785790((void *)message,4,(u32)(s32)(s8)B(self+0x44u));
    cm_fn_00785790((void *)message,5,index);
    cm_fn_00785790((void *)message,6,0);
    i=(u32)(s32)(s8)B(self+0xabu); target=(u32)(s32)(s8)B(self+0xacu);
    cm_fn_00785790((void *)message,7,(s32)(i+1u)<(s32)target);
    i=(u32)(s32)(s16)value;
    if ((s32)i<=0) goto done;
    offset=0;
    do {
        rows=D(self+0xa7u);
        if (!B(rows+offset+5u)) {
            target=D(rows+offset);
            if (target && cm_fn_007875e0(D(0x00b7431cu),target)) {
                target=D(D(self+0xa7u)+offset); target=D(target);
                cm_fn_00785790((void *)message,3,target);
                target=D(D(self+0xa7u)+offset);
                cm_fn_007861e0(D(0x00b7431cu),(void *)message,target);
            }
        }
        offset+=6u; --i;
    } while (i);
    goto done;
missing:
    value=(u16)((s16)(s8)B(record+2u)+W(self+0x40u));
    cm_fn_00549ef0((void *)date,W(record),value);
    seh.state=0;
    target=cm_fn_0054c5b0((void *)date,(void *)next,1);
    cm_fn_00417c10((void *)date,(void *)copy,target);
    rows=D(self+0xa3u); value=W(date); W(rows+offset)=(u16)value;
    value=B(date+2u); target=B(self+0x40u); rows=D(self+0xa3u);
    ++index; B(rows+offset+2u)=(u8)(value-target);
    if ((s16)index>=(s16)W(self+0x3cu)) goto done;
    do {
        i=(u32)(s32)(s16)index; rows=D(self+0xa3u); offset=i*104u;
        value=(u16)((s16)(s8)B(rows+offset+2u)+W(self+0x40u));
        cm_fn_00549ef0((void *)next,W(rows+offset),value);
        value=W(date+2u); target=W(next+2u); rows=D(date);
        if (value>target || (value==target && (s16)rows>(s16)W(next))) {
            W(D(self+0xa3u)+offset)=(u16)rows;
            value=B(date+2u); target=B(self+0x40u); rows=D(self+0xa3u);
            B(rows+offset+2u)=(u8)(value-target);
        }
        ++index;
    } while ((s16)index<(s16)W(self+0x3cu));
done:
    CM_FS(u32,0)=seh.prev;
    return 1;
}
