#include "cmre.h"
extern u32 CM_THISCALL cm_fn_004cc9e0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);
extern int CM_FASTCALL cm_fn_0054cfb0(short *);
extern u32 cm_fn_005e8290(const char *,const char *,int);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *,u32);
extern u32 CM_THISCALL cm_fn_00786b70(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007fe600(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007ff270(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_008c61e0(void *,u32);
extern u32 CM_THISCALL cm_fn_008d0590(u32,u32,u32,u32,u32,u32,u32,u32);
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
#define M32(a) (*(volatile u32 *)(a))
#define M8(a) (*(volatile u8 *)(a))
#define SET(i,v) (r=cm_fn_007ff270(0xb74450,(i),(u32)(v),0))
u32 cm_fn_008f1c30(u32 p1,u32 p2)
{
    volatile struct { u32 prev,handler,state; } frame;
    u8 local[0x2f0];
    u32 a,b,obj,rec,first,other,base,x,r,ratio;
    s32 q;
    frame.state=~0u;
    frame.handler=0x964d3b;
    frame.prev=CM_FS(u32,0);
    CM_FS(u32,0)=(u32)&frame;
    cm_fn_0077bbe0(local+0x10,0,0);
    frame.state=0;
    cm_fn_00786b70(CM_GLOBAL(volatile u32,0xb7431c),p1,p2,(u32)(local+0x10));
    a=(u32)cm_fn_00785830(local+0x10,6);
    b=(u32)cm_fn_00785830(local+0x10,7);
    x=cm_fn_008c61e0((void *)CM_GLOBAL(volatile u32,0xde1f54),a);
    obj=M32(x);
    if (M32(obj+0x24)==b && M8(obj+0x2c)!=3) {
        rec=CM_GLOBAL(volatile u32,0xae23bc)+M32(obj+0xc)*110u;
        r=cm_fn_007fe600(0xb74450,0x8f9a20,0x8fac30,1,0,0);
        if (!r) goto done;
        SET(0,rec);
        first=cm_fn_004cf5a0(CM_GLOBAL(volatile u32,0xae1050),rec);
        x=M32(obj+8);
        a=M32(rec);
        other=cm_fn_004cc9e0(CM_GLOBAL(volatile u32,0xae1050),a,x);
        if (!other) {
            cm_fn_00944cff(0xa9e98c,0,0,local+0x1f0,local+0xf0);
            x=(u32)cm_fn_0090d130();
            cm_fn_00944c9f(0xae24d0,0x9870e8,x,local+0x1f0,local+0xf0,0x237f);
            r=cm_fn_005e8290((const char *)0x9870e0,(const char *)0xae24d0,0);
            CM_GLOBAL(volatile u32,0xb67a34)=0;
            goto done;
        }
        q=(s32)(M32(other+12)*100u)/(s32)M32(first+12);
        ratio=(u32)(((s32)((u32)q+5u))/10)*10u;
        SET(1,M32(first+12));
        SET(2,M32(first+12));
        a=cm_fn_004cf5a0(CM_GLOBAL(volatile u32,0xae1050),rec);
        base=CM_GLOBAL(volatile u32,0xae23b4);
        CM_GLOBAL(volatile u32,0xae1080)=a;
        b=a ? base+M32(a+4)*0x245u : 0;
        x=M32(obj+8);
        cm_fn_008d0590(CM_GLOBAL(volatile u32,0xde1f54),base+x*0x245u,b,rec,(u32)(local+3),(u32)(local+8),(u32)(local+2),(u32)(local+12));
        SET(3,(s32)*(s8 *)(local+3));
        SET(4,M32((u32)(local+8))&0xffff);
        SET(5,(s32)*(s8 *)(local+2));
        SET(6,M32((u32)(local+12))&0xffff);
        SET(7,obj);
        SET(8,other);
        SET(9,M32(obj+0x24));
        SET(10,0);
        SET(11,ratio);
        a=cm_fn_0054cfb0((short *)(other+0x2d));
        SET(12,(s32)(s8)a);
        SET(13,*(volatile u16 *)(other+0x2f));
        SET(14,(s32)*(volatile s8 *)(other+0x1c));
        SET(15,(s32)*(volatile s8 *)(other+0x1d));
        SET(16,ratio);
        a=cm_fn_0054cfb0((short *)(other+0x2d));
        SET(17,(s32)(s8)a);
        SET(18,*(volatile u16 *)(other+0x2f));
        SET(19,(s32)*(volatile s8 *)(other+0x1c));
        SET(20,(s32)*(volatile s8 *)(other+0x1d));
    } else {
        r=cm_fn_007fe600(0xb74450,0x8f24c0,0x8f2800,1,0,0);
        if (!r) goto done;
        SET(0,obj);
        SET(1,b);
    }
done:
    CM_FS(u32,0)=frame.prev;
    return r;
}
