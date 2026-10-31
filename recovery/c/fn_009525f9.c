#include "cmre.h"
/* Preferred-image VA009525f9, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL6/fullEAX. Scratch1c: constantL0, packed96-bit valueLc,
   return flagL18. Formal low/high/exponent slots become a12-byte copy.
   Preserve unsigned address tests and signedBYTE rounding/length behavior. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_CDECL cm_fn_0094c3a0(u32,u32);
extern u32 CM_CDECL cm_fn_009531a6(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00952f86(u32,u32);
extern u32 CM_CDECL cm_fn_00952006(u32);
extern u32 CM_CDECL cm_fn_00952034(u32);
extern u32 CM_CDECL cm_fn_00951fa8(u32,u32);

u32 CM_CDECL cm_fn_009525f9(volatile u32 low_arg,volatile u32 high_arg,volatile u32 exponent_arg,volatile u32 digits_arg,volatile u32 options_arg,volatile u32 output_arg)
{
    u8 scratch[0x1c];
    u32 copied_args[3];
    u32 l=(u32)scratch,copy=(u32)copied_args,raw=exponent_arg,out,sign,exponent,high,estimate,shift,digits,pointer,start,cursor,i,string,length,value,remaining;
    u8 digit;
    sign=raw&0x8000; exponent=raw&0x7fff;
    out=output_arg;
    for (i=0;i<10;++i) B(l+i)=0xcc;
    B(l+10)=0xfb; B(l+11)=0x3f;
    D(l+0x18)=1;
    B(out+2)=sign?0x2d:0x20;
    high=high_arg;
    if (!exponent && !high && low_arg==0) goto zero;
    if (exponent==0x7fff) {
        W(out)=1;
        if ((high!=0x80000000 || low_arg!=0) && !(high&0x40000000)) {
            string=0x009719dc; length=6;
        } else if (sign && high==0xc0000000) {
            if (low_arg==0) {string=0x009719d4; length=5;}
            else {string=0x009719c4; length=6;}
        } else if (high==0x80000000 && low_arg==0) {
            string=0x009719cc; length=5;
        } else {string=0x009719c4; length=6;}
        cm_fn_0094c3a0(out+4,string);
        B(out+3)=(u8)length;
        D(l+0x18)=D(l+0x18)&0;
        return D(l+0x18);
    }
    estimate=(u32)(s32)((s32)((exponent>>8)+(high>>24)*2)*0x4d+exponent*0x4d10-0x134312f4);
    estimate=(u32)((s32)estimate>>16);
    W(l+0xc)=0;
    W(l+0x16)=(u16)exponent;
    D(l+0x12)=high;
    D(l+0xe)=low_arg;
    cm_fn_009531a6(l+0xc,0-(u32)(s32)(s16)estimate,1);
    if (W(l+0x16)>=0x3fff) {
        ++estimate;
        cm_fn_00952f86(l+0xc,l);
    }
    value=options_arg;
    W(out)=(u16)estimate;
    digits=digits_arg;
    if (value&1) {
        digits+=(u32)(s32)(s16)estimate;
        if ((s32)digits<=0) goto zero;
    }
    if ((s32)digits>21) digits=21;
    shift=(u32)W(l+0x16)-0x3ffe;
    W(l+0x16)=0;
    i=8;
    do {cm_fn_00952006(l+0xc); --i;} while (i);
    if ((s32)shift<0) {
        shift=(0-shift)&0xff;
        while (shift) {cm_fn_00952034(l+0xc); --shift;}
    }
    pointer=out+4;
    remaining=digits+1;
    if ((s32)remaining>0) {
        do {
            D(copy)=D(l+0xc); D(copy+4)=D(l+0x10); D(copy+8)=D(l+0x14);
            cm_fn_00952006(l+0xc);
            cm_fn_00952006(l+0xc);
            cm_fn_00951fa8(l+0xc,copy);
            cm_fn_00952006(l+0xc);
            digit=B(l+0x17);
            B(l+0x17)=B(l+0x17)&0;
            digit=(u8)(digit+0x30);
            ++pointer;
            --remaining;
            B(pointer-1)=digit;
        } while (remaining);
    }
    digit=B(pointer-1);
    cursor=pointer-2;
    start=out+4;
    if ((s8)digit>=0x35) {
        while (cursor>=start && B(cursor)==0x39) {
            B(cursor)=0x30;
            --cursor;
        }
        if (cursor<start) {
            ++cursor;
            W(out)=(u16)(W(out)+1);
        }
        B(cursor)=(u8)(B(cursor)+1);
    } else {
        while (cursor>=start && B(cursor)==0x30) --cursor;
        if (cursor<start) goto zero;
    }
    digit=(u8)((u8)cursor-(u8)out-3);
    B(out+3)=digit;
    B(out+(u32)(s32)(s8)digit+4)=B(out+(u32)(s32)(s8)digit+4)&0;
    return D(l+0x18);
zero:
    W(out)=W(out)&0;
    B(out+2)=0x20; B(out+3)=1; B(out+4)=0x30;
    B(out+5)=B(out+5)&0;
    return 1;
}
