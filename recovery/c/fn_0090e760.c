#include "cmre.h"

/* Preferred-image VA 0090e760, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Preserve the full EAX of each exit: ordinary offsets return the second
   computed address, ID-2 returns the first output-slot pointer, ID-3 the
   second slot pointer, and other negative IDs retain partial AX-over-EAX. */
#define D(a) CM_GLOBAL(volatile u32,(a))
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_STDCALL cm_fn_0090e760(volatile u32 id_arg,volatile u32 named_arg,
                              volatile u32 first_arg,volatile u32 output_a,
                              volatile u32 output_b)
{
    u8 scratch[0x200];
    u32 local=(u32)scratch,result,mode,offset,base,a,b,version;
    s16 id;
    result=named_arg;
    if (result==0) {
        id=(s16)id_arg;
        if (id<0) {
            cm_fn_00944cff(0x00ad439c,0,0,local+0x100,local);
            version=cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,
                           local+0x100,local,0x5e6);
            result=cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
            D(0x00b67a34)=0;
            return result;
        }
        base=D(0x00ae2364);
        offset=((u32)(s32)id+36)*25;
        a=output_a;
        D(a)=base+offset;
        base=D(0x00ae236c);
        b=output_b;
        result=offset+base;
        D(b)=result;
        return result;
    }
    mode=first_arg;
    id=(s16)id_arg;
    result=(mode&0xffff0000u)|(u16)id;
    if (mode!=0) {
        if (id>=0) {
            offset=(u32)(s32)id*50;
            base=D(0x00ae2364);
            a=output_a;
            D(a)=offset+base;
            base=D(0x00ae236c);
            b=output_b;
            result=offset+base;
            D(b)=result;
            return result;
        }
        if (id==-2) {
            base=D(0x00ae2364);
            a=output_a;
            D(a)=base+0x320;
            base=D(0x00ae236c);
            b=output_b;
            D(b)=base+0x320;
            return a;
        }
        if (id==-3) {
            base=D(0x00ae2364);
            a=output_a;
            D(a)=base+0x352;
            base=D(0x00ae236c);
            b=output_b;
            D(b)=base+0x352;
            return b;
        }
    } else {
        if (id>=0) {
            offset=(u32)(s32)id*50;
            base=D(0x00ae2364);
            a=output_a;
            D(a)=offset+base+25;
            base=D(0x00ae236c);
            b=output_b;
            result=offset+base+25;
            D(b)=result;
            return result;
        }
        if (id==-2) {
            base=D(0x00ae2364);
            a=output_a;
            D(a)=base+0x339;
            base=D(0x00ae236c);
            b=output_b;
            D(b)=base+0x339;
            return a;
        }
        if (id==-3) {
            base=D(0x00ae2364);
            a=output_a;
            D(a)=base+0x36b;
            base=D(0x00ae236c);
            b=output_b;
            D(b)=base+0x36b;
            return b;
        }
    }
    return result;
}
