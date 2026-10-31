#include "cmre.h"

/* Preferred-image VA 00800330, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Capacity diagnostics continue. Retain unsigned wrapping checks, late
   argument reads, original buffer orientation and the partial AX send arg. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0077ac30(u32,u32,u32,u32);

#define REPORT(path,name,extension,line) do { \
    cm_fn_00944cff((path),0,0,(name),(extension)); \
    version=cm_fn_0090d130(); \
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,(name),(extension),(line)); \
    result=cm_fn_005e8290(0x009870e0,0x00ae24d0,0); \
    D(0x00b67a34)=0; \
} while (0)

u32 CM_THISCALL cm_fn_00800330(u32 self,volatile u32 word_arg,volatile u32 dword_arg)
{
    u8 scratch[0x200];
    u32 capacity,local=(u32)scratch,offset,buffer,value,result,version,port,owner;
    capacity=D(0x00ae2900);
    D(0x00ae28fc)=0;
    if (capacity<1) {
        REPORT(0x009c3734,local+0x100,local,0x85);
    } else {
        buffer=D(0x00ae28f8);
        B(buffer)=0xf;
        result=D(0x00ae28fc)+1;
        D(0x00ae28fc)=result;
    }
    offset=D(0x00ae28fc);
    capacity=D(0x00ae2900);
    if (offset+2>capacity) {
        REPORT(0x009c3734,local,local+0x100,0x86);
    } else {
        buffer=D(0x00ae28f8);
        value=(u16)word_arg;
        W(buffer+offset)=(u16)value;
        result=D(0x00ae28fc)+2;
        D(0x00ae28fc)=result;
    }
    offset=D(0x00ae28fc);
    capacity=D(0x00ae2900);
    if (offset+4>capacity) {
        REPORT(0x009c3734,local,local+0x100,0x88);
    } else {
        buffer=D(0x00ae28f8);
        value=dword_arg;
        D(buffer+offset)=value;
        result=D(0x00ae28fc)+4;
        D(0x00ae28fc)=result;
    }
    if (D(self+0x7fc)==0x00ca72f5) {
        port=W(0x00ae2914);
        port=(result&0xffff0000u)|port;
        offset=D(0x00ae28fc);
        buffer=D(0x00ae28f8);
        owner=D(self+0x3840);
        result=cm_fn_0077ac30(owner,buffer,offset,port);
        if (result==0) {
            REPORT(0x00a7ddbc,local,local+0x100,0xa0b);
        }
    }
    return result;
}
