#include "cmre.h"
/* Preferred-image VA006c23f0/base00400000, primary SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS2/RET8, AL return: sole native caller stores AL, then overwrites AL
   and kills full EAX on both successors before reading remaining bits.
   First stack DWORD is initially a signed BYTE group and subsequently a
   mutable float multiplier. Unmatched role flags retain its actual bits. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile s8,(a))
#define U(a) CM_GLOBAL(volatile u8,(a))
#define W(a) CM_GLOBAL(volatile s16,(a))
#define F(a) CM_GLOBAL(volatile float,(a))
#define Q(a) CM_GLOBAL(volatile double,(a))
extern u16 CM_THISCALL cm_fn_00717410(u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);

__attribute__((target("no-sse3")))
u8 CM_THISCALL cm_fn_006c23f0(u32 self,volatile float factor,u32 mode)
{
    s32 side=B((u32)&factor);
    u32 row=self+(u32)side*0x22d8u+0x4796u,index;
    volatile float best=0.0f;
    u8 selected=255;
    long double live;
    volatile long long converted;
    for(index=0;index<20;index++,row+=0x1beu) {
        u16 flags;
        u32 stats,a,b;
        s32 value;
        s8 byte;
        if(B(row+0x19)<0)continue;
        flags=cm_fn_00717410(row);
        if(flags&1)continue;
        if(flags&2)factor=0.15f;
        else if(flags&4)factor=0.2f;
        else if(flags&8)factor=(flags&0x880)?0.5f:0.3f;
        else if(flags&0x10)factor=(flags&0x880)?0.65f:0.8f;
        else if(flags&0x20)factor=1.05f;
        else if(flags&0x40)factor=(flags&0x700)?0.95f:0.85f;
        stats=D(row+0x6d);
        if(stats) {
            value=W(stats+5);
            if(value>140)live=(long double)factor*(long double)F(0x96d3d4);
            else if(value<60)live=(long double)factor*(long double)F(0x96d3d0);
            else {live=(long double)value;live*=Q(0x967898);live*=(long double)factor;}
            byte=B(stats+0x37);
            factor=(float)live;
            if(byte>15)factor=(float)((long double)factor*(long double)F(0x96a33c));
            else if(byte<6)factor=(float)((long double)factor*(long double)F(0x96d38c));
            else if(byte<10)factor=(float)((long double)factor*(long double)F(0x96b014));
            byte=B(stats+0x43);
            if(byte>15)factor=(float)((long double)factor*(long double)F(0x96a33c));
            else if(byte<6)factor=(float)((long double)factor*(long double)F(0x96d38c));
            else if(byte<10)factor=(float)((long double)factor*(long double)F(0x96b014));
            if((u8)mode==0) {
                u8 sum=U(stats+0x40),part=U(stats+0x36);
                sum=(u8)(sum+part);part=U(stats+0x26);sum=(u8)(sum+part);
                part=U(stats+0x1b);sum=(u8)(sum+part);
                byte=(s8)sum;
                if(byte>75)factor=(float)((long double)factor*(long double)F(0x96b010));
                else if(byte>60)factor=(float)((long double)factor*(long double)F(0x96d3b8));
                else if(byte<25)factor=(float)((long double)factor*(long double)Q(0x969040));
                else if(byte<40)factor=(float)((long double)factor*(long double)F(0x96b014));
            }
            value=B(stats+0x21);
            a=cm_fn_0090cfc0((u32)value);b=cm_fn_0090cfc0(20);
            if((s32)a<(s32)b)factor=(float)((long double)factor*(long double)F(0x96d248));
        }
        if((u8)mode!=0) {
            live=(long double)F(row+0xa1);live-=(long double)F(row+0x99);
            live-=(long double)F(row+0xad);live+=(long double)F(0x969560);
            converted=(long long)live; /* __ftol CALL006c2679, site1. */
            a=cm_fn_0090cfc0((u32)converted);
            live=(long double)(s32)a;
        } else {
            a=cm_fn_0090cfc0(5);
            if(a) {
                live=(long double)F(row+0xf1);live+=(long double)F(row+0xc5);
                live+=(long double)F(row+0x8d);live+=(long double)F(0x968990);
                converted=(long long)live; /* __ftol CALL006c26b7, site2. */
                a=cm_fn_0090cfc0((u32)converted);
            } else {
                live=(long double)F(row+0xa9);live+=(long double)F(0x968990);
                converted=(long long)live; /* __ftol CALL006c26db, site3. */
                a=cm_fn_0090cfc0((u32)converted);
            }
            live=(long double)(s32)a;live*=(long double)factor;
        }
        if(selected==255 || live>(long double)best) {
            best=(float)live;selected=(u8)index;
        }
    }
    return cm_fn_0090cfc0(5)!=0?selected:255;
}
