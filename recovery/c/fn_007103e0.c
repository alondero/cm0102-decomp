#include "cmre.h"
/* Preferred-image VA007103e0/base00400000, primary SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/RET0. All five native callers and their forwarding continuations
   discard return bits above AL; incidental FNSTSW AX is not an engine result. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
#define F(a) CM_GLOBAL(volatile float,(a))
#define Q(a) CM_GLOBAL(volatile double,(a))
extern u8 CM_THISCALL cm_fn_006bce20(u32,u32,u32,u32);
extern u16 CM_THISCALL cm_fn_006c38c0(u32,u32);
/* Complete native006cb960 uses only its three stack BYTEs, never ECX. */
extern u8 CM_STDCALL cm_fn_006cb960(u32,u32,u32);
extern long double CM_THISCALL cm_fn_006f09b0(u32,u32,u32);
extern void CM_THISCALL cm_fn_007109f0(u32,u32);
extern u8 CM_THISCALL cm_fn_00713fc0(u32,u32);
extern u8 CM_THISCALL cm_fn_00714d40(u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);

static u8 children(u32 self,u32 order)
{
    u8 count=B(self+0x101),i=0;
    if((s8)count<=0)return 0;
    do {
        u8 x,y;
        u32 slot=self+(u32)(s32)(s8)i*4u+0x1ae,object,state;
        if(order==2) { x=B(self+0x102);y=B(self+0x103); }
        else { y=B(self+0x103);x=B(self+0x102); }
        if(order==0) { object=D(slot);state=D(self+0x19e); }
        else { state=D(self+0x19e);object=D(slot); }
        if(cm_fn_006bce20(state,object,x,y)!=0) {
            u8 a=B(self+0x107),b=B(self+0x104);
            u32 n=cm_fn_0090cfc0(900)+50u;
            if((s32)(s8)a*(s32)(s8)b<=(s32)n)
                (void)cm_fn_00714d40(D(slot));
        }
        count=B(self+0x101);
        i=(u8)(i+1u);
    }while((s8)i<(s8)count);
    return i;
}

static u8 eligible(u32 self,u32 state)
{
    u8 side=B(self+0x27),v=B(state+0x8eae);
    u32 other;
    if((s32)(s8)v==(side==0))return 1;
    if(side==v)return 0;
    other=D(state+0xf57a);
    if(other!=0 && B(other+0x27)==side)return 0;
    {
        u8 y=B(state+0x8ea8),x=B(state+0x8ea7);
        if(cm_fn_006cb960(side==0,x,y)!=0 && B(state+0x8ea9)!=255)
            return 0;
    }
    return 1;
}

static s8 absolute_byte(s32 x)
{
    u32 a=(u32)x,sign=(u32)(x>>31);
    return (s8)(u8)((a^sign)-sign);
}

__attribute__((target("no-sse3")))
u8 CM_THISCALL cm_fn_007103e0(u32 self)
{
    for(;;) {
        u32 state=D(self+0x19e);
        u8 flag;
        if(self!=D(state+0xf582)) {
            flag=state!=0?(u8)cm_fn_006c38c0(state,self):0;
            if((flag&1)==0) {
                volatile float saved=F(self+0x4d);
                s32 first=(s32)cm_fn_0090cfc0(65);
                volatile double value=(double)(long double)first;
                s32 second=(s32)cm_fn_0090cfc0(10);
                long double sum=(long double)saved*(long double)Q(0x96d818);
                sum=(long double)second+sum;
                if(!(sum>(long double)value)) {
                    u8 result=children(self,0);
                    W(self+0x198)=0x67;
                    return result;
                }
            }
        }
        state=D(self+0x19e);
        if(self!=D(state+0xf582)) {
            flag=state!=0?(u8)cm_fn_006c38c0(state,self):0;
            if((flag&1)==0) {
                u32 current=D(self+0x19e);
                if(eligible(self,current)) {
                    u32 n=cm_fn_0090cfc0(20);
                    if(n==0 && B(self+0x10d)==0) {
                        u8 v;
                        u32 result;
                        (void)children(self,1);
                        v=B(self+0x2b);
                        W(self+0x35)=(u16)(W(self+0x35)-15u);
                        W(self+0x198)=0x67;
                        if(v==0)B(self+0x2b)=1;
                        result=cm_fn_0090cfc0(5)+5u;
                        F(self+0x4d)=(float)((long double)(s32)result+(long double)F(self+0x4d));
                        return (u8)result;
                    }
                }
            }
        }
        state=D(self+0x19e);
        if(self!=D(state+0xf582)) {
            s32 side=(s8)B(self+0x27);
            if(B(state+(u32)side+0x1ab)==5) {
                flag=state!=0?(u8)cm_fn_006c38c0(state,self):0;
                if((flag&1)==0) {
                    volatile float distance;
                    u32 n;
                    if(B(self+0x27)==0) {
                        s8 y=absolute_byte((s8)B(self+0x103)-11);
                        s8 x=absolute_byte((s8)B(self+0x102)-4);
                        u32 index=(u32)((s32)y*9+(s32)x);
                        distance=F(0xa15e44+index*4u);
                    } else distance=(float)cm_fn_006f09b0(self,4,0);
                    n=cm_fn_0090cfc0(12);
                    if((long double)(s32)n>(long double)distance) {
                        u8 a=B(self+0x10d);
                        n=cm_fn_0090cfc0(10);
                        if((s32)(s8)a<(s32)n) {
                            u8 v;
                            u32 result;
                            (void)children(self,2);
                            v=B(self+0x2b);
                            W(self+0x198)=0x67;
                            if(v==0)B(self+0x2b)=1;
                            result=cm_fn_0090cfc0(5)+5u;
                            F(self+0x4d)=(float)((long double)(s32)result+(long double)F(self+0x4d));
                            return (u8)result;
                        }
                    }
                }
            }
        }
        if(B(self+0x2b)!=0) {
            state=D(self+0x19e);
            if(self!=D(state+0xf582)) {
                u8 result=children(self,1);
                W(self+0x198)=0x67;
                return result;
            }
        }
        state=D(self+0x19e);
        if(self!=D(state+0xf582)) {
            state=D(self+0x19e);
            if(eligible(self,state))return cm_fn_00714d40(self);
            return cm_fn_00713fc0(self,1);
        }
        (void)children(self,0);
        cm_fn_007109f0(self,0);
        state=D(self+0x19e);
        B(state+0x1a7)=0;
        state=D(self+0x19e);
        if(self==D(state+0xf582))return (u8)state;
        if(W(self+0x198)==0x6d)return (u8)state;
        if(B(self+0x2b)!=0)return (u8)state;
        if(B(state+0x8eb2)!=0)return (u8)state;
        if(B(self+0x107)==0)return 0;
        {
            long double t=(long double)F(self+0x7d);
            volatile long long converted=(long long)t; /* __ftol VA007106b0 */
            u32 first=cm_fn_0090cfc0((u32)converted);
            u32 second=cm_fn_0090cfc0(5);
            if((s32)first<=(s32)second)return (u8)second;
        }
    }
}
