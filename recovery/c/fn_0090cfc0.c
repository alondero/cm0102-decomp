/* Physical EDX:EAX return projection of preferred-image VA 0x0090cfc0.
   Entry EDX is real input on the zero-bound path; this interface makes no
   historical 64-bit source-type claim. Semantic routine recovery only:
   ECX preservation/output and caller composition remain explicit boundaries.
   Preferred-image VA0090CFC0, base00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Two register-prefix positions describe EAX (unused), actual EDX input;
   bound remains at entry ESP+4. The return describes physical EDX:EAX.
   ECX's original large-path output is NOT represented by this C interface. */
#include "cmre.h"
typedef unsigned long long u64;
typedef signed long long s64;
extern u32 CM_CDECL cm_fn_00946a04(void);
#define D(a) CM_GLOBAL(volatile u32,(a))

__attribute__((regparm(2), noinline, target("no-sse3")))
unsigned long long CM_CDECL cm_fn_0090cfc0(u32 entry_eax, u32 entry_edx, u32 bound)
{
    u32 cursor,end,value,sample,result,recursive,limit;
    u32 product,magic_high,precorrected,recursive_eax,recursive_bound;
    s32 remainder,quotient;
    long double lower,live;
    volatile long long converted;
    (void)entry_eax; /* Position only: the original never reads entry EAX. */
    if(!bound)return (u64)entry_edx<<32;
    cursor=D(0x00de1f60);
    end=D(0x00de2798);
    cursor+=4u;
    D(0x00de1f60)=cursor;
    if(cursor>end) {
        D(0x00de1f60)=0x00aa2454;
        value=cm_fn_00946a04();
        cursor=D(0x00de1f60);
        value&=0xffffu;
        D(0x00de1f5c)=value;
    } else value=D(0x00de1f5c);
    if((s32)bound<=65535 && (s32)bound>=-65535) {
        value+=D(cursor);
        remainder=(s32)value%(s32)bound;
        result=(u32)remainder;
        if((s32)bound<=0)result=0u-result;
        return ((u64)result<<32)|result;
    }
    sample=D(cursor);
    value+=sample;
    remainder=(s32)value%65535;
    lower=(long double)remainder;
    live=(long double)(s32)bound;
    live*=CM_GLOBAL(volatile double,0x00970a70);
    live*=lower;
    converted=(long long)live; /* Native __ftol CALL VA0090D04E. */
    result=(u32)converted;
    product=bound*10u;
    magic_high=(u32)(((s64)(s32)0x80008001u*(s64)(s32)product)>>32);
    precorrected=(u32)((s32)(magic_high+product)>>15);
    recursive_eax=precorrected>>31;
    recursive_bound=precorrected+recursive_eax;
    /* Physical inputs at native CALL0090D06E: EAX sign bit, EDX bound,
       and the same bound in the first native stack argument. */
    recursive=(u32)cm_fn_0090cfc0(recursive_eax,recursive_bound,recursive_bound);
    value=recursive+5u;
    quotient=(s32)value/10;
    result+=(u32)quotient;
    limit=bound-1u;
    if((s32)result>(s32)limit)result=limit;
    return ((u64)(u32)quotient<<32)|result;
}
