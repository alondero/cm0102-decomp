#include "cmre.h"
/* Preferred-image VA005c3be0, primary SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Local offsets are relative to L, immediately after SUB ESP,240h. */
extern u32 CM_THISCALL cm_fn_0054c5b0(void *,void *,u32);
extern u32 CM_THISCALL cm_fn_0054c8e0(void *,void *,u32);
extern u32 CM_THISCALL cm_fn_00417c10(void *,void *,void *);
extern u32 CM_THISCALL cm_fn_0054a3f0(void *);
extern u32 *CM_THISCALL cm_fn_005c6e00(void *,u32,u8,u32,u8,u8,u16,u16,u16,u32 *,char *);
extern void CM_THISCALL cm_fn_005c5310(void *,void *);
extern u32 CM_CDECL cm_fn_0066f4e0(u32,u32);
extern void CM_CDECL cm_fn_005e8290(u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_THISCALL cm_fn_0068d020(u32);
extern unsigned long long CM_FASTCALL cm_fn_0068d0d0(void *,u32,void *,s16);
#define R32(a) (*(volatile u32 *)(u32)(a))
#define R16(a) (*(volatile u16 *)(u32)(a))
#define R8(a) (*(volatile u8 *)(u32)(a))
u32 CM_THISCALL cm_fn_005c3be0(void *self,u32 entries,u32 count_in,u32 type_in)
{
    volatile struct {u32 prev,handler,state;} frame;
    volatile u8 local[0x240];
    s16 maximum=0,minimum=365;
    s8 count=(s8)count_in,type=(s8)type_in;
    u32 p,i,r,base,first,result,delta;
    frame.state=0xffffffffu;frame.handler=0x00959f5f;
    frame.prev=CM_FS(u32,0);CM_FS(u32,0)=(u32)&frame;
    R32(local+0x10)=(u32)self;
    if(count>0) {
        p=entries;i=(u32)(s32)count;
        do {
            r=R32(p);delta=R16(r+0xe);
            if((u16)delta!=0xffffu) {
                if((s16)delta>maximum)maximum=(s16)delta;
                delta=R16(r+0x10);
                if((s16)delta<minimum)minimum=(s16)delta;
            }
            p+=4;--i;
        }while(i);
    }
    R32(local+0x14)=(u32)(s32)type;
    if((u32)(s32)type<=11u) {
        r=R32(0x005c3fd4u+(u32)(s32)type*4u);(void)r;
        delta=(type==1||type==10)?2u:type==2?3u:(type==0||type==3||type==4||type==11)?1u:0u;
    }else delta=0;
    if(!delta) {
        cm_fn_00944cff(0x009cd0a0,0,0,(void *)(local+0x140),(void *)(local+0x40));
        r=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,r,(u32)(local+0x140),(u32)(local+0x40),0xf18);
        cm_fn_005e8290(0x009870e0,0x00ae24d0,0);R32(0x00b67a34)=0;delta=1;
    }
    minimum=(s16)((u16)minimum-(u16)delta);
    if(maximum>=minimum) {result=0;goto done;}
    cm_fn_0054c5b0((void *)((u32)self+0xa7),(void *)local,(u32)(s32)minimum);
    frame.state=0;
    if((s32)minimum-(s32)maximum>6) {
        r=cm_fn_0054a3f0((void *)local);
        while((u16)r!=5) {
            r=cm_fn_0054c8e0((void *)local,(void *)(local+0x30),1);
            cm_fn_00417c10((void *)local,(void *)(local+0x38),(void *)r);
            r=cm_fn_0054a3f0((void *)local);
        }
    }
    p=entries;base=R32(0x00ae23b4);r=R32(p);r=R32(r);first=base+r*0x245u;
    if(count>0) {
        i=0;
        do {r=R32(entries+i*4u);r=R32(r);R32(local+0x20+i*4u)=base+r*0x245u;++i;}while(i<(u32)(s32)count);
        self=(void *)R32(local+0x10);
    }
    cm_fn_0066f4e0(0x00de1f64,type==1?0x009cd0e0:0x009cd0d0);
    R8(0x00de1f7c)=0;
    r=(u32)cm_fn_005c6e00(self,first,type==1?1:(u8)type_in,(u32)local,type==1?0:3,1,0,type==1?1:0,(u16)(s16)count,(u32 *)(local+0x20),(char *)0x00de1f64);
    cm_fn_005c5310(self,(void *)r);
    if(count>0) {
        p=entries;i=(u32)(s32)count;
        do {
            r=R32(p);r=R32(r+0x12);
            if(!r) {
                r=(u32)cm_fn_00944cf1(14);R32(local+0x10)=r;
                frame.state=(frame.state&0xffffff00u)|1u;
                if(r)r=cm_fn_0068d020(r);
                base=R32(p);frame.state&=0xffffff00u;R32(base+0x12)=r;
            }
            r=R32(p);delta=R8(r+4);delta=(u8)(delta+(type==1?2:3));
            p+=4;--i;R8(r+4)=(u8)delta;
        }while(i);
    }
    r=R32(local+0x14);
    if(r<=11u) {
        base=R32(0x005c4004u+r*4u);(void)base;
        delta=(r==1||r==10)?2u:r==2?3u:(r==0||r==3||r==4||r==11)?1u:0u;
    }else delta=0;
    if(!delta) {
        cm_fn_00944cff(0x009cd0a0,0,0,(void *)(local+0x40),(void *)(local+0x140));
        r=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,r,(u32)(local+0x40),(u32)(local+0x140),0xf18);
        cm_fn_005e8290(0x009870e0,0x00ae24d0,0);R32(0x00b67a34)=0;delta=1;
    }
    cm_fn_0054c5b0((void *)local,(void *)(local+0x18),delta);
    r=R32(local);base=R32(local+4);R32(local+8)=r;R32(local+0xc)=base;
    frame.state=(frame.state&0xffffff00u)|3u;
    for(;;) {
        u16 endyear=R16(local+0x1a),year=R16(local+0xa);
        if(endyear<year)break;
        if(endyear==year) {
            s16 day=(s16)R16(local+8);
            if((s16)R16(local+0x18)<=day)break;
        }
        if(count>0) {
            p=entries;i=(u32)(s32)count;
            do {r=R32(p);r=R32(r+0x12);cm_fn_0068d0d0((void *)r,(u32)(local+8),(void *)(local+8),8);p+=4;--i;}while(i);
        }
        r=cm_fn_0054c5b0((void *)(local+8),(void *)(local+0x38),1);
        cm_fn_00417c10((void *)(local+8),(void *)(local+0x30),(void *)r);
    }
    result=1;
done:
    CM_FS(u32,0)=frame.prev;return result;
}
