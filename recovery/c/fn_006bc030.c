#include "cmre.h"
/* Preferred-image VA006bc030/base00400000; primary SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/RET4, full EAX pointer. Candidate scores escape only through signed
   WORD ranking at006bc573/006bcb4d. Their upper16 never reach an address,
   floating input, original call argument or return; modulo16 arithmetic
   below removes no observable input from the incidental register carriers. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
#define F(a) CM_GLOBAL(volatile float,(a))
#define Q(a) CM_GLOBAL(volatile double,(a))
extern u8 CM_THISCALL cm_fn_005b57c0(u32,u32);
extern u32 CM_THISCALL cm_fn_007160f0(u32);
extern void CM_THISCALL cm_fn_006c2be0(u32,u32,u32,u32,u32);
extern long double CM_THISCALL cm_fn_006f09b0(u32,u32,u32);
extern u16 CM_THISCALL cm_fn_00717410(u32);
extern u8 CM_THISCALL cm_fn_0070dfa0(u32);
extern u8 CM_THISCALL cm_fn_00715f30(u32);
extern u8 CM_THISCALL cm_fn_007103c0(u32);
extern u8 CM_THISCALL cm_fn_007163f0(u32);
extern u8 CM_THISCALL cm_fn_006cb780(u32,u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);

static s8 absolute_byte(s32 a)
{
    u32 bits=(u32)a;
    u32 sign=(u32)(a>>31);
    return (s8)(u8)((bits^sign)-sign);
}

static void distance_one(u32 l,s32 dx,s32 dy)
{
    s8 x=absolute_byte(dx),y=absolute_byte(dy);
    if(x>=0 && x<=8 && y>=0 && y<=11) {
        long double v=(long double)F(0x00a15e44+((u32)y*9u+(u32)x)*4u);
        F(l+12)=(float)v;
    } else D(l+12)=0x447a0000;
}

static void distance_two(u32 l,s32 dx,s32 dy)
{
    s8 x=absolute_byte(dx),y=absolute_byte(dy);
    long double v;
    if(x>=0 && x<=8 && y>=0 && y<=11)
        v=(long double)F(0x00a15e44+((u32)y*9u+(u32)x)*4u);
    else v=(long double)F(0x0096d2dc);
    v+=F(0x00968990);
    F(l+8)=(float)v;
}

static u8 crowd(u32 self,u32 bank,u32 x,u32 y)
{
    u32 person=self+bank*8920u+0x4796;
    u8 count=0;
    for(u32 n=0;n<20;++n,person+=0x1be) {
        if((s8)B(person+0x19)>=0 && cm_fn_00717410(person)!=1) {
            long double value=cm_fn_006f09b0(person,x,y);
            double zero=Q(0x009689a0);
            /* Native C3 test includes unordered as well as equality. */
            if(!(value<(long double)zero || value>(long double)zero))
                count=(u8)(count+3);
            else if(!(value>(long double)Q(0x00968940)))
                count=(u8)(count+1);
        }
    }
    return count;
}

static u32 position_allowed(u32 item,u32 candidate,u32 l)
{
    if(B(item+0x10a) && cm_fn_0070dfa0(candidate)) {
        u8 bank=B(l+5);
        if(!bank) {
            if((s8)B(l+2)>=6)return 0;
        } else if(bank==1 && (s8)B(l+2)<6)return 0;
    }
    return 1;
}

static u16 ending_bonus(u32 item,u32 candidate,u32 first,u32 second)
{
    u32 a=D(candidate+0x69),b=D(item+0x69),p;
    u32 x=D(b+0x1a),y=D(a+0x1a);
    if(x==y) {p=D(candidate+0x6d);return (u16)((s32)(s16)W(p+9)/100-50);}
    if(first && second) {
        x=D(first+0x53);y=D(second+0x53);
        if(x==y) {p=D(candidate+0x6d);return (u16)((s32)(s16)W(p+11)/100-50);}
    }
    p=D(candidate+0x6d);
    return (u16)((s32)(s16)W(p+13)/100-50);
}

