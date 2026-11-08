/* Preferred-image VA 0x00700420, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   WIP: converted computations require complete O0/O2 and sensitivity review. */
#include "cmre.h"
extern u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_006c38c0(u32, u32, u32, u32);
extern u8 CM_THISCALL cm_fn_00713d90(u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
#define B(a) CM_GLOBAL(volatile s8,(a))
#define W(a) CM_GLOBAL(volatile s16,(a))
#define D(a) CM_GLOBAL(volatile u32,(a))
#define F(a) CM_GLOBAL(volatile float,(a))
#define Q(a) CM_GLOBAL(volatile double,(a))
__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_00700420(u32 self,u32 first,u32 second,u32 select,u32 kind,u32 out1,u32 out2)
{
    u32 eax,edi,club,slot5=out1,rng,other,target,index;
    s32 delta,ix,iy,quot;
    s8 group,coord,bytekind;
    volatile float ratio;
    volatile double saved;
    volatile long long converted;
    long double live,left;
    CM_GLOBAL(volatile u8,out1)=0;
    CM_GLOBAL(volatile u8,out2)=0;
    slot5=(slot5&0xffff0000u)|(u16)W(self+((u8)select?0x16au:0x168u));
    club=D(self+0x19eu);
    iy=B(club+0x8ea8u);
    edi=slot5+0xdcu;
    delta=(s32)B(self+0x103u)-iy;
    if(delta<0)delta=-delta;
    iy=(s8)delta;
    ix=B(club+0x8ea7u);
    delta=(s32)B(self+0x102u)-ix;
    eax=delta<0?0xffffffffu:0;
    if(delta<0)delta=-delta;
    ix=(s8)delta;
    index=(u32)(iy*9+ix);
    eax=(u32)(s32)(s8)delta;
    group=B(self+0x2bu);
    eax=(eax&0xffffff00u)|(u8)group;
    ratio=F(0x00a15e44u+index*4u);
    if(group>1||!club)return eax&0xffff0000u;
    eax=cm_fn_006c38c0(eax,0,club,self);
    if((u16)eax!=1)return eax&0xffff0000u;
    bytekind=(s8)kind;
    if(bytekind==51){edi=(u32)(s32)(s16)edi;rng=cm_fn_0090cfc0(edi);edi-=rng;edi=cm_fn_0090cfc0(edi);goto footer;}
    group=B(self+0x27u);
    if((group==1&&B(self+0x103u)<(s8)second)||(group==0&&B(self+0x103u)>(s8)second)){
        CM_GLOBAL(volatile u8,out1)=1;edi=0;goto footer;
    }
    coord=B(self+0x102u);
    if(coord<2||coord>6){CM_GLOBAL(volatile u8,out1)=3;edi=0;goto footer;}
    coord=B(self+0x103u);
    if((group==1&&(coord<10||coord>11))||(group!=1&&(coord<0||coord>1))){CM_GLOBAL(volatile u8,out1)=3;edi=0;goto footer;}
    if(!cm_fn_00713d90(self,first,second)){
        CM_GLOBAL(volatile u8,out1)=2;
        rng=cm_fn_0090cfc0((u32)(s32)W(self+0x3bu));other=cm_fn_0090cfc0(285);
        if((s32)rng>(s32)other){
            live=(long double)(s32)(s16)edi;
            saved=(double)live;
            live*=Q(0x00968e40u);
            converted=(long long)live; /* __ftol CALL 0x007005b9, ADR0006. */
            eax=(u32)converted;
            rng=cm_fn_0090cfc0(eax);
            live=(long double)(s32)rng;
            left=(long double)saved;
            left*=Q(0x00968e80u);
            live+=left;
            converted=(long long)live; /* __ftol CALL 0x007005db, ADR0006. */
            eax=(u32)converted;
            edi=eax-200u;
        }else edi=cm_fn_0090cfc0((u32)(s32)(s16)edi)-200u;
        goto footer;
    }
    coord=B(self+0x103u);
    if(coord==11||coord==0){
        if(bytekind==26)goto path9d1;
        if(bytekind==27)goto path955;
        goto path8b7;
    }
    index=(u32)(s32)bytekind-3u;
    if(index>24u)goto path7fc;
    index=CM_GLOBAL(volatile u8,0x00700bb4u+index);
    target=D(0x00700ba0u+index*4u);
    if(target==0x00700636u)goto path636;
    if(target==0x007006afu)goto path6af;
    if(target==0x00700715u)goto path715;
    if(target==0x00700792u)goto path792;
    goto path7fc;
path636:
    edi-=150u;
    rng=cm_fn_0090cfc0((u32)(s32)W(self+0x3bu));other=cm_fn_0090cfc0(285);
    if((s32)rng>(s32)other){
            live=(long double)(s32)(s16)edi;
            saved=(double)live;
            live*=Q(0x00968e40u);
            converted=(long long)live; /* __ftol CALL 0x0070066e, ADR0006. */
            eax=(u32)converted;
            rng=cm_fn_0090cfc0(eax);
            live=(long double)(s32)rng;
            left=(long double)saved;
            left*=Q(0x00968e80u);
            live+=left;
            converted=(long long)live; /* __ftol CALL 0x00700690, ADR0006. */
            eax=(u32)converted;
    }else eax=cm_fn_0090cfc0((u32)(s32)(s16)edi);
    edi=eax;
    goto footer;
path6af:
    edi-=150u;
    rng=cm_fn_0090cfc0((u32)(s32)W(self+0x3bu));other=cm_fn_0090cfc0(285);
    if((s32)rng>(s32)other){
            live=(long double)(s32)(s16)edi;
            saved=(double)live;
            live*=Q(0x00968e40u);
            converted=(long long)live; /* __ftol CALL 0x007006e7, ADR0006. */
            eax=(u32)converted;
            rng=cm_fn_0090cfc0(eax);
            live=(long double)(s32)rng;
            left=(long double)saved;
            left*=Q(0x00968e80u);
            live+=left;
            converted=(long long)live; /* __ftol CALL 0x00700709, ADR0006. */
            eax=(u32)converted;
    }else eax=cm_fn_0090cfc0((u32)(s32)(s16)edi);
    edi=eax;
    goto footer;
path715:
    CM_GLOBAL(volatile u8,out1)=4;
    edi-=150u;
    rng=cm_fn_0090cfc0((u32)(s32)W(self+0x3bu));other=cm_fn_0090cfc0(285);
    if((s32)rng>(s32)other){
            live=(long double)(s32)(s16)edi;
            saved=(double)live;
            live*=Q(0x00968e40u);
            converted=(long long)live; /* __ftol CALL 0x00700751, ADR0006. */
            eax=(u32)converted;
            rng=cm_fn_0090cfc0(eax);
            live=(long double)(s32)rng;
            left=(long double)saved;
            left*=Q(0x00968e80u);
            live+=left;
            converted=(long long)live; /* __ftol CALL 0x00700773, ADR0006. */
            eax=(u32)converted;
    }else eax=cm_fn_0090cfc0((u32)(s32)(s16)edi);
    edi=eax;
    goto ratio_low;
path792:
    CM_GLOBAL(volatile u8,out1)=4;
    edi-=200u;
    rng=cm_fn_0090cfc0((u32)(s32)W(self+0x3bu));other=cm_fn_0090cfc0(285);
    if((s32)rng>(s32)other){
            live=(long double)(s32)(s16)edi;
            saved=(double)live;
            live*=Q(0x00968e40u);
            converted=(long long)live; /* __ftol CALL 0x007007ce, ADR0006. */
            eax=(u32)converted;
            rng=cm_fn_0090cfc0(eax);
            live=(long double)(s32)rng;
            left=(long double)saved;
            left*=Q(0x00968e80u);
            live+=left;
            converted=(long long)live; /* __ftol CALL 0x007007f0, ADR0006. */
            eax=(u32)converted;
    }else eax=cm_fn_0090cfc0((u32)(s32)(s16)edi);
    edi=eax;
    goto ratio_low;
path7fc:
    rng=cm_fn_0090cfc0((u32)(s32)W(self+0x3bu));other=cm_fn_0090cfc0(285);
    if((s32)rng>(s32)other){
            live=(long double)(s32)(s16)edi;
            saved=(double)live;
            live*=Q(0x00968e40u);
            converted=(long long)live; /* __ftol CALL 0x0070082e, ADR0006. */
            eax=(u32)converted;
            rng=cm_fn_0090cfc0(eax);
            live=(long double)(s32)rng;
            left=(long double)saved;
            left*=Q(0x00968e80u);
            live+=left;
            converted=(long long)live; /* __ftol CALL 0x00700850, ADR0006. */
            eax=(u32)converted;
    }else eax=cm_fn_0090cfc0((u32)(s32)(s16)edi);
    edi=eax;
    if((long double)ratio>Q(0x0096d700u)){
        live=(long double)(s32)W(self+0x3bu);live*=Q(0x0096d7c8u);
        converted=(long long)live; /* __ftol CALL 0x0070088c, ADR0006. */
        edi+=10u-(u32)converted;
    }
    goto footer;
path8b7:
    rng=cm_fn_0090cfc0((u32)(s32)W(self+0x3bu));other=cm_fn_0090cfc0(285);
    if((s32)rng>(s32)other){
            live=(long double)(s32)(s16)edi;
            saved=(double)live;
            live*=Q(0x00968e40u);
            converted=(long long)live; /* __ftol CALL 0x007008e4, ADR0006. */
            eax=(u32)converted;
            rng=cm_fn_0090cfc0(eax);
            live=(long double)(s32)rng;
            left=(long double)saved;
            left*=Q(0x00968e80u);
            live+=left;
            converted=(long long)live; /* __ftol CALL 0x00700906, ADR0006. */
            eax=(u32)converted;
    }else eax=cm_fn_0090cfc0((u32)(s32)(s16)edi);
    edi=eax;
    if((long double)ratio>Q(0x0096d700u)){
        live=(long double)(s32)W(self+0x3bu);live*=Q(0x0096d7c8u);
        converted=(long long)live; /* __ftol CALL 0x00700942, ADR0006. */
        edi+=10u-(u32)converted;
    }
    goto footer;
path955:
    rng=cm_fn_0090cfc0((u32)(s32)W(self+0x3bu));other=cm_fn_0090cfc0(285);
    if((s32)rng>(s32)other){
            live=(long double)(s32)(s16)edi;
            saved=(double)live;
            live*=Q(0x00968e40u);
            converted=(long long)live; /* __ftol CALL 0x00700982, ADR0006. */
            eax=(u32)converted;
            rng=cm_fn_0090cfc0(eax);
            live=(long double)(s32)rng;
            left=(long double)saved;
            left*=Q(0x00968e80u);
            live+=left;
            converted=(long long)live; /* __ftol CALL 0x007009a4, ADR0006. */
            eax=(u32)converted;
    }else eax=cm_fn_0090cfc0((u32)(s32)(s16)edi);
    edi=eax;
    edi+=125u;goto ratio_low;
path9d1:
    rng=cm_fn_0090cfc0((u32)(s32)W(self+0x3bu));other=cm_fn_0090cfc0(285);
    if((s32)rng>(s32)other){
            live=(long double)(s32)(s16)edi;
            saved=(double)live;
            live*=Q(0x00968e40u);
            converted=(long long)live; /* __ftol CALL 0x00700a03, ADR0006. */
            eax=(u32)converted;
            rng=cm_fn_0090cfc0(eax);
            live=(long double)(s32)rng;
            left=(long double)saved;
            left*=Q(0x00968e80u);
            live+=left;
            converted=(long long)live; /* __ftol CALL 0x00700a25, ADR0006. */
            eax=(u32)converted;
    }else eax=cm_fn_0090cfc0((u32)(s32)(s16)edi);
    edi=eax;
    edi+=75u;goto ratio_low;
ratio_low:
    /* FCOMP sets C0 for less-than and unordered. */
    if(!((long double)ratio>=Q(0x00967890u)))edi+=100u;
footer:
    live=(long double)F(self+0xd1u);
    club=D(self+0x19eu);
    eax=CM_GLOBAL(volatile u8,self+0xfu);
    live+=live;
    bytekind=B(club+0x8eaau);
    live+=(long double)(s32)eax;
    live+=Q(0x009688b0u);
    converted=(long long)live; /* __ftol CALL 0x00700a78, ADR0006. */
    rng=cm_fn_0090cfc0((u32)converted);
    live=(long double)ratio;
    live=Q(0x00968950u)-live;
    live*=(long double)(s32)bytekind;
    live*=Q(0x009688b0u);
    converted=(long long)live; /* __ftol CALL 0x00700aa2, ADR0006. */
    other=cm_fn_0090cfc0((u32)converted);
    if((s32)rng>(s32)other){
        quot=(s32)(s16)slot5/6;
        left=(long double)quot;
        live=(long double)(s32)(s16)edi;
    }else{
        CM_GLOBAL(volatile u8,out2)=1;
        rng=cm_fn_0090cfc0((u32)(s32)W(self+0x3bu));other=cm_fn_0090cfc0(285);
        if((s32)rng>(s32)other){
            live=(long double)(s32)(s16)edi;
            saved=(double)live;
            live*=Q(0x00968e40u);
            converted=(long long)live; /* __ftol CALL 0x00700b18, ADR0006. */
            eax=(u32)converted;
            rng=cm_fn_0090cfc0(eax);
            live=(long double)(s32)rng;
            left=(long double)saved;
            left*=Q(0x00968e80u);
            live+=left;
            converted=(long long)live; /* __ftol CALL 0x00700b3a, ADR0006. */
            eax=(u32)converted;
        }else eax=cm_fn_0090cfc0((u32)(s32)(s16)edi);
        quot=(s32)(s16)slot5/6;
        left=(long double)quot;
        live=(long double)(s32)(s16)eax;
    }
    live*=Q(0x0096d698u);
    live=left+live;
    live+=Q(0x00968ea8u);
    converted=(long long)live; /* __ftol CALL 0x00700b83, ADR0006. */
    return (u32)converted;
}
