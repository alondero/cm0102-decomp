/* Preferred-image VA 008e08e0, base 00400000.
 * SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 * All callees remain original boundaries. Offsets below use the native ESP
 * after the four saved registers; scratch starts at that ESP + 0x10.
 */
#include "cmre.h"
extern unsigned long long CM_THISCALL cm_fn_00785830(void *, u32);
extern u32 CM_CDECL cm_fn_0053e140(u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0053e990(u32,u32,u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00549ef0(void *,u32,u32);
extern u32 CM_THISCALL cm_fn_0054ba20(void *,u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_0043f460(const int *,u32);
extern u32 CM_CDECL cm_fn_00441c10(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32,u32,...);
extern u32 CM_CDECL cm_fn_0090c520(u32,int,short,int);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *,u32);
extern u32 CM_THISCALL cm_fn_004ae770(u32,u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32,u32,u32,u32);
#define P(o) ((u32)(scratch + (o) - 0x10))
#define D(o) (*(volatile u32 *)(scratch + (o) - 0x10))
#define B(o) (*(volatile u8 *)(scratch + (o) - 0x10))
#define F(o) (*(volatile float *)(scratch + (o) - 0x10))
#define G(a) CM_GLOBAL(volatile u32,a)
#define GET(n) ((u32)cm_fn_00785830((void *)input,(n)))

u32 CM_STDCALL __attribute__((target("no-sse3")))
cm_fn_008e08e0(u32 input,u32 mode,u32 output)
{
    volatile struct { u32 prev,handler,state; } seh;
    u8 scratch[0x7dc]; /* Deliberately uninitialized, including date scratch. */
    u32 eax,person,first,second,third,club,other,base,pa,pb,sa,sb;
    u32 i,n,selector=13,fmt;
    unsigned long long value;
    seh.prev=CM_FS(u32,0); seh.handler=0x9646d1; seh.state=0xffffffff;
    CM_FS(u32,0)=(u32)&seh;
    if (!mode) {
        person=GET(0); first=GET(1); second=GET(2); third=GET(3);
        D(0x14)=third; club=GET(4);
        cm_fn_0053e140(first,second,third,P(0xe4),300,5,person);
        base=G(0xae23b4); club*=581u;
        sa=(u32)(s32)CM_GLOBAL(volatile s8,club+base+0x52);
        cm_fn_0066f4e0(0xde1f64,0xa9bb9c,sa,sa,club+base+0x38,P(0xe4));
        cm_fn_004ae660((void *)output,0xde1f64);
        base=G(0xae23b4); club+=base;
        cm_fn_004ae8a0(output,club+0x38,0x7d5,club);
        base=G(0xae23bc); person*=110u;
        eax=cm_fn_0053e990(person+base,first,second,D(0x14),0);
        if (eax) { base=G(0xae23bc); eax=cm_fn_004ae8a0(output,P(0xe4),0x7d7,person+base); }
        goto done;
    }
    B(0x210)=CM_GLOBAL(volatile u8,0xad9c64);
    for (i=0;i<99;i++) D(0x211+i*4)=0;
    *(volatile u16 *)(scratch+0x39d-0x10)=0;
    B(0x39f)=0;
    D(0x28)=GET(0); D(0x14)=GET(1); D(0x24)=GET(2);
    D(0x20)=GET(3); club=GET(4); D(0x2c)=club; other=GET(5);
    eax=GET(6); D(0x18)=eax; F(0x1c)=(float)(s32)D(0x18);
    eax=GET(8); D(0x18)=eax; F(0x18)=(float)(s32)D(0x18);
    D(0x30)=GET(10); D(0x38)=GET(11); eax=GET(12); B(0x12)=(u8)eax;
    if ((u8)eax) {
        B(0x13)=0;
        for (;;) {
            n=(u32)(s32)(s8)(u8)eax;
            if ((s32)n>=3) n=3;
            if ((s8)B(0x13)>=(s32)n) break;
            eax=(eax&0xffffff00u)|(u8)selector++;
            eax=GET(eax); i=(u32)(s32)(s8)B(0x13); D(0x3c)=eax;
            base=G(0xae23bc); D(0x58+4*i)=base+eax*110u;
            eax=(base&0xffffff00u)|(u8)selector++; eax=GET(eax);
            D(0x44)=eax; D(0x64+4*i)=eax;
            eax=(eax&0xffffff00u)|(u8)selector++; eax=GET(eax);
            D(0x40)=eax; D(0x4c+4*i)=eax;
            eax=(eax&0xffffff00u)|(u8)selector++; eax=GET(eax); D(0x70+4*i)=eax;
            eax=cm_fn_0053e140(D(0x44),D(0x40),eax,P(0x468+i*300),300,1,D(0x3c));
            club=D(0x2c); B(0x13)=(u8)(B(0x13)+1);
            eax=(eax&0xffffff00u)|B(0x12);
        }
        if ((u8)eax==1) {
            n=0; do { n++; } while (B(0x468+n-1));
            for (i=0;i<n/4;i++) D(0x210+i*4)=D(0x468+i*4);
            for (i=(n/4)*4;i<n;i++) B(0x210+i)=B(0x468+i);
            club=D(0x2c);
        } else if ((u8)eax==2) cm_fn_0066f4e0(P(0x210),0xa91c40,P(0x468),P(0x594));
        else if ((u8)eax==3) cm_fn_0066f4e0(P(0x210),0xa91bc0,P(0x468),P(0x594),P(0x6c0));
        else {
            cm_fn_0090c520((u32)(s32)(s8)(u8)eax,(int)P(0xb0),0x32,1);
            cm_fn_0066f4e0(P(0x210),0xa91ba0,P(0xb0));
        }
    }
    cm_fn_0053e140(D(0x14),D(0x24),D(0x20),P(0xe4),300,1,D(0x28));
    eax=(u32)cm_fn_00549ef0((void *)P(0x30),D(0x30),D(0x38));
    seh.state=0; cm_fn_0054ba20((void *)eax,P(0x3a0),200,255);
    if (F(0x18)>F(0x1c)) {
        /* Physical __ftol at preferred VA 008e0bde, float32 reload. */
        D(0x30)=(u32)(long long)(long double)F(0x18);
        seh.state=1; value=cm_fn_0043f460((const int *)P(0x30),6);
        cm_fn_00441c10(P(0x7c),50,(u32)value,(u32)(value>>32),6);
        fmt=B(0x12)?0xa9bfc4:0xa9c0c0;
    } else {
        /* Physical __ftol at preferred VA 008e0cd3, float32 reload. */
        D(0x48)=(u32)(long long)(long double)F(0x1c);
        seh.state=2; value=cm_fn_0043f460((const int *)P(0x48),6);
        cm_fn_00441c10(P(0x7c),50,(u32)value,(u32)(value>>32),6);
        n=F(0x1c)>CM_GLOBAL(volatile float,0x968970);
        if (B(0x12)) fmt=n?0xa9bd20:0xa9bc3c;
        else if (n) fmt=0xa9bf10;
        else fmt=other==0xffffffff?0xa9be10:0xa9be7c;
    }
    base=G(0xae23b4);
    if (fmt==0xa9be10) {
        club*=581u; sa=(u32)(s32)CM_GLOBAL(volatile s8,club+base+0x52);
        cm_fn_0066f4e0(0xde1f64,fmt,100,sa,sa,P(0xe4),club+base+0x38);
    } else {
        pb=base+other*581u+0x38; club*=581u;
        sb=(u32)(s32)CM_GLOBAL(volatile s8,pb+0x1a);
        sa=(u32)(s32)CM_GLOBAL(volatile s8,club+base+0x52); pa=club+base+0x38;
        if (fmt==0xa9bfc4 || fmt==0xa9bd20)
            cm_fn_0066f4e0(0xde1f64,fmt,100,sa,sa,sb,sb,P(0xe4),pa,pb,P(0x7c),P(0x210));
        else if (fmt==0xa9bc3c)
            cm_fn_0066f4e0(0xde1f64,fmt,100,sa,sa,sb,sb,P(0xe4),pa,pb,P(0x210));
        else if (fmt==0xa9be7c)
            cm_fn_0066f4e0(0xde1f64,fmt,100,sa,sa,sb,sb,P(0xe4),pa,pb);
        else cm_fn_0066f4e0(0xde1f64,fmt,100,sa,sa,sb,sb,P(0xe4),pa,pb,P(0x7c));
    }
    seh.state=0xffffffff; cm_fn_004ae660((void *)output,0xde1f64);
    cm_fn_0066f4e0(0xde1f64,0xa9bbec,100,P(0x3a0));
    cm_fn_004ae770(output,0xde1f64);
    base=G(0xae23b4); club+=base;
    cm_fn_004ae8a0(output,club+0x38,0x7d5,club);
    if (other!=0xffffffff) {
        base=G(0xae23b4); pa=base+other*581u;
        cm_fn_004ae8a0(output,pa+0x38,0x7d5,pa);
    }
    person=D(0x28)*110u; base=G(0xae23bc);
    eax=cm_fn_0053e990(person+base,D(0x14),D(0x24),D(0x20),0);
    if (eax) { base=G(0xae23bc); eax=cm_fn_004ae8a0(output,P(0xe4),0x7d7,person+base); }
    eax=(eax&0xffffff00u)|B(0x12);
    if ((s8)(u8)eax>0 && (s8)(u8)eax<=3) {
        n=(u8)eax; D(0x14)=n;
        for (i=0;;i++) {
            pa=D(0x58+4*i);
            eax=cm_fn_0053e990(pa,D(0x64+4*i),D(0x4c+4*i),D(0x70+4*i),0);
            if (eax) cm_fn_004ae8a0(output,P(0x468+300*i),0x7d7,pa);
            eax=D(0x14)-1; D(0x14)=eax; if (!eax) break;
        }
    }
done:
    CM_FS(u32,0)=seh.prev;
    return eax;
}