__attribute__((target("no-sse3")))
static u32 first_score(u32 self,u32 item,u32 candidate,u32 l,u32 first,
                       u32 second,u8 flag,u8 count,u16 *result)
{
    u32 a=cm_fn_0090cfc0((21u-(u32)(s32)(s8)B(item+0x10a))*5u);
    u16 score=(u16)a;
    long double live=(long double)F(l+8);
    volatile long long converted;
    if(!(live>=(long double)Q(0x009688a0)) && B(candidate+0x109)) {
        live=(long double)F(l+8);
        live*=F(0x00968a78);
        live=(long double)F(0x0096d2d8)-live;
        converted=(long long)live; /* __ftol CALL006bc3d0, site1. */
        score=(u16)(score+(u16)converted);
    }
    a=cm_fn_0090cfc0(30);
    if(!((long double)(s32)a>=(long double)F(candidate+0xb1))) {
        a=cm_fn_0090cfc0(30);
        if(!((long double)(s32)a>=(long double)F(item+0xb1))) {
            live=(long double)F(l+8);
            live*=F(0x00968a78);
            live=(long double)F(0x0096d2d8)-live;
            converted=(long long)live; /* __ftol CALL006bc429, site2. */
            score=(u16)(score+(u16)converted);
        }
    }
    score=(u16)(score-(u16)((s32)(s8)flag*100));
    a=cm_fn_006cb780(self,candidate);
    u32 preferred=D(item+0x1aa);
    if((u8)a) {
        if(candidate==preferred)return 1;
        score=(u16)(score+175);
    } else if(candidate==preferred) {
        a=cm_fn_0090cfc0((u32)((s32)(s8)flag+1));
        if(!a)return 1;
    }
    /* Native MOVZX AX/CX and MOVSX DI preserve upper16 carriers; all
       resulting products feed only the final WORD score. */
    a=B(candidate+11);
    live=(long double)F(l+8);live*=F(l+8);
    u32 b=B(candidate+10);
    live-=F(0x00968960);
    u32 cube=(u32)(s32)(s8)count;
    u16 part=(u16)((a+b+20u)*10u);
    converted=(long long)live; /* __ftol CALL006bc4b3, site3. */
    part=(u16)(part-(u16)converted);
    part=(u16)(part-(u16)(cube*cube*cube));
    s32 word=(s32)(s16)W(candidate+0x35);
    a=B(candidate+0x10);
    live=(long double)(s32)a;
    part=(u16)(part+(u16)(word/200));
    live*=Q(0x0096d2d0);
    converted=(long long)live; /* __ftol CALL006bc4ee, site4. */
    part=(u16)(part-(u16)converted);
    live=(long double)F(l+12);live*=F(l+12);live*=F(0x00969560);
    converted=(long long)live; /* __ftol CALL006bc503, site5. */
    part=(u16)(part-(u16)converted);
    score=(u16)(score+part);
    score=(u16)(score+ending_bonus(item,candidate,first,second));
    *result=score;
    return 0;
}

static u16 flags_word(u32 self,u32 candidate,s8 index,u32 table)
{
    if(index<0 || index>10)return 0;
    u32 group=(u32)(s32)(s8)B(candidate+0x27);
    u32 row=D(self+group*4u+table);
    return W(row+(u32)(s32)index*2u);
}

static u16 table_bonus(u32 self,u32 candidate,s8 index)
{
    if(flags_word(self,candidate,index,0x8ebc)&0x20) {
        if(!(flags_word(self,candidate,index,0x8ebc)&0x800) &&
           !(flags_word(self,candidate,index,0x8ebc)&0x80)) {
            u32 p=D(candidate+0x6d);
            return (u16)((s32)(s8)B(p+0x1a)*10);
        }
    }
    if(index>=0 && index<=10) {
        if(flags_word(self,candidate,index,0x8ebc)==0x140 ||
           flags_word(self,candidate,index,0x8ebc)==0x240 ||
           flags_word(self,candidate,index,0x8ebc)==0x440)goto ordinary;
        if(flags_word(self,candidate,index,0x8ec4)==0x140 ||
           flags_word(self,candidate,index,0x8ec4)==0x240 ||
           flags_word(self,candidate,index,0x8ec4)==0x440)return 125;
    }
ordinary:
    if(cm_fn_007163f0(candidate))return 40;
    return 0;
}

