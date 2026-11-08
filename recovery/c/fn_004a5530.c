#include "cmre.h"
/* Preferred-image VA 004a5530, base 00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS3/RET12. Original diagnostic scratch L0/L100 remains contiguous.
   Original callees stay explicit; qsort's EAX/EDX prefix consumes no stack. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004afa40(u32);
extern unsigned long long CM_THISCALL cm_fn_004ae660(u32,u32);
extern unsigned long long CM_CDECL __attribute__((regparm(2))) cm_fn_00945333(u32,u32,u32,u32,u32,u32);
extern long double CM_STDCALL cm_fn_004a2ec0(u32,s8);
extern u32 CM_THISCALL cm_fn_004ad7f0(u32);
extern u32 CM_THISCALL cm_fn_004a5690(u32,u32,u32,u32);

__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_004a5530(u32 self,u32 flag,u32 selector,u32 output)
{
    u8 scratch[0x200];
    u32 l=(u32)scratch,version,cache,stats,prefix,count,first;
    u8 mode;
    volatile long double value,constant;
    long long converted;
    if (!output) {
        cm_fn_00944cff(0x0099ca94,0,0,l+0x100,l);
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,l+0x100,l,0xf08);
        cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        return 0;
    }
    cm_fn_004afa40(output);
    cm_fn_004ae660(output,0x0098ba0c);
    if ((u8)flag==1) cache=self+0x26;
    else {
        cache=self+0x28f;
        if ((u8)selector==8) {
            if (D(cache+0x6b)==0xffffffffu) {
                stats=D(self+0xc);
                if (stats) {
                    mode=B(self+0x18);
                    if (mode!=2) {
                        B(self+0x18)=2;
                        D(0x00adae10)=self;
                        count=D(self+0x14);
                        first=D(self+0xc);
                        prefix=(stats&0xffffff00u)|mode;
                        cm_fn_00945333(prefix,first,first,count,0x76,0x004ad1a0);
                    }
                    stats=D(self+0xc);
                    value=cm_fn_004a2ec0(stats,2);
                    /* Original __ftol call at preferred-image VA 004a562b. */
                    converted=(long long)value;
                    D(cache+0x6b)=(u32)converted;
                    first=D(stats);
                    W(cache+0x73)=0;
                    D(cache+0x6f)=first;
                }
            }
        } else if ((u8)selector==0x12) {
            value=(long double)CM_GLOBAL(volatile float,cache+0x189);
            constant=(long double)CM_GLOBAL(volatile double,0x00968948);
            /* FCOMP C3 is set for equality and unordered operands. */
            if (value==constant || __builtin_isunordered(value,constant))
                cm_fn_004ad7f0(self);
        }
    }
    return cm_fn_004a5690(self,cache,selector,output);
}
