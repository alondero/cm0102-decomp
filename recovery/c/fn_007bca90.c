#include "cmre.h"
/* Preferred-image VA007bca90/base00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL3/fullEAX. The first and third formal slots are rewritten below.
   Original helpers remain explicit native boundaries. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile s16,(a))
#define B(a) CM_GLOBAL(volatile s8,(a))
#define U(a) CM_GLOBAL(volatile u8,(a))
#define Q(a) CM_GLOBAL(volatile double,(a))
extern u32 CM_CDECL cm_fn_0053c590(u32,u8,u32);
extern u32 CM_THISCALL cm_fn_005a0590(u32,u32);
extern u8 CM_THISCALL cm_fn_005986b0(u32);

__attribute__((target("no-sse3")))
u32 CM_CDECL cm_fn_007bca90(volatile u32 arg1,u32 output,volatile u32 arg3)
{
    volatile union { struct {u32 low,high;} bits; double real; } scratch;
    u32 club=arg1,other=0,a,p,table,target,constant;
    s32 score,base,adjusted,derived,result,count,difference,offset;
    long double live;
    volatile long long converted;
    a=D(club+0x1db);
    if((s32)a>-1)other=D(0xae23d0)+a*107u;
    a=arg3;
    if(a) {
        score=W(a);scratch.bits.low=(u32)score;
    } else {
        live=(long double)(s32)D(club+0x77);
        score=W(club+0x80);base=score;adjusted=score;
        scratch.bits.low=(u32)score;
        live*=Q(0x96e2b0);live-=Q(0x96e2a8);
        converted=(long long)live; /* Native FTOL007bcaeb, site1. */
        derived=(s16)(u16)converted;
        if(score>derived)derived=score;
        else if(derived>10000)derived=10000;
        p=D(club+0x57);
        if(p) {
            count=W(p+0x69);
            if(count>base/500) {base=count*500;adjusted=base;}
        }
        if(other) {
            count=W(other+0x69);
            score=(s32)scratch.bits.low;
            if(count>score/500)adjusted=count*600;
        }else score=(s32)scratch.bits.low;
        if(base<8000)
            a=(u32)base+(u32)derived+(u32)base*2u+((u32)adjusted+(u32)score*2u)*4u;
        else a=(u32)derived+(u32)score*7u+((u32)base+(u32)adjusted)*4u;
        score=(s32)a/16;scratch.bits.low=(u32)score;
    }
    if(U(club+0x64)!=1) {score/=2;scratch.bits.low=(u32)score;}
    if(score>9000)result=(s32)((u32)score*3u+7500u);
    else if(score>7000) {
        live=(long double)(s32)scratch.bits.low;live*=Q(0x96d688);live-=Q(0x96d760);
        goto cast2;
    }else if(score>5000) {
        live=(long double)(s32)scratch.bits.low;live*=Q(0x968e18);live+=Q(0x9690b8);
        goto cast2;
    }else if(score>3000)result=(s32)((u32)score*5u-5000u);
    else {
        live=(long double)(s32)scratch.bits.low;live*=Q(0x969018);live+=Q(0x9688e8);
        goto cast2;
    }
    goto after2;
cast2:
    converted=(long long)live; /* Native FTOL007bcc21, site2. */
    result=(s32)(u32)converted;
after2:
    a=arg3;scratch.bits.low=(u32)result;
    if(a)goto done;
    a=cm_fn_0053c590(club,0,0);
    table=D(0xadadfc);count=(s8)(u8)a;
    U((u32)&arg1)=(u8)a;
    if(count<16) {
        p=D(club+0x57);
        if(p) {
            a=D(p);
            if(D(table+a*4u)) {result/=2;goto done;}
        }
    }
    if(count<20) {
        p=D(club+0x57);
        if(p) {
            a=D(p);
            if(D(table+a*4u)) {
                a=cm_fn_005a0590(D(0xae2a38),club);
                if(!a)goto done;
                a=cm_fn_005986b0(a);
                if((u8)a==254) {result/=2;goto done;}
                if((u8)a!=255)goto done;
                live=(long double)(s32)scratch.bits.low;live*=Q(0x96a670);
                goto cast5;
            }
        }
    }
    p=D(club+0x57);
    if(p) {
        a=D(p);
        if(D(table+a*4u)) {
            arg3=(u32)(s32)W(p+0x69);
            live=(long double)(s32)arg3;live*=Q(0x969588);live+=Q(0x968ea8);
            converted=(long long)live; /* Native FTOL007bccde, site3. */
            difference=(s8)(u8)(U((u32)&arg1)-(u8)converted);
            if(difference<0) {
                arg1=(u32)(difference>-10?difference:-10);
                live=(long double)(s32)arg1;live*=Q(0x967888);live+=Q(0x967890);
                live*=(long double)(s32)scratch.bits.low;
                converted=(long long)live; /* Native FTOL007bcd15, site4. */
                result=(s32)(u32)converted;scratch.bits.low=(u32)result;
            }
        }
    }
    a=cm_fn_005a0590(D(0xae2a38),club);
    if(!a)goto done;
    a=cm_fn_005986b0(a);
    a=(u32)(s32)(s8)(u8)a+2u;
    if(a>4)goto done;
    target=D(0x7bce38+a*4u);
    switch(target) {
    case 0x7bcde2:offset=-1000;constant=0x96d718;break;
    case 0x7bcdb3:offset=-500;constant=0x96e2a0;break;
    case 0x7bce16:goto done;
    case 0x7bcd87:offset=125;constant=0x96d6b8;break;
    case 0x7bcd51:offset=250;constant=0x968fa0;break;
    default:goto done; /* All five immutable pinned table slots listed above. */
    }
    live=(long double)(s32)scratch.bits.low;
    arg1=(u32)result+(u32)offset;
    live*=Q(constant);
    scratch.real=(double)(s32)arg1;
    if(offset>0) {
        if(!(live>(long double)scratch.real))live=(long double)scratch.real;
    }else {
        if(live>=(long double)scratch.real)live=(long double)scratch.real;
    }
cast5:
    converted=(long long)live; /* Native FTOL007bce0f, site5. */
    result=(s32)(u32)converted;
done:
    if(result<500)result=500;
    if(output)D(output)=(u32)score;
    return (u32)result;
}