__attribute__((target("no-sse3")))
static u32 other_score(u32 self,u32 item,u32 candidate,u32 l,u32 first,
                       u32 second,u8 flag,u8 count,u16 *result)
{
    u32 a=cm_fn_0090cfc0((21u-(u32)(s32)(s8)B(item+0x10a))*5u);
    u16 score=(u16)a;
    u32 slot=cm_fn_0090cfc0(30);
    long double live;
    volatile long long converted;
    if(!((long double)(s32)slot>=(long double)F(candidate+0xad))) {
        slot=cm_fn_0090cfc0(30);
        if(!((long double)(s32)slot>=(long double)F(item+0xad))) {
            live=(long double)F(l+8);live*=F(0x00968a78);
            live=(long double)F(0x0096d2d8)-live;
            converted=(long long)live; /* __ftol CALL006bc888, site6. */
            score=(u16)(score+(u16)converted);
        }
    }
    score=(u16)(score-(u16)((s32)(s8)flag*100));
    /* 006bc8a7 is ESP+34 while the candidate push is still live: this
       is the same baseline ESP+30 slot that previously held RNG30. */
    slot=score;
    a=cm_fn_006cb780(self,candidate);
    u32 preferred=D(item+0x1aa);
    if((u8)a) {
        if(candidate==preferred)return 1;
        score=(u16)(score+175);slot=score;
    } else if(candidate==preferred) {
        a=cm_fn_0090cfc0((u32)((s32)(s8)flag+1));
        if(!a) {
            if(cm_fn_007103c0(candidate))return 1;
            if(candidate==D(self+0xf57a))return 1;
            score=(u16)(score+20);slot=score;
        }
    }
    /* The later006bc8ff store updates this slot after a bonus. */
    u32 b=B(candidate+11);a=B(candidate+10);
    u32 square=(u32)(s32)(s8)count;
    s32 word=(s32)(s16)W(candidate+0x35);
    u16 part=(u16)((a+b+10u)*10u);
    live=(long double)word;live*=Q(0x0096d2c8);
    live=(long double)Q(0x0096d2a8)-live;
    converted=(long long)live; /* __ftol CALL006bc936, site7. */
    part=(u16)(part-(u16)converted);
    live=(long double)F(l+8);live*=F(l+8);live-=F(0x00968960);
    converted=(long long)live; /* __ftol CALL006bc94b, site8. */
    part=(u16)(part-(u16)converted);
    a=B(candidate+0x10);live=(long double)(s32)a;live*=Q(0x0096d2d0);
    converted=(long long)live; /* __ftol CALL006bc965, site9. */
    part=(u16)(part-(u16)converted);
    live=(long double)F(l+12);live*=F(l+12);
    part=(u16)(part-(u16)(square*square));
    live*=F(0x00969560);
    converted=(long long)live; /* __ftol CALL006bc981, site10. */
    part=(u16)(part-(u16)converted);
    score=(u16)((u16)slot+part);
    s8 index=(s8)B(candidate+0x19);
    score=(u16)(score+table_bonus(self,candidate,index));
    score=(u16)(score+ending_bonus(item,candidate,first,second));
    *result=score;
    return 0;
}

