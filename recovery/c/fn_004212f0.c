#include "cmre.h"

/* Preferred-image VA 004212f0, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Retain packed six-byte rows, late table reloads, duplicate fallback,
   2001 pointer swap and distinct allocation/lookup failure returns. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_004afd80(u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_004212f0(u32 self)
{
    u8 scratch[0x200];
    u32 local=(u32)scratch,array,value,key,base,other,line,version,result;
    W(self+0x3e)=2;
    array=cm_fn_00944e46(12);
    D(self+0xa7)=array;
    if (array==0) {
        cm_fn_00944cff(0x00989ccc,0,0,local+0x100,local);
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,local+0x100,local,0xc6);
        cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        return 0xffffffffu;
    }
    key=D(0x009cf59c);
    base=D(0x00ae23d0);
    value=cm_fn_004afd80(base+key*107,0);
    array=D(self+0xa7);
    D(array)=value;
    array=D(self+0xa7);
    if (D(array)==0) {
        line=0xcf;
        goto diagnostic;
    }
    key=D(0x009cf700);
    base=D(0x00ae23d0);
    value=cm_fn_004afd80(base+key*107,0);
    array=D(self+0xa7);
    D(array+6)=value;
    array=D(self+0xa7);
    other=D(array+6);
    if (other==0) {
        line=0xd8;
        goto diagnostic;
    }
    if (D(array)==other) {
        key=D(0x009cf59c);
        base=D(0x00ae23d0);
        value=cm_fn_004afd80(base+key*107,1);
        array=D(self+0xa7);
        D(array+6)=value;
        array=D(self+0xa7);
        if (D(array+6)==0) {
            line=0xe4;
            goto diagnostic;
        }
    }
    B(array+4)=2;
    array=D(self+0xa7);
    B(array+5)=0;
    array=D(self+0xa7);
    B(array+10)=1;
    array=D(self+0xa7);
    B(array+11)=0;
    if (W(self+0x40)==0x7d1) {
        array=D(self+0xa7);
        other=D(array+6);
        value=D(array);
        D(array)=other;
        array=D(self+0xa7);
        D(array+6)=value;
    }
    return 1;
diagnostic:
    cm_fn_00944cff(0x00989ccc,0,0,local,local+0x100);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,local,local+0x100,line);
    result=cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
    (void)result;
    D(0x00b67a34)=0;
    return 0;
}
