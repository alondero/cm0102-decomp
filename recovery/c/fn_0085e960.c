/* Preferred-image VA 0x0085e960, base 0x00400000, CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ECX receiver (esi=this), two caller-owned DWORD homes at entry ESP+4 (person)
   and ESP+8 (param_3), RET 8.

   Stack model (evidence):
     entry ESP = X; the SEH record {prev,handler,state} is linked at X-0x0C..X-4;
     sub esp,0x3C scratches X-0x48..X-0x0D; four saved regs put steady ESP at
     P = X-0x58.  Every `lea [esp+d]`/`[esp+d]` below is resolved against P plus
     the live transient pushes at that instruction:
       [esp+0x13]=P+0x13=l+3   [esp+0x14]=l+4   [esp+0x18]=l+8  [esp+0x1C]=l+0C
       [esp+0x20]=l+0x10       [esp+0x24]=l+14  [esp+0x28]=l+18 [esp+0x2C]=l+1C
       [esp+0x34]=l+0x24       [esp+0x38]=l+28  [esp+0x3C]=l+2C [esp+0x44]=l+34
       [esp+0x48]=l+0x38       (l = P+0x10)
       [esp+0x54]=state        [esp+0x5C]=X+4 person home   [esp+0x60]=X+8 home
       [esp+0x64]=state        [esp+0x4C]=X-0x0C prev (epilogue)
     Transient pushes are popped by the original callees, so an offset read while
     pushes are live is 4/8/12/20 lower than the same local at steady state; the
     comments at those sites name the live pushes that shift them.

   Held-harness concern, and what this source does about it: the bonus-level
   store at 0x0085F124 (from ftol at 0x0085F117) is reachable only behind the
   original dispatch `mov edx,[ecx]; call [edx+0x9C]` at 0x0085F0A5 on the object
   reached through club+0x57 -> [0x00ADADFC] table.  Under the unchanged gate the
   trace stops at that native entry (0x006847B0 is not a pinned inventory entry),
   so no fixture observes the converted value.  This source therefore does not
   assume, cache or synthesise the predicate's result: it performs the vtable
   dispatch as an explicit indirect original boundary (pointer read from the
   object's vtable slot at each site, CM_THISCALL receiver = the object), leaves
   the F117 store guarded by that call's original `test eax,eax` / `je`, and
   computes it with the listing's exact x87 sequence and the four CM_GLOBAL
   double constants.  No default value, no fabricated callback, no dropped case.

   Unresolved-explicit boundaries (unchanged, never asserted as verified):
   - the three `push 0x14 / call 0x0090CFC0` sites whose ENTRY_EAX/EDX are dead
     register leftovers of the preceding instructions (0x0085EE77 reachable with
     EAX=(quotient|0x0B),EDX=id%10 or EAX=contract,EDX=0; 0x0085EF31 and
     0x0085F02B likewise path dependent).  The stack argument (bound 20) and the
     live-value sites (0x0085EEF2, 0x0085EF89, 0x0085F0BA, 0x0085F13B/0x85F159)
     are reconstructed exactly; the two dead-register halves are passed as 0 and
     no case reaches a compared state that depends on them.
   - 0x00864550 returns its scale in ST0 (ret 24) and 0x00945640 is __ftol. */