__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_006bc030(u32 self,volatile u32 item_arg)
{
    volatile u8 scratch[0x54];
    u32 l=(u32)scratch,item=item_arg,best=0,first,second;
    u16 best_score=0,score;
    u8 index=B(item+0x19);
    u32 group=(u32)(s32)(s8)B(item+0x27);
    /* Native caller reads whole uninitialized scratch DWORD after its BYTE
       store; complete005b57c0 reads only BYTE [ESP+4], so upper24 are dead. */
    if(cm_fn_005b57c0(self+group*6371u+0x91e2,index)&1)return 0;
    u32 candidate=cm_fn_007160f0(item);
    if(candidate && (s8)B(candidate+0x19)>=0)return candidate;
    B(l+5)=(B(item+0x27)==0);
    u32 p=D(self+0x4792);
    if(B(l+5)) {first=D(p+0x1c);second=D(p+0x20);}
    else {first=D(p+0x20);second=D(p+0x1c);}
    cm_fn_006c2be0(self,item,l+3,l+2,1);
    u8 y=(u8)(B(l+2)-2u);
    while((s8)y<=(s8)(u8)(B(l+2)+1u)) {
        s32 row=(s32)(s8)y;
        if(row==(s32)(s8)B(l+2)-2 || ((s8)y>=0 && (s8)y<=11)) {
            u8 x=(u8)(B(l+3)-1u);
            while((s8)x<=(s8)(u8)(B(l+3)+1u)) {
                if(row==(s32)(s8)B(l+2)-2) {
                    if((s32)(s8)x==(s32)(s8)B(l+3)-1) {
                        candidate=D(self+0xf582);
                        if(candidate) {
                            u32 cy=B(item+0x103),cx=B(item+0x102);
                            long double value=cm_fn_006f09b0(candidate,cx,cy);
                            if(!(value>(long double)Q(0x00968950))) {
                                candidate=D(self+0xf582);
                                cx=B(candidate+0x102);cy=B(candidate+0x103);
                                distance_one(l,(s32)(s8)cx-(s32)(s8)B(l+3),
                                               (s32)(s8)cy-(s32)(s8)B(l+2));
                                u8 count=crowd(self,B(l+5),cx,cy);
                                u32 original_item=item_arg;
                                candidate=D(self+0xf582);
                                u8 side=B(original_item+0x27);
                                s32 dx=(s32)(s8)B(candidate+0x102)-4;
                                s32 dy=(s32)(s8)B(candidate+0x103);
                                if(side)dy-=11;
                                distance_two(l,dx,dy);
                                u8 flag=B(candidate+0x101);
                                if(flag!=3 && position_allowed(original_item,candidate,l)) {
                                    u32 early=first_score(self,original_item,candidate,l,first,second,flag,count,&score);
                                    if(early)return candidate;
                                    if(!best || (s16)score>(s16)best_score) {best=candidate;best_score=score;}
                                }
                            }
                        }
                    }
                    /* Native first-row remaining columns execute only its
                       local-byte increment/skip sequence before ending row. */
                    break;
                }
                if((s8)x>=0 && (s8)x<=8) {
                    u32 bank=(u32)(s32)(s8)B(l+5);
                    u32 list=self+(u32)(row+(s32)(s8)x*12)*90u+0x215e;
                    if(B(list+bank+0x58)) {
                        distance_one(l,(s32)(s8)x-(s32)(s8)B(l+3),row-(s32)(s8)B(l+2));
                        u8 count=crowd(self,bank,x,y);
                        u32 original_item=item_arg;
                        u8 side=B(original_item+0x27);
                        distance_two(l,(s32)(s8)x-4,side?row-11:row);
                        if((s8)B(list+bank+0x58)>0) {
                            u8 n=0;
                            do {
                                candidate=D(list+((u32)(s32)(s8)n+bank*11u)*4u);
                                if(candidate!=D(self+0xf582)) {
                                    u8 flag=B(candidate+0x101);
                                    if(flag!=3 && position_allowed(original_item,candidate,l)) {
                                        u32 allowed=1;
                                        if(candidate!=D(self+0xf582) && candidate!=D(self+0xf57a)) {
                                            group=(u32)(s32)(s8)B(original_item+0x27);
                                            u32 f=D(self+group*6371u+0x9766);
                                            if((f&0x400) && B(original_item+0x10a)) {
                                                if((s8)cm_fn_00715f30(candidate)>=0)allowed=0;
                                            } else if((s8)cm_fn_00715f30(candidate)>0)allowed=0;
                                        }
                                        if(allowed) {
                                            u32 early=other_score(self,original_item,candidate,l,first,second,flag,count,&score);
                                            if(early)return candidate;
                                            if(!best || (s16)score>(s16)best_score) {best=candidate;best_score=score;}
                                        }
                                    }
                                }
                                ++n;
                            }while((s8)n<(s8)B(list+bank+0x58));
                        }
                    }
                }
                ++x;
            }
        }
        ++y;
    }
    return best;
}
