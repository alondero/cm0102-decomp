#include "cmre.h"

/* Preferred-image VAs, base 00400000, primary SHA256
 * 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 * THIS4/RET16. The wrapper at 006cfd5a forwards full EAX unchanged.
 * Native writes to the first argument's low byte change the local category,
 * not the original object address cached in ESI.
 */
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
#define F(a) CM_GLOBAL(volatile float,(a))

__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_006eab60(u32 self,u32 item,u32 out,u32 kind,u32 flag)
{
    u32 eax, ecx, comparison;
    u8 bl, category=(u8)item, al;
    s32 quotient;
    long double live;
    volatile long long converted;
    eax=cm_fn_0090cfc0(B(item+0x14));
    ecx=D(self+4);
    bl=(u8)eax;
    if (ecx) {
        al=B(ecx+0x25);
        if ((s8)al>18) bl=(u8)(bl+2u);
        else if ((s8)al>12) bl=(u8)(bl+1u);
        else if ((s8)al<=6) {
            if ((s8)al>2) bl=(u8)(bl-1u);
            else bl=(u8)(bl-2u);
        }
        eax=(eax&0xffffff00u)|al;
    }
    eax=(eax&0xffffff00u)|(u8)kind;
    if ((u8)kind==1 || (u8)kind==6) { bl=(u8)(bl+4u); category=5; }
    else if ((u8)kind==2) { bl=(u8)(bl+10u); category=8; }
    else if ((u8)kind==3) { bl=(u8)(bl+7u); category=1; }
    else if ((u8)kind==4 || (u8)kind==5) { bl=(u8)(bl+100u); category=12; }
    al=B(item+0x24);
    if (al && ((u8)eax==1 || (u8)eax==6)) {
        eax=cm_fn_0090cfc0((u32)(s32)(s8)B(ecx+0x24));
        if (eax) { B(out)=9; return eax; }
        eax=(eax&0xffffff00u)|(u8)kind;
    }
    if ((u8)eax!=1) {
        eax=cm_fn_0090cfc0(15);
        eax+=7u;
        comparison=eax;
        if ((s32)(s8)bl>(s32)comparison) {
            eax=(eax&0xffffff00u)|B(item+0x19);
            if ((s8)eax>=0) {
                eax=(eax&0xffffff00u)|(u8)flag;
                if ((u8)eax==0) {
                    W(item+0x35)=(u16)(W(item+0x35)+0xfc18u);
                    B(out)=4;
                    eax=(u32)(s32)(s8)B(item+0x23);
                    ecx=(u32)(s32)(s8)B(item+0x24);
                    eax+=ecx;
                    if (eax==0) { B(item+0x25)=category; return eax; }
                    eax=(eax&0xffffff00u)|category;
                    B(item+0x26)=(u8)eax;
                    return eax;
                }
            }
            if (category!=12 && category!=8) return eax;
            ecx=(u32)(s32)(s8)B(item+0x23);
            eax=(u32)(s32)(s8)B(item+0x24);
            eax+=ecx;
            if ((s32)eax<=1) { B(item+0x25)=category; return eax; }
            if (eax) B(item+0x26)=category;
            return eax;
        }
        /* The MOV at 006eac3c is skipped by JLE 006eac3a. */
    }
    eax=(u32)(s32)(s8)B(item+0x24);
    ecx=eax*3u+5u;
    eax=cm_fn_0090cfc0(ecx);
    eax+=2u;
    comparison=eax;
    eax=(eax&0xffffff00u)|B(item+0x19);
    if ((s32)(s8)bl>(s32)comparison) {
        if ((s8)eax<0) return eax;
        eax=(eax&0xffffff00u)|(u8)flag;
        if ((u8)eax) return eax;
        eax=D(self);
        live=(long double)F(0x00968e70);
        ecx=(u32)(s32)(s16)W(eax+0x8ed0);
        /* 006eacfe..006ead0c: signed high product, SAR 1, sign correction. */
        quotient=(s32)(((long long)(s32)ecx*0x2e8ba2e9LL)>>32);
        quotient>>=1;
        quotient+=(u32)quotient>>31;
        if ((s16)quotient>45) {
            live=(long double)(s32)(s16)quotient;
            live=(long double)F(0x0096d64c)/live;
        }
        live=live*(long double)F(0x0096d648);
        /* ADR0006 registered __ftol, physical call 006ead2d; AX consumed. */
        converted=(long long)live;
        eax=(u32)converted;
        W(item+0x35)=(u16)(W(item+0x35)+(u16)eax);
        eax=(eax&0xffffff00u)|B(item+0x24);
        if ((u8)eax) {
            eax=(eax&0xffffff00u)|(u8)((u8)eax+1u);
            B(item+0x24)=(u8)eax;
            eax=out;
            B(eax)=3;
        } else {
            B(item+0x24)=1;
            B(out)=2;
        }
        ecx=(u32)(s32)(s8)B(item+0x23);
        eax=(u32)(s32)(s8)B(item+0x24);
        eax+=ecx;
        if (eax==1) { B(item+0x25)=category; return eax; }
        if (eax) B(item+0x26)=category;
        return eax;
    }
    if ((s8)eax<0) return eax;
    eax=(eax&0xffffff00u)|(u8)flag;
    if ((u8)eax) return eax;
    eax=out;
    B(eax)=9;
    return eax;
}