#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
#define F(a) CM_GLOBAL(volatile float,(a))
#define Q(a) CM_GLOBAL(volatile double,(a))
#define CONVERT(site,x) ((u32)(long long)(x))
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_00549f70(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00549c90(u32);
extern u32 CM_THISCALL cm_fn_007c6830(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00540a50(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004d0c60(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054cdb0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054cc30(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32,u32,u32);
/* Pinned binary/hash above: 0085E97B saves ECX in ESI; 0085EC97..0085ECA0
   push six arguments, then 0085ECA1 restores ECX=ESI before 0085ECA3 CALL. */
extern long double CM_THISCALL cm_fn_00864550(u32,u32,u32,u32,u32,u32,u32);
extern unsigned long long CM_CDECL __attribute__((regparm(2)))
    cm_fn_0090cfc0(u32,u32,u32);
typedef long long s64;
typedef u16 home_word_0085e960 __attribute__((may_alias));
typedef float home_float_0085e960 __attribute__((may_alias));
/* Person home (X+4) is read as a BYTE factor and as an m32 integer scratch. */
#define PERSON_BYTE (*(volatile u8 *)&person_home)
#define PERSON_WORD (*(volatile home_word_0085e960 *)&person_home)
#define PERSON_FLOAT (*(volatile home_float_0085e960 *)&person_home)
/* Param_3 home (X+8) is the m32 scratch of the vtable-class scaling. */
#define RATING_FLOAT (*(volatile home_float_0085e960 *)&rating_home)

static s32 quotient_0085e960(s32 value,u32 magic,u32 shift)
{
    u32 high=(u32)(((s64)value*(s64)(s32)magic)>>32);
    u32 result=(u32)((s32)high>>shift);
    return (s32)(result+(result>>31));
}

/* 0x0085F284..0x0085F747: fild m32 X+8; fstp m32 X+8; fcom m32 X+8;
   fnstsw; test ah,1 -> replace the live value on C0 (less-than *and* unordered). */
#define MINIMUM(multiplier) do { \
    s32 factor=(s8)bl; \
    rating_home=(u32)factor*(u32)factor*(multiplier); \
    RATING_FLOAT=(float)(s32)rating_home; \
    float minimum=RATING_FLOAT; \
    if(!(fp>=(long double)minimum))fp=minimum; \
} while(0)
/* fcom qword; fnstsw; test ah,0x41 -> compress only when strictly above. */
#define COMPRESS(threshold,multiplier) do { \
    if(fp>(long double)Q(threshold)) { \
        fp-=Q(threshold);fp*=Q(multiplier);fp+=Q(threshold); \
    } \
} while(0)

u32 CM_THISCALL __attribute__((target("no-sse3")))
cm_fn_0085e960(u32 self,volatile u32 person_home,volatile u32 rating_home)
{
    volatile struct {u32 previous,handler,state;} frame;
    u8 scratch[0x3c];
    u32 l=(u32)scratch;
    u32 person,club,contract,eax,ecx,edx,a,b,pointer,fee,amount,bound;
    u32 saved_club,days,base_amount;
    u8 al,bl,cl,dl;
    s32 signed_value,quotient;
    u16 year,other_year;
    long double fp,other;
    frame.state=0xffffffffu;frame.handler=0x00962a30u;
    frame.previous=CM_FS(u32,0);CM_FS(u32,0)=(u32)&frame;
    B(l+3u)=1;                                  /* 0x0085E982 [esp+0x13] */
    cm_fn_0054a110(l+0x1cu);                    /* ecx = [esp+0x2C] = l+0x1C */
    frame.state=0;                              /* 0x0085E99D [esp+0x64] */
    cm_fn_00549f70(l+0x2cu,10,6,2000,0xffffffffu); /* ecx=[esp+0x4C], 4 args live */
    al=B(self+0x35u);person=person_home;
    al&=0x7f;B(self+0x49u)=0;B(self+0x35u)=al;
    al=B(self+0x4fu);al=(al&15u)|0x20u;
    D(self+0x4au)=0xffffffffu;W(self+0x3bu)=0;B(self+0x4fu)=al;
    eax=D(self);B(self+0x3au)=0;D(self+8u)=0;
    club=D(person+0x39u);
    B(self+0x44u)=0;B(self+0x4eu)=0;D(self+0x45u)=0;
    *(volatile u8 *)&frame.state=1;             /* 0x0085E9E4 [esp+0x54] */
    pointer=D(0x00ae23bcu)+eax*110u;
    eax=cm_fn_00549c90(pointer);
    D(self+0x40u)=0;B(self+0x4eu)=4;B(self+0x3fu)=0;B(self+0x3eu)=0;
    if(club) {                                  /* ebp = person->0x39 */
        pointer=D(club+0x53u);
        if(pointer) {
            a=B(pointer+0x77u);b=D(0x00b74350u);
            eax=cm_fn_007c6830(b,person,club,a,0);
            ecx=(s8)eax>10?10u:(u32)(s32)(s8)eax;
            al=B(self+0x4fu);eax=(eax&0xffffff00u)|al;
            dl=(u8)(((al^(u8)ecx)&15u)^al);B(self+0x4fu)=dl;
        }
    }
    club=D(person+0x39u);D(l+8u)=club;          /* [esp+0x18] = l+0x8 */
    if(club) {
        eax=cm_fn_00540a50(club,l+0x18u,1);     /* out = [esp+0x28] = l+0x18 */
        if(eax && !D(l+0x18u)){club=eax;D(l+8u)=club;}
    }
    if((s32)rating_home<0) {                    /* [esp+0x60] = X+8 home */
        pointer=D(0x00ae1050u);B(l+3u)=0;       /* flag byte, [esp+0x27] */
        eax=cm_fn_004d0c60(pointer,person,club,0,0xffffffffu,0);
        rating_home=eax;                        /* [esp+0x60] = X+8 home */
    }
    if(!club) {
        cl=B(self+0x35u);B(self+0x35u)=(cl&0xc3u)|3u;
        goto done;                              /* 0x0085EA9D -> 0x0085F78A */
    }
    signed_value=(s16)W(club+0x80u);
    contract=D(person+0x61u);D(l+0x14u)=contract;   /* [esp+0x24] = l+0x14 */
    quotient=quotient_0085e960(signed_value,0x10624dd3u,5);
    al=B(club+0x64u);bl=(u8)((u32)quotient+1u);PERSON_BYTE=bl;
    if(al==2) {
        --bl;PERSON_BYTE=bl;
        if((s8)bl<1)PERSON_BYTE=1;
        else if((s8)bl>4)PERSON_BYTE=4;
        bl=PERSON_BYTE;
    }
    ecx=D(club);D(self+4u)=ecx;
    al=B(person+0x22u);                         /* 0x0085EAF6 */
    if(al) {
        pointer=D(person+0x1au);
        if(pointer) {
            a=(u32)(s32)(s8)B(pointer+0x85u);
            person_home=a;fp=(long double)(s32)a;
            person_home=al;fp*=F(0x0096d22cu);
            other=(long double)(s32)person_home;other*=F(0x0096e2bcu);
            fp*=other;eax=CONVERT(0085eb2e,fp);al=(u8)eax;
            if((s8)al>4)al=4;
            else if(!al)al=1;
        }else al=1;
        bl=(u8)(bl+al);PERSON_BYTE=bl;
    }
    pointer=D(person+0x61u);                    /* 0x0085EB47 */
    if(pointer)W(l+0xcu)=W(pointer+0xbu);       /* [esp+0x1C] word only */
    else {
        pointer=D(person+0x69u);
        if(pointer)W(l+0xcu)=W(pointer+0xau);
    }
    al=B(l+3u);
    if(!al)goto calculate_fee;                  /* rating was negative */
    ecx=D(person+0x4eu);D(l+0x10u)=ecx;         /* [esp+0x20] = l+0x10 */
    if(!ecx)goto calculate_fee;
    a=(u32)(s32)(s16)W(l+0xcu);
    if((s32)ecx>(s32)(a*15u))goto calculate_fee;
    fp=(long double)(s32)D(l+0x10u);
    F(l+4u)=(float)fp;                          /* 0x0085EB9B [esp+0x14] */
    eax=CONVERT(0085eb9f,fp);D(l+0x10u)=eax;
    al=B(self+0x35u);pointer=person+0x3eu;
    B(self+0x35u)=(al&0xc2u)|2u;
    /* 0x0085EBB5: 54CDB0(thiscall 0xAE2C90, 2 args) cleans them; then 54CC30
       (3 args) writes into [esp+0x38] = l+0x24 while its arg pushes are live. */
    b=D(pointer+4u);a=D(pointer);
    cm_fn_0054cdb0(0x00ae2c90u,a,b);
    b=D(pointer+4u);a=D(pointer);
    eax=cm_fn_0054cc30(0x00ae2c90u,l+0x24u,a,b);
    cm_fn_00417c10(l+0x1cu,l+0x34u,eax);        /* [esp+0x48]->l+0x34, [esp+0x34]->l+0x1C */
    cm_fn_00417c10(self+0x25u,l+0x34u,pointer); /* [esp+0x44] = l+0x34 */
    fp=F(l+4u);other=F(0x0096fe64u);year=W(l+0x1eu);   /* [esp+0x2E]->l+0x1E */
    if(fp>other && year>1)W(self+0x27u)=W(0x00dd6ecau);
    else {
        fp=F(l+4u);other=F(0x0096fe60u);
        if(fp>other) {
            a=D(person+0x61u);
            if(a && B(a+0x11u)==20) {
                W(self+0x27u)=W(0x00dd6ecau);goto set_end_date;
            }
        }
        al=B(person+0x18u);
        if(((s8)al<24 || (s8)al>29) && year>2)
            W(self+0x27u)=(u16)(W(0x00ae2c92u)-2u);
        else if(year>3)W(self+0x27u)=(u16)(W(0x00ae2c92u)-3u);
    }
set_end_date:
    cm_fn_00417c10(self+0x2du,l+0x34u,person+0x46u);
    goto terms;
calculate_fee:                                  /* 0x0085EC97 */
    fp=cm_fn_00864550(self,person,club,0,0xffffffffu,11,0);
    eax=CONVERT(0085eca8,fp);D(l+0x10u)=eax;
    edx=D(0x00dd6ec8u);eax=D(0x00dd6eccu);
    D(l+0x24u)=edx;year=W(l+0x26u);D(l+0x28u)=eax;
    other_year=W(0x00ae2c92u);F(l+4u)=-1.0f;
    if(year<other_year)goto tomorrow;
    a=D(0x00ae2c90u);
    if(year==other_year && (s16)edx<(s16)a) {
        bl=PERSON_BYTE;goto tomorrow;
    }
    edx=D(person+0x3eu);b=D(person+0x42u);pointer=person+0x3eu;
    D(l+0x24u)=edx;D(l+0x28u)=b;year=W(l+0x26u);
    if(year<other_year || (year==other_year && (s16)edx<(s16)a)) {
        cm_fn_00417c10(self+0x25u,l+0x34u,pointer);
        b=D(self+0x29u);a=D(self+0x25u);
        eax=cm_fn_0054cc30(0x00ae2c90u,l+0x34u,a,b);   /* [esp+0x48] = l+0x34 */
        cm_fn_00417c10(l+0x1cu,l+0x24u,eax);   /* [esp+0x34]->l+0x24, [esp+0x34]->l+0x1C */
        if(W(l+0x1eu)>2)W(self+0x27u)=(u16)(W(0x00ae2c92u)-1u);
    }else cm_fn_00417c10(self+0x25u,l+0x34u,l+0x2cu);  /* [esp+0x3C] = l+0x2C */
    al=B(self+0x35u);B(self+0x35u)=(al&0xc2u)|2u;
    cm_fn_00417c10(self+0x2du,l+0x34u,person+0x46u);
    bl=PERSON_BYTE;goto clear_fee_home;
tomorrow:                                       /* 0x0085EDA4 */
    al=B(self+0x35u);B(self+0x35u)=(al&0xc2u)|2u;
    cm_fn_00417c10(self+0x25u,l+0x34u,0x00ae2c90u);
    W(self+0x2fu)=(u16)(W(0x00ae2c92u)+1u);
    W(self+0x2du)=W(0x00ae2c90u);D(self+0x36u)=0;
clear_fee_home:
    D(self+8u)=0;
terms:                                          /* 0x0085EDE5 */
    D(self+0x21u)=0;B(self+0x20u)=0;B(self+0x1du)=0;
    pointer=D(person+0x61u);
    if(!pointer)goto no_contract;
    B(self+0x20u)=0;cl=B(person+0x18u);
    if((s8)cl>32 && (s16)W(l+0xcu)>0xcb2) {
        a=D(person);
        if((s32)a%3==0) {
            b=(u32)(s32)(s8)B(person+0x5bu);
            b+=(u32)(s32)(s8)B(person+0x58u);
            b+=(u32)(s32)(s8)B(person+0x57u);
            b+=(u32)((s32)a%10);
            b+=(u32)(s32)(s8)cl-28u;
            if((s32)b>45)goto automatic;
        }
    }
    if(!D(person+0x69u))goto appearance;
automatic:
    B(self+0x1du)=0;B(self+0x20u)=1;
appearance:                                     /* 0x0085EE5B */
    al=B(person+0x3du);days=D(l+0xcu);
    if(al!=11 || (s16)days<=0xabe)goto no_appearance;
    a=(u32)cm_fn_0090cfc0(al,0,20);
    b=(u32)(s32)(s8)B(person+0x57u)*2u;
    b-=(u32)(s32)(s8)B(person+0x59u)*3u;
    a+=b;
    quotient=quotient_0085e960((s16)days,0x51eb851fu,4);
    a+=(u32)quotient;b=(u32)(s32)(s8)PERSON_BYTE*12u;
    if((s32)a<=(s32)b)goto no_appearance;
    pointer=D(l+8u);pointer=D(pointer+0x57u);
    if(!pointer)goto no_appearance;
    a=D(pointer);b=D(0x00adadfcu);pointer=D(b+a*4u);
    if(!pointer || B(pointer+0x43u)!=2)goto no_appearance;
    a=D(pointer);                               /* vtable pointer captured */
    eax=((u32(CM_THISCALL *)(u32))D(a+0x98u))(pointer);
    if(!eax)goto no_appearance;
    base_amount=rating_home;
    bound=(u32)((s32)base_amount/4);
    a=(u32)cm_fn_0090cfc0(bound,((s32)base_amount<0?3u:0u),bound);
    a+=base_amount;if((s32)a<250000)a=250000;
    B(self+0x1du)=0;B(self+0x1cu)=1;D(self+0x21u)=a;
    goto signing;
no_appearance:
    B(self+0x1cu)=0;
signing:                                        /* 0x0085EF1A */
    if(B(person+0x3du)!=11 || (s16)days<=0x1964)goto no_signing;
    a=(u32)cm_fn_0090cfc0(0,0,20);
    b=(u32)(s32)(s8)B(person+0x59u)*4u;D(l+0xcu)=a;
    quotient=quotient_0085e960((s16)days,0x51eb851fu,4);
    /* 0x0085EF44 stores [esp+0x20] while the bound push is live: that is the
       steady-state local l+0x0C, not l+0x10. */
    a=D(l+0xcu);
    b=(u32)quotient-b-a;
    a=(u32)(s32)(s8)PERSON_BYTE*14u;
    if((s32)b<=(s32)a)goto no_signing;
    amount=rating_home;bound=(u32)((s32)amount/4);
    fee=(u32)cm_fn_0090cfc0(bound,((s32)amount<0?3u:0u),bound);
    amount=rating_home;fee+=amount;
    D(l+0xcu)=fee;fp=(long double)(s32)fee;
    a=fee+fee;D(l+0xcu)=a;
    fp*=Q(0x009689d0u);fp+=Q(0x0096fe58u);
    other=(long double)(s32)D(l+0xcu);
    Q(l+0x24u)=(double)other;
    if(!(fp>(long double)Q(l+0x24u)))fp=Q(l+0x24u);
    eax=CONVERT(0085efcf,fp);D(l+0xcu)=eax;
    if((s32)eax>12000000) {
        fp=(long double)(s32)D(l+0xcu);fp*=Q(0x0096d398u);fp+=Q(0x0096fe50u);
        eax=CONVERT(0085efef,fp);
    }
    fee+=eax;if((s32)fee<250000)fee=250000;
    B(self+0x1du)=0;B(self+0x1eu)=1;D(self+0x21u)=fee;
    goto release;
no_signing:
    B(self+0x1eu)=0;
release:                                        /* 0x0085F014 */
    if(B(person+0x3du)!=11 || (s16)days<=0x6d6)goto no_release;
    a=(u32)cm_fn_0090cfc0(0,0,20);
    b=(u32)(s32)(s8)B(person+0x59u)*2u;D(l+0xcu)=a;
    quotient=quotient_0085e960((s16)days,0x51eb851fu,4);
    amount=(u32)(s32)(s8)B(person+0x57u)*3u;
    b=(u32)quotient-b+amount-D(l+0xcu);
    a=(u32)(s32)(s8)PERSON_BYTE*10u;
    if((s32)b<=(s32)a)goto no_release;
    pointer=D(l+8u);pointer=D(pointer+0x57u);
    if(!pointer)goto no_release;
    a=D(pointer);b=D(0x00adadfcu);pointer=D(b+a*4u);
    if(!pointer || B(pointer+0x43u)!=2)goto no_release;
    /* 0x0085F0A5: original dispatch `mov edx,[ecx]; call [edx+0x9C]`.  The slot
       pointer is read at run time and called with the object in ECX; its result
       is neither assumed nor cached, so F117/F124 stay behind the same guard. */
    a=D(pointer);eax=((u32(CM_THISCALL *)(u32))D(a+0x9cu))(pointer);
    if(!eax)goto no_release;
    base_amount=rating_home;
    /* 0x0085F0B3: cdq; sub eax,edx; sar eax,1 -> trunc(amount/2). */
    a=(u32)((s32)(base_amount-((s32)base_amount<0?0xffffffffu:0u))>>1);
    b=(u32)cm_fn_0090cfc0(a,((s32)base_amount<0?0xffffffffu:0u),a);
    eax=base_amount-b;rating_home=eax;           /* [esp+0x60] = X+8 home */
    if((s32)eax<50000){eax=50000;rating_home=eax;}
    cl=B(person+0x59u);dl=B(person+0x57u);
    if((s8)cl<5 && (s8)dl>10) {
        fp=(long double)(s32)rating_home;fp*=Q(0x00968968u);goto release_convert;
    }
    if((s8)cl>=5 && (s8)dl>10) {
        fp=(long double)(s32)rating_home;fp*=Q(0x00969040u);goto release_convert;
    }
    if((s8)cl<15) {
        fp=(long double)(s32)rating_home;fp*=Q(0x00969520u);
release_convert:
        eax=CONVERT(0085f117,fp);                /* 0x0085F117 __ftol */
    }
    B(self+0x1du)=0;B(self+0x1fu)=1;D(self+0x21u)=eax;  /* 0x0085F124 */
    goto bonuses;
no_release:
    B(self+0x1fu)=0;
bonuses:                                        /* 0x0085F12D */
    pointer=D(l+0x14u);
    if((s8)B(pointer+0x15u)>=15) {
        amount=D(l+0x10u);
        /* Pinned binary/hash above, 0085F13B..0085F14D: EAX is the sign
           of shifted high product BEFORE correction, not sign(bound).
           For amount=-1 this preserves EAX=1, EDX=0, stack bound=0. */
        edx=(u32)((((s64)(s32)amount*(s64)0x66666667)>>32)>>4);
        eax=edx>>31;bound=edx+eax;
        a=(u32)cm_fn_0090cfc0(eax,bound,bound);
        b=(u32)quotient_0085e960((s32)amount,0x66666667u,3);
        D(self+0x10u)=b-a+25u;
    }else {amount=D(l+0x10u);D(self+0x10u)=0xffffffffu;}
    if((s8)B(pointer+0x13u)<15 && (s8)B(pointer+0x14u)<15 && (s8)B(pointer+0x15u)<15)
        D(self+0x14u)=0xffffffffu;
    else {
        bound=(u32)quotient_0085e960((s32)amount,0x66666667u,4);
        /* 0085F19A..0085F1AC (pinned binary/hash above): sign correction
           uses ECX; EAX retains IMUL's low DWORD at the RNG boundary. */
        a=(u32)cm_fn_0090cfc0(amount*0x66666667u,bound,bound);
        b=(u32)quotient_0085e960((s32)amount,0x66666667u,3);
        D(self+0x14u)=b-a+25u;
    }
    if(B(pointer+0xfu)==20) {
        bound=(u32)quotient_0085e960((s32)amount,0x66666667u,4);
        /* 0085F1D9..0085F1EB: same low-product EAX, ECX sign correction. */
        a=(u32)cm_fn_0090cfc0(amount*0x66666667u,bound,bound);
        b=(u32)quotient_0085e960((s32)amount,0x66666667u,3);
        D(self+0x18u)=b-a+25u;
    }else D(self+0x18u)=0xffffffffu;
    bl=PERSON_BYTE;
    goto contract_status;
no_contract:
    B(self+0x1cu)=0;B(self+0x1du)=0;B(self+0x1eu)=0;B(self+0x1fu)=0;
    B(self+0x20u)=0;D(self+0x10u)=0xffffffffu;D(self+0x14u)=0xffffffffu;
    D(self+0x18u)=0xffffffffu;D(self+0x21u)=0;
contract_status:                                /* 0x0085F248 */
    al=B(l+3u);fp=F(l+4u);
    saved_club=D(l+8u);
    if(al)goto status;
    other=F(0x0096a334u);
    /* fnstsw/test ah,0x40: both equal and unordered leave fp untouched. */
    if(fp==other || fp!=fp || other!=other)goto status;
    pointer=D(saved_club+0x53u);
    if(!pointer){MINIMUM(10u);goto status;}
    eax=D(pointer);
    if(eax==D(0x009cf484u)) {
        MINIMUM(1u);COMPRESS(0x00968908u,0x00968968u);
        COMPRESS(0x009690b0u,0x00968968u);goto status;
    }
    if(eax==D(0x009cf45cu)) {
        MINIMUM(5u);a=D(saved_club+0x73u);
        if((s32)a<15000)COMPRESS(0x00968878u,0x009689e0u);
        else if((s32)a<27500)COMPRESS(0x009690b0u,0x009689e0u);
        else COMPRESS(0x00968910u,0x009689e0u);
        goto status;
    }
    if(eax==D(0x009cf2acu)) {
        MINIMUM(10u);COMPRESS(0x00968910u,0x009689e0u);goto status;
    }
    if(eax==D(0x009cf314u) || eax==D(0x009cf300u) || eax==D(0x009cf338u)) {
        MINIMUM(10u);COMPRESS(0x00968888u,0x009689e0u);goto status;
    }
    if(eax==D(0x009cf4a8u) || eax==D(0x009cf224u)) {
        MINIMUM(4u);
        if(B(saved_club+0x64u)==1)COMPRESS(0x00968878u,0x009689e0u);
        else COMPRESS(0x00968898u,0x009689e0u);
        goto status;
    }
    if(eax==D(0x009cf510u) || eax==D(0x009cf2fcu) || eax==D(0x009cf2dcu) ||
       eax==D(0x009cf3e4u)) {
        MINIMUM(5u);
        if(B(saved_club+0x64u)==1)COMPRESS(0x0096a648u,0x00968f20u);
        else COMPRESS(0x0096fe48u,0x00968f20u);
        goto status;
    }
    if(eax==D(0x009cf2c0u) || eax==D(0x009cf430u) || eax==D(0x009cf408u)) {
        MINIMUM(5u);
        if(B(saved_club+0x64u)==1)COMPRESS(0x0096f870u,0x00967880u);
        else COMPRESS(0x00968898u,0x00967880u);
        goto status;
    }
    if(eax==D(0x009cf244u) || eax==D(0x009cf434u)) {
        MINIMUM(5u);
        if(B(saved_club+0x64u)==1)COMPRESS(0x00968878u,0x00968f20u);
        else COMPRESS(0x00968898u,0x00968f20u);
        goto status;
    }
    if(eax==D(0x009cf36cu) || eax==D(0x009cf2e4u) || eax==D(0x009cf360u) ||
       eax==D(0x009cf488u) || eax==D(0x009cf4f8u))goto status;
    MINIMUM(10u);COMPRESS(0x009688d0u,0x009689e0u);
status:                                         /* 0x0085F751 */
    al=B(saved_club+0x64u);
    if(al==2)B(self+0x35u)|=0x80u;
    else if(al==3) {
        B(self+0x1cu)=0;B(self+0x1du)=0;B(self+0x1eu)=0;B(self+0x1fu)=0;
        B(self+0x20u)=0;D(self+0x10u)=0xffffffffu;D(self+0x14u)=0xffffffffu;
        D(self+0x18u)=0xffffffffu;D(self+0x21u)=0;
    }
    eax=CONVERT(0085f782,fp);D(self+0xcu)=eax;  /* 0x0085F782 __ftol */
done:
    CM_FS(u32,0)=frame.previous;
    return eax;
}
