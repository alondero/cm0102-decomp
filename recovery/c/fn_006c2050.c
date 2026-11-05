#include "cmre.h"
/* Preferred-image VA006c2050/base00400000, primary SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS2/RET8, AL return. All four native callers discard upper24, including
   006bfccb through the original006d6f70 boundary and following MOVSX EAX,AL.
   First stack DWORD retains its actual incoming bits: signed BYTE group first,
   then mutable float multiplier, carried when no role mask assigns a value. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile s8,(a))
#define W(a) CM_GLOBAL(volatile s16,(a))
#define F(a) CM_GLOBAL(volatile float,(a))
#define Q(a) CM_GLOBAL(volatile double,(a))
extern u16 CM_THISCALL cm_fn_00717410(u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);

__attribute__((target("no-sse3")))
u8 CM_THISCALL cm_fn_006c2050(u32 self,volatile float factor,u32 mode)
{
    s32 side=B((u32)&factor);
    u32 row=self+(u32)side*0x22d8u+0x4796u,index;
    volatile float best=0.0f,saved;
    u8 selected=255;
    long double live,x;
    volatile long long converted;
    for(index=0;index<20;index++,row+=0x1beu) {
        u32 stats,a,b;
        u16 flags;
        s32 value;
        float limit;
        if(B(row+0x19)<0)continue;
        flags=cm_fn_00717410(row);
        if(flags&1)continue;
        if(flags&2)factor=0.4f;
        else if(flags&4)factor=0.45f;
        else if(flags&8)factor=0.5f;
        else if(flags&0x10)factor=0.75f;
        else if(flags&0x20)factor=0.85f;
        else if(flags&0x40)factor=(flags&0x700)?1.1f:1.0f;
        stats=D(row+0x6d);
        if(stats) {
            value=W(stats+5);
            if(value>140)live=(long double)factor*(long double)F(0x96d3d4);
            else if(value<60)live=(long double)factor*(long double)F(0x96d3d0);
            else {live=(long double)value;live*=Q(0x967898);live*=(long double)factor;}
            value=B(stats+0x21);
            factor=(float)live;
            a=cm_fn_0090cfc0((u32)value);b=cm_fn_0090cfc0(20);
            if((s32)a<(s32)b)factor=(float)((long double)factor*(long double)F(0x96d3cc));
        }
        if((u8)mode!=0) {
            live=(long double)F(row+0xc9);
            converted=(long long)live; /* __ftol CALL006c21a6, site1. */
            a=cm_fn_0090cfc0((u32)converted);live=(long double)(s32)a;
        } else {
            a=cm_fn_0090cfc0(7);
            if(a==0) {
                live=(long double)F(row+0xa1);live+=(long double)F(0x96b020);
                converted=(long long)live; /* __ftol CALL006c21db, site2. */
                a=cm_fn_0090cfc0((u32)converted);live=(long double)(s32)a;
            } else {
                a=cm_fn_0090cfc0(3);
                if(a!=0) {
                    live=(long double)F(row+0x9d);live*=Q(0x967880);
                    x=(long double)F(row+0x95);x*=Q(0x96a6a8);live+=x;
                    x=(long double)F(row+0xb1);x*=Q(0x969510);live+=x;
                    x=(long double)F(row+0x7d);x*=Q(0x969510);live+=x;
                    live+=(long double)F(row+0x99);live+=Q(0x969550);
                    converted=(long long)live; /* __ftol CALL006c2243, site3. */
                    a=cm_fn_0090cfc0((u32)converted);
                } else {
                    live=(long double)F(row+0xd9);live*=Q(0x9689e0);
                    x=(long double)F(row+0xad);x*=Q(0x969040);live+=x;
                    live+=(long double)F(row+0xa9);live+=(long double)F(0x968a78);
                    converted=(long long)live; /* __ftol CALL006c2285, site4. */
                    a=cm_fn_0090cfc0((u32)converted);
                }
                live=(long double)(s32)a;live*=(long double)factor;
            }
        }
        limit=F(0x96af88);
        saved=(float)live;
        if(live>(long double)limit) {
            a=cm_fn_0090cfc0(5);
            live=(long double)saved-(long double)(s32)a;
        }
        if(selected==255 || live>(long double)best) {
            best=(float)live;selected=(u8)index;
        }
    }
    return selected;
}
