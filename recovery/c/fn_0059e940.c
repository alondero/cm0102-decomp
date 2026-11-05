#include "cmre.h"

/* Preferred-image VAs, base 00400000; original SHA256
 * 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 * THIS2/RET8. Locals relative to L after AND ESP,-8 / SUB ESP,1c:
 * saved self L+4, integer L+8, scale L+c, rounded ratio L+14.
 * No local address escapes; the live x87 ratio is distinct from its FST copy.
 */
extern u8 CM_THISCALL cm_fn_005986b0(u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern long long CM_THISCALL cm_fn_00598800(u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00539c60(u32);
/* 0053a5b0 ignores its physical stack argument. */
extern u32 CM_CDECL cm_fn_0053a5b0(u32);
extern u32 CM_CDECL cm_fn_0053a1b0(u32);
/* Incoming ECX is overwritten at 0059f324 before any use or nested call.
 * Keep the caller's actual cached ECX and four physical pushes explicit. */
extern void CM_THISCALL cm_fn_0059f300(u32,u32,u32,u32,u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile s16 *)(u32)(a))
#define B(a) (*(volatile s8 *)(u32)(a))

__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_0059e940(u32 self, u32 edi, u32 esi)
{
    u32 eax, edx, ecx, delta, low, high, saved;
    s32 rating, denominator;
    u8 category = cm_fn_005986b0(esi);
    union { double d; u32 w[2]; } scale;
    volatile double ratio;
    long double live;
    int staff;
    if (category == 0xfe) {
        if (W(edi+0x80) > 0x109a) {
            scale.w[0]=0x9999999a; scale.w[1]=0x3fd99999;
        } else { scale.w[0]=0; scale.w[1]=0x3fd00000; }
    } else if (category == 0xff) {
        rating=W(edi+0x80);
        scale.w[0]=0x9999999a;
        scale.w[1]=rating>0x109a ? 0x3fc99999 : 0x3fa99999;
    } else { scale.w[0]=0xd2f1a9fc; scale.w[1]=0x3f50624d; }
    eax=D(edi+0x57);
    if (eax) {
        rating=W(edi+0x80); denominator=W(eax+0x69);
        live=(long double)rating;
        live=live*CM_GLOBAL(volatile double,0x00969048);
        live=live/(long double)denominator;
        ratio=(double)live;
        /* FNSTSW C0 is also set for unordered. */
        if (!(live >= CM_GLOBAL(volatile double,0x00969040))) ratio=0.75;
    } else ratio=0.5;
    eax=D(edi+0xbf);
    staff=0;
    if (eax) { eax=D(eax+0x69); if (eax) staff=1; }
    if (staff) {
        eax=(u32)(s32)B(eax+0x16);
        if (cm_fn_0090cfc0(eax)) return 0;
        rating=W(edi+0x80);
        ratio=(double)((long double)rating*(long double)ratio);
        live=(long double)CM_GLOBAL(volatile double,0x0096a728);
        live=live/(long double)scale.d;
        /* ADR0006: physical __ftol call 0059EA77, live ST0 -> EDX:EAX. */
        eax=(u32)(long long)live;
        eax=cm_fn_0090cfc0(eax);
        if ((long double)(s32)eax >= (long double)ratio) return 0;
    } else {
        if (cm_fn_0090cfc0(0x14)) return 0;
        rating=W(edi+0x80);
        ratio=(double)((long double)rating*(long double)ratio);
        live=(long double)CM_GLOBAL(volatile double,0x0096a728);
        live=live/(long double)scale.d;
        /* ADR0006: physical __ftol call 0059EC95, live ST0 -> EDX:EAX. */
        eax=(u32)(long long)live;
        eax=cm_fn_0090cfc0(eax);
        if ((long double)(s32)eax >= (long double)ratio) return 0;
    }
    eax=(u32)(s32)W(edi+0x80)+1000u;
    ecx=D(esi);
    delta=eax*1000u-ecx;
    if ((s32)delta>0x2625a00) {
        eax=cm_fn_0090cfc0(2500);
        rating=W(edi+0x80);
        eax+=2000u;
        if (rating<(s32)eax) delta=0u-D(esi);
    }
    eax=D(esi+0xb4); edx=D(esi+0x154); ecx=D(esi);
    D(esi+0xb4)=eax+delta;
    D(esi+0x154)=edx+delta;
    low=ecx+delta;
    D(esi)=low;
    high=D(esi+4);
    D(esi+4)=high+(u32)((s32)delta>>31)+(low<ecx);
    saved=(u32)cm_fn_00598800(esi,0,0,0,1,0);
    if (staff) {
        eax=D(edi+0xbf);
        if (eax) {
            eax=D(edi+0x53);
            if (eax && cm_fn_0090cfc0(1000)) {
                eax=D(edi+0xbf); ecx=D(edi+0x53); D(eax+0x1a)=ecx;
            } else {
                edx=CM_GLOBAL(volatile u32,0x00ae2350);
                eax=cm_fn_0090cfc0(edx);
                edx=CM_GLOBAL(volatile u32,0x00ae23a8);
                eax=edx+eax*290u;
                ecx=D(edi+0xbf); D(ecx+0x1a)=eax;
            }
            edx=D(edi+0xbf); eax=D(edx+0x1a);
            eax=cm_fn_00539c60(eax); ecx=D(edi+0xbf); D(ecx+4)=eax;
            edx=D(edi+0xbf); eax=D(edx+0x1a);
            eax=cm_fn_0053a5b0(eax); ecx=D(edi+0xbf); D(ecx+0xc)=eax;
            edx=D(edi+0xbf); eax=D(edx+0x1a);
            eax=cm_fn_0053a1b0(eax); ecx=D(edi+0xbf); D(ecx+8)=eax;
            edx=D(edi+0xbf); eax=D(edx+0x69);
            if (eax) {
                eax=cm_fn_0090cfc0(20); ecx=D(edi+0xbf); edx=D(ecx+0x69); B(edx+0x1d)=(s8)(eax+1);
                eax=cm_fn_0090cfc0(20); ecx=D(edi+0xbf); edx=D(ecx+0x69); B(edx+0xf)=(s8)(eax+1);
                eax=cm_fn_0090cfc0(20); ecx=D(edi+0xbf); edx=D(ecx+0x69); B(edx+0x16)=(s8)(eax+1);
                eax=cm_fn_0090cfc0(20); ecx=D(edi+0xbf); edx=D(ecx+0x69); B(edx+0x20)=(s8)(eax+1);
                eax=D(edi+0xbf); ecx=D(eax+0x69);
                if (B(ecx+0x20)<5) {
                    eax=cm_fn_0090cfc0(20); edx=D(edi+0xbf); ecx=D(edx+0x69); B(ecx+0x20)=(s8)(eax+1);
                }
            }
        }
    }
    cm_fn_0059f300(self,edi,delta,saved,2);
    B(esi+0x165)=0;
    return 1;
}
