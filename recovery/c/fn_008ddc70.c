#include "cmre.h"
extern unsigned long long CM_THISCALL cm_fn_00785830(void *, u32);
extern u32 CM_CDECL cm_fn_0053e140(u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0053e990(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32,u32,...);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *,u32);
extern u32 CM_THISCALL cm_fn_004ae770(u32,u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32,const char *,u32,u32);

u32 CM_STDCALL cm_fn_008ddc70(void *p, u32 mode, u32 out)
{
    u32 person, a, b, c, x, y, v, fmt, base, first, second, r;
    u8 t, flag, other, kind;
    u8 scratch[0x13c];
    char *buf = (char *)(scratch + 0x10);
    person = (u32)cm_fn_00785830(p,2);
    a = (u32)cm_fn_00785830(p,3);
    b = (u32)cm_fn_00785830(p,4);
    c = (u32)cm_fn_00785830(p,5);
    if (!mode) {
        cm_fn_0053e140(a,b,c,(u32)buf,300,5,person);
        cm_fn_0066f4e0(0xde1f64,0xa98f64,100,buf);
        cm_fn_004ae660((void *)out,0xde1f64);
        base = CM_GLOBAL(volatile u32,0xae23bc);
        r = cm_fn_0053e990(base+person*110,a,b,c,0);
        if (!r) return r;
        base = CM_GLOBAL(volatile u32,0xae23bc);
        return cm_fn_004ae8a0(out,buf,0x7d7,base+person*110);
    }
    x = (u32)cm_fn_00785830(p,0);
    y = (u32)cm_fn_00785830(p,1);
    t = (u8)cm_fn_00785830(p,6);
    flag = (u8)cm_fn_00785830(p,7);
    other = (u8)cm_fn_00785830(p,8);
    kind = (u8)cm_fn_00785830(p,10);
    cm_fn_0053e140(a,b,c,(u32)buf,300,1,person);
    fmt = 0;
    if (t == 0x11) {
        if (!kind) fmt=0xa994a4;
        else if (kind & 5) fmt=0xa993b8;
        else if (kind & 0x23) fmt=0xa992ec;
        first=x; second=y;
    } else {
        if (!kind) fmt=0xa9924c;
        else if (kind==3) fmt=0xa9917c;
        else if (kind==4) fmt=0xa990a4;
        else if (kind==0x22) fmt=0xa98fd8;
        first=y; second=x;
    }
    if (fmt) {
        s32 f, s;
        base=CM_GLOBAL(volatile u32,0xae23b4);
        first=base+first*0x245;
        second=base+second*0x245;
        f=*(volatile s8 *)(first+0x52);
        s=*(volatile s8 *)(second+0x52);
        cm_fn_0066f4e0(0xde1f64,fmt,f,f,100,s,s,first+0x38,buf,second+0x38);
    }
    cm_fn_004ae660((void *)out,0xde1f64);
    if (flag) {
        s32 n;
        base=CM_GLOBAL(volatile u32,0xae23b4);
        v=base+(t==0x11?y:x)*0x245;
        n=*(volatile s8 *)(v+0x52);
        cm_fn_0066f4e0(0xde1f64,other?0xa98f98:0xa96bb4,n,n,v+0x38);
        cm_fn_004ae770(out,0xde1f64);
    }
    base=CM_GLOBAL(volatile u32,0xae23bc);
    r=cm_fn_0053e990(base+person*110,a,b,c,0);
    if (r) {
        base=CM_GLOBAL(volatile u32,0xae23bc);
        cm_fn_004ae8a0(out,buf,0x7d7,base+person*110);
    }
    base=CM_GLOBAL(volatile u32,0xae23b4);
    v=base+y*0x245;
    cm_fn_004ae8a0(out,(char *)(v+0x38),0x7d5,v);
    base=CM_GLOBAL(volatile u32,0xae23b4);
    v=base+x*0x245;
    return cm_fn_004ae8a0(out,(char *)(v+0x38),0x7d5,v);
}
