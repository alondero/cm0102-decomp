#include "cmre.h"
extern u32 CM_THISCALL cm_fn_004178a0(u32,u32,u32,float);
extern void CM_THISCALL cm_fn_00417a20(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00418320(u32);
extern void CM_CDECL cm_fn_005e8290(const char *,const char *,int);
extern char *CM_CDECL cm_fn_0090d130(void);
extern void CM_THISCALL cm_fn_009302f0(u32,u16 *,int);
extern u16 CM_THISCALL cm_fn_00931ae0(u32,u32,s16,s32,s32,s32,s32,s32,u8,s8);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
#define R32(a) (*(volatile u32 *)(u32)(a))
#define R16(a) (*(volatile s16 *)(u32)(a))
#define R8(a) (*(volatile s8 *)(u32)(a))
void CM_THISCALL cm_fn_007683c0(u32 self,u32 p2,u32 p3,u32 p4,u32 p5,u32 p6,u32 p7,u32 p8,u32 p9,u32 p10,u32 p11)
{
    u8 scratch[0x260];
    u32 obj=R32(p3+0x1f), mem, i, base, rec, x, a,b,c,d;
    s16 count=3;
    s8 idx;
    if(obj) count=R16(obj+6);
    mem=(u32)cm_fn_00944e46((u32)(s32)count*4);
    if(!mem || !p4) {
        char *t;
        if(!mem) cm_fn_00944cff(0xa5b26c,0,0,scratch+0x160,scratch+0x60);
        else cm_fn_00944cff(0xa5b26c,0,0,scratch+0x60,scratch+0x160);
        t=cm_fn_0090d130();
        if(!mem) cm_fn_00944c9f(0xae24d0,0x9870e8,t,scratch+0x160,scratch+0x60,0xab);
        else cm_fn_00944c9f(0xae24d0,0x9870e8,t,scratch+0x60,scratch+0x160,0xb2);
        cm_fn_005e8290((char *)0x9870e0,(char *)0xae24d0,0);
        CM_GLOBAL(u32,0xb67a34)=0;
        return;
    }
    if((s16)cm_fn_00931ae0(p4,mem,count,p5,p6,p7,p8,p9,(u8)p10,(s8)p11)>=count) {
        if(obj) {
            cm_fn_00418320(obj);
            for(i=0;i<(u32)(s32)count && count>0;i++) {
                s32 numerator,denominator;
                float weight;
                rec=R32(mem+i*4);
                x=R32(rec);
                base=CM_GLOBAL(volatile u32,0xae23bc)+x*110;
                d=R32(base+0x39);
                if((u8)p10==0) numerator=R16(rec+8);
                else if((u8)p10==1) numerator=R16(rec+10);
                else if((u8)p10==2) numerator=R16(rec+12);
                else numerator=R16(rec+15)*2+R16(rec+17);
                denominator=R8(rec+14);
                weight=(float)((long double)numerator/(long double)denominator);
                cm_fn_004178a0(obj,base,d,weight);
            }
            a=R32(p3+0x11);
            b=R32(self+0x13);
            cm_fn_00417a20(obj,p2,b,a);
        } else {
            *(u16 *)(scratch+0x14)=(u16)R16(p2+2);
            idx=0;
            if(count>0) {
                base=CM_GLOBAL(volatile u32,0xae23bc);
                do {
                    rec=R32(mem+(s32)idx*4);
                    x=R32(rec);
                    rec=base+x*110;
                    d=R32(rec+0x39);
                    d=d?R32(d):0xffffffff;
                    a=R32(rec+4); a=a?R32(a+0x33):0xffffffff;
                    b=R32(rec+8); b=b?R32(b+0x33):0xffffffff;
                    c=R32(rec+12); c=c?R32(c+0x33):0xffffffff;
                    x=idx==0?0x16:idx==1?0x2e:0x46;
                    *(u32 *)(scratch+x)=a;
                    *(u32 *)(scratch+x+4)=b;
                    *(u32 *)(scratch+x+8)=c;
                    *(u32 *)(scratch+x+0x14)=d;
                    *(u32 *)(scratch+x+0xc)=R32(rec);
                    idx=(s8)(idx+1);
                } while(idx<count);
            }
            cm_fn_009302f0(p7,(u16 *)(scratch+0x14),1);
        }
    }
    cm_fn_009452ca(mem);
}
