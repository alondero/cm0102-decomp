#include "cmre.h"
/* Preferred-image VA00713e90/base00400000; target SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS7/RET28. All four original callers consume AL only. */
extern u32 CM_THISCALL cm_fn_006cb6c0(u32,u8,u8,u8,u8);
#define B(a) CM_GLOBAL(volatile u8,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define D(a) CM_GLOBAL(volatile u32,(a))
#define F(a) CM_GLOBAL(volatile float,(a))

__attribute__((target("no-sse3")))
u8 CM_THISCALL cm_fn_00713e90(u32 self,volatile u32 mode_arg,
    volatile u32 x1_arg,volatile u32 y1_arg,volatile u32 x2_arg,
    volatile u32 y2_arg,volatile float speed_arg,volatile u32 out_arg)
{
    u32 out=out_arg,mode=(u16)mode_arg,a,b,c,d,state,bearing,person;
    u16 delta,magnitude,direction,threshold;
    s32 sign,attribute;
    volatile long double live;
    volatile long long converted;
    W(out)=0;
    switch(mode) {
    case 0x5b:case 0x5c:case 0x3d:case 0x36:case 0x34:case 0x37:
    case 0x5a:case 0x33:case 0x35:case 0x39:case 3:return 2;
    }
    /* Original formal reads: argument5,4,3,2, then the receiver DWORD. */
    d=y2_arg;c=x2_arg;b=y1_arg;a=x1_arg;state=D(self+0x19e);
    bearing=cm_fn_006cb6c0(state,(u8)a,(u8)b,(u8)c,(u8)d);
    delta=(u16)((u16)bearing-W(self+0x19a));
    sign=(s16)delta>=0?1:-1;
    /* Only the low WORDs of native partial ECX/EDX products are live. */
    magnitude=(u16)((u32)sign*(u32)delta);
    if((s16)magnitude>180)magnitude=(u16)(360u-magnitude);
    direction=(u16)((u32)sign*(u32)magnitude);
    live=(long double)speed_arg;
    live=live*(long double)F(0x0096d84c);
    live=live-(long double)F(self+0xe5);
    live=(long double)F(0x0096d848)-live;
    converted=(long long)live; /* Native FTOL00713f78; ADR0006. */
    threshold=(u16)(u32)converted;
    if((s16)direction>(s16)threshold) {
        person=D(self+0x6d);attribute=(s8)B(person+0x30);
        W(out)=(u16)((20-attribute)*12);
        return 0;
    }
    if((s16)direction<(s16)(u16)(0u-(u32)threshold)) {
        person=D(self+0x6d);attribute=(s8)B(person+0x3b);
        W(out)=(u16)((20-attribute)*12);
        return 1;
    }
    return B(self+0x2c);
}
