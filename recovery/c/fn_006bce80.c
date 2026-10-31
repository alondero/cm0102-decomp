#include "cmre.h"
/* Preferred-image VA006BCE80/base00400000; pinned 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS4/RET16/AL. L=afterSUB2C. The BYTE coordinates wrap before signed
   comparisons. Native6FA140/6F09B0 consume only BYTE stack coordinates;
   the upper bytes of the original partial stack DWORDs are unobserved.
   Preserve every float32 store and use extended precision between stores. */
#define D(a) CM_GLOBAL(volatile u32,(u32)(a))
#define W(a) CM_GLOBAL(volatile u16,(u32)(a))
#define B(a) CM_GLOBAL(volatile u8,(u32)(a))
#define F(a) CM_GLOBAL(volatile float,(u32)(a))
#define Q(a) CM_GLOBAL(volatile double,(u32)(a))
#define SB(a) ((s32)(s8)B(a))
extern u8 CM_THISCALL cm_fn_00714d30(u32);
extern u8 CM_THISCALL cm_fn_006fa140(u32,s8,s8);
extern long double CM_THISCALL cm_fn_006f09b0(u32,s8,s8);
extern u32 CM_FASTCALL cm_fn_00717410(u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);

u8 CM_THISCALL cm_fn_006bce80(u32 self,volatile u32 actor_argument,
                              u32 first_output,u32 second_output,
                              volatile u32 flags)
{
    volatile u8 scratch[0x2c];
    u32 l=(u32)scratch,actor=actor_argument,context,pointer,candidate,a,c,d;
    u32 random_first,random_second;
    s32 inner,count;
    u8 al,bl,cl,dl;
    volatile long double value,other;

    D(l+0x18u)=self;
    al=B(actor+0x102u);
    cl=B(actor+0x103u);
    B(l+3u)=al;
    al=(u8)flags&2u;
    B(l+4u)=0xf6;
    B(l+9u)=0xff;
    B(l+8u)=0xff;
    D(l+0x1cu)=0;
    D(l+0x24u)=0x40f00000u;
    B(l+5u)=cl;
    B(l+6u)=al;
    if(al==0 && B(actor+0x107u)==0) {
        if(B(actor+0x105u)==0) return 0;
        random_first=cm_fn_0090cfc0(6);
        random_second=cm_fn_0090cfc0(6);
        D(l+0x20u)=random_first-random_second;
        F(l+0x1cu)=(float)(long double)(s32)D(l+0x20u);
    }
    al=cm_fn_00714d30(actor);
    bl=B(l+5u);
    cl=B(l+5u);
    B(l+7u)=(u8)(al==0);
    --bl;
    ++cl;
    B(l+0x10u)=bl;
    B(l+0xbu)=cl;
    if((s8)bl>(s8)cl) goto outputs;
outer:
    if((s8)bl<0 || (s8)bl>=12) goto next_outer;
    if(((u8)flags&4u)!=0) {
        al=B(actor+0x107u);
        if((s8)al<2) {
            al=cm_fn_00714d30(actor);
            if(al==1) {
                context=D(l+0x18u);
                al=B(l+0x10u);
                bl=B(context+0x8eb0u);
            } else {
                context=D(l+0x18u);
                bl=B(l+0x10u);
                al=B(context+0x8eafu);
            }
            bl=(u8)(bl-al);
            al=(u8)cm_fn_0090cfc0(3);
            bl=(u8)(bl-al);
            al=(u8)cm_fn_0090cfc0(3);
            bl=(u8)(bl+al);
            B(l+4u)=bl;
            bl=B(l+0x10u);
        } else {
            al=cm_fn_00714d30(actor);
            context=D(l+0x18u);
            if(al==1) al=(u8)(B(context+0x8eb0u)-bl);
            else al=(u8)(bl-B(context+0x8eafu));
            B(l+4u)=al;
        }
        if(B(actor+0x106u)!=0) {
            al=cm_fn_00714d30(actor);
            a=(u32)(al==0);
            context+=a;
            a=D(context+a*6370u+0x9766u);
            if((a&0x400u)!=0) B(l+4u)=(u8)(B(l+4u)+1u);
        }
        if(SB(l+4u)>2) goto next_outer;
    }
    cl=B(l+3u);
    al=(u8)(cl-1u);
    ++cl;
    B(l+0xcu)=al;
    B(l+0xau)=cl;
    if((s8)al>(s8)cl) goto next_outer;
inner_loop:
    if((s8)al<0) goto next_inner;
    al=B(l+0xcu);
    if((s8)al>=9) goto next_inner;
    bl=B(l+3u);
    if(bl==al) {
        al=B(l+5u);
        cl=B(l+0x10u);
        if(al==cl && B(l+6u)!=0) goto next_inner;
    }
    if(B(l+6u)!=0) {
        if(cm_fn_006fa140(actor,(s8)B(l+0xcu),(s8)B(l+0x10u))==0)
            goto next_inner;
    }
    al=B(actor+0x19u);
    if((s8)al>=0 && (s8)al<=10) {
        c=(u32)SB(actor+0x27u);
        context=D(l+0x18u);
        d=(u32)(s32)(s8)al;
        c=D(context+c*4u+0x8ebcu);
        c=(c&0xffff0000u)|W(c+d*2u);
    } else {
        context=D(l+0x18u);
        c=0;
    }
    if(((u8)c&0x30u)!=0) {
        if((s8)al>=0 && (s8)al<=10) {
            d=(u32)SB(actor+0x27u);
            a=(u32)(s32)(s8)al;
            c=D(context+d*4u+0x8ebcu);
            a=(a&0xffff0000u)|W(c+a*2u);
        } else a=0;
        if((a&0x880u)!=0) {
            al=B(l+0xcu);
            if((s8)al>=2 && (s8)al<=6 && !((s8)bl>=2 && (s8)bl<=6)) {
                al=cm_fn_00714d30(actor);
                if(al==1) {
                    if((s8)bl>=2 && (s8)bl<=6) {
                        al=B(l+5u);
                        if((s8)al>=10 && (s8)al<=11) goto role_allowed;
                    }
                } else if((s8)bl>=2 && (s8)bl<=6) {
                    al=B(l+5u);
                    if((s8)al>=0 && (s8)al<=1) goto role_allowed;
                }
                if(((u8)flags&1u)==0) goto next_inner;
            }
        }
    }
role_allowed:
    inner=SB(l+0xcu);
    a=(u32)SB(l+0x10u)+(u32)inner*12u;
    bl=0;
    d=D(l+0x1cu);
    D(l+0x14u)=d;
    candidate=context+a*90u+0x215eu;
    cl=B(l+6u);
    c=(u32)SB(l+7u);
    if(cl!=0) {
        bl=B(candidate+c+0x58u);
        if(bl!=0) {
            a=D(actor+0x6du);
            c=(u32)SB(a+0x26u);
            random_first=cm_fn_0090cfc0(c);
            random_second=cm_fn_0090cfc0(3);
            if((s32)random_first>(s32)random_second) {
                a=(u32)(s32)(s8)bl-2u;
                bl=(s32)a<0 ? 0 : (u8)a;
            }
            actor=actor_argument;
        }
        D(l+0x14u)=(u32)(s32)(s8)bl;
        value=(long double)(s32)D(l+0x14u);
        a=(u32)SB(actor+0x2cu);
        value=value-(long double)Q(0x00968968u);
        value=(long double)F(l+0x1cu)-value;
        F(l+0x14u)=(float)value;
        if(a==0 || a==1) {
            al=B(actor+0x27u);
            cl=B(l+3u);
            dl=B(l+0xcu);
            if((a==1 && al==0) || (a==0 && al!=0)) {
                if((s8)dl<(s8)cl || (dl==cl && (s8)dl<4)) goto directional;
            } else {
                if((s8)dl>(s8)cl || (dl==cl && (s8)dl>4)) goto directional;
            }
        }
        goto counted;
directional:
        D(l+0x28u)=(u32)SB(actor+0x2du);
        value=(long double)(s32)D(l+0x28u);
        value=value*(long double)Q(0x00968fd0u);
        value=(long double)F(l+0x14u)-value;
        F(l+0x14u)=(float)value;
    } else {
        cl=B(candidate+c+0x58u);
        if((s8)cl>0) {
            pointer=candidate+c*44u;
            count=SB(candidate+c+0x58u);
            do {
                a=D(pointer);
                if(SB(a+0x19u)>=0 && SB(a+0x2bu)<2) ++bl;
                pointer+=4u;
                --count;
            } while(count!=0);
        }
    }
counted:
    if(bl!=0) {
        dl=B(l+6u);
        if(dl==0) {
            D(l+0x28u)=(u32)(s32)(s8)bl;
            value=(long double)(s32)D(l+0x28u);
            value=value*value;
            value=value+value;
            value=(long double)F(l+0x14u)-value;
            goto score;
        }
    }
    c=(u32)SB(l+7u);
    d=D(l+0x18u);
    D(l+0x20u)=0x459c4000u;
    bl=0;
    pointer=d+c*8920u+0x4796u;
    do {
        al=B(pointer+0x19u);
        if((s8)al>=0 && (cm_fn_00717410(pointer)&1u)==0) {
            value=cm_fn_006f09b0(pointer,(s8)B(l+0xcu),(s8)B(l+0x10u));
            other=(long double)Q(0x009689a0u);
            if(!(value<other) && !(value>other)) {
                value=(long double)F(0x00968970u);
                goto square;
            }
            other=(long double)F(l+0x20u);
            if(!(value>=other)) F(l+0x20u)=(float)value;
        }
        ++bl;
        pointer+=0x1beu;
    } while((s8)bl<20);
    value=(long double)F(l+0x20u);
square:
    dl=B(l+6u);
    value=value*value;
    value=value+(long double)F(l+0x14u);
score:
    al=B(actor+0x27u);
    bl=B(l+0x10u);
    if(al==0 && (s8)bl<(s8)B(actor+0x103u)) goto edge;
    if(al==1 && (s8)bl>(s8)B(actor+0x103u)) goto edge;
    cl=B(l+5u);
    al=0;
    if(bl!=cl) value=value+(long double)Q(0x00968938u);
    else value=value+(long double)Q(0x00968968u);
    goto margin;
edge:
    if(((u8)flags&8u)!=0) al=0;
    else { value=value-(long double)Q(0x0096afa0u); al=1; }
margin:
    cl=B(l+4u);
    if((s8)cl>0) {
        D(l+0x28u)=(u32)(s32)(s8)cl*4u;
        value=value-(long double)(s32)D(l+0x28u);
    }
    if(al!=0) goto choose;
    if(dl!=0 && ((u8)flags&1u)==0) goto inner_position;
    if((s8)bl>=3 && (s8)bl<=8) goto inner_position;
    bl=B(l+0xcu);
    if((s8)bl<6 && (s8)bl>2) value=value+(long double)Q(0x00968e38u);
    goto position;
inner_position:
    bl=B(l+0xcu);
position:
    if(bl==4) goto central;
    if((s8)bl>2) {
        al=B(actor+0x102u);
        if((s8)al<4 && (s8)al>=(s8)bl) goto central;
    }
    if((s8)bl<6) {
        al=B(actor+0x102u);
        if((s8)al>4 && (s8)al<=(s8)bl) goto central;
    }
    goto displacement;
central:
    value=value+(long double)Q(0x00968930u);
displacement:
    cl=B(l+3u);
    if(dl==0) {
        if((s8)cl>4) {
            a=(u32)(s32)(s8)cl-4u;
            d=(u32)inner-4u;
            if((s32)a<(s32)d) {
                D(l+0x28u)=a*3u;
                value=value-(long double)(s32)D(l+0x28u);
            }
        } else if((s8)cl<4 && (s32)(s8)cl-4>inner-4) {
            D(l+0x28u)=(4u-(u32)(s32)(s8)cl)*3u;
            value=value-(long double)(s32)D(l+0x28u);
        }
        goto choose;
    }
    if(!((s8)cl>=2 && (s8)cl<=6) && !((s8)bl>=2 && (s8)bl<=6)
        && ((u8)flags&1u)==0) {
        other=(long double)F(actor+0xe1u);
        other=other*(long double)Q(0x009689e0u);
        value=value+other;
    }
    if((s8)cl>=4) {
        a=(u32)(s32)(s8)cl-4u;
        d=(u32)inner-4u;
        D(l+0x28u)=a;
        if((s32)a<(s32)d) {
            other=(long double)(s32)D(l+0x28u);
            goto penalty;
        }
        if((s8)cl>4) goto old_outputs;
    }
    if((s32)(s8)cl-4>inner-4) {
        D(l+0x28u)=4u-(u32)(s32)(s8)cl;
        other=(long double)(s32)D(l+0x28u);
        goto penalty;
    }
    goto old_outputs;
penalty:
    other=other*(long double)Q(0x00968e18u);
    value=value-other;
old_outputs:
    al=B(first_output);
    if(al==bl) {
        al=B(l+0x10u);
        cl=B(second_output);
        if(cl==al) value=value+(long double)F(0x0096b008u);
    }
choose:
    bl=B(l+0xcu);
    other=(long double)F(l+0x24u);
    if(value>=other) {
        al=B(l+0x10u);
        B(l+9u)=bl;
        F(l+0x24u)=(float)value;
        B(l+8u)=al;
    }
next_inner:
    al=B(l+0xcu);
    cl=B(l+0xau);
    ++al;
    B(l+0xcu)=al;
    if((s8)al<=(s8)cl) goto inner_loop;
    bl=B(l+0x10u);
next_outer:
    al=B(l+0xbu);
    ++bl;
    B(l+0x10u)=bl;
    if((s8)bl<=(s8)al) goto outer;
outputs:
    cl=B(l+8u);
    al=B(l+9u);
    B(second_output)=cl;
    B(first_output)=al;
    return (u8)((s8)al>-1);
}
