#include "cmre.h"
/* Preferred-image VA005b7810, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL4/fullEAX, scratch210: first textL0/secondL8/diagnosticsL10,L110.
   Keep formatter arity, bit priority and the late abbreviated-mode read. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_CDECL cm_fn_0066f4e0(u32,u32,...);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_CDECL cm_fn_005b7810(volatile u32 flags_arg,volatile u32 destination_arg,volatile u32 capacity_arg,volatile u32 abbreviated_arg)
{
    u8 scratch[0x210];
    u32 l=(u32)scratch,destination,flags,format,abbreviated,remaining,pointer,length,name,extension,line,version;
    u8 byte;
    destination=destination_arg;
    if (!destination || (s32)capacity_arg<10) {
        name=l+0x10; extension=l+0x110; line=0xd0c;
        goto diagnostic;
    }
    flags=flags_arg;
    B(destination)=0;
    if (flags&1) {
        cm_fn_0066f4e0(destination,0x009b7610,0x00ad9c64);
        return 1;
    }
    if (flags&2) format=0x009b75e4;
    else if (flags&4) format=0x009b75c0;
    else if (flags&8) format=0x009b7588;
    else if (flags&0x10) format=0x009b7508;
    else if (flags&0x20) format=0x009b7544;
    else if (flags&0x40) format=0x009b74b4;
    else {
        name=l+0x10; extension=l+0x110; line=0xd47;
        goto diagnostic;
    }
    cm_fn_0066f4e0(l,format,0x00ad9c64);
    abbreviated=abbreviated_arg;
    if (abbreviated) {
        remaining=0xffffffff;
        pointer=l;
        do {
            byte=B(pointer);
            ++pointer; --remaining;
            if (!byte) break;
        } while (remaining);
        length=(~remaining)-1;
        if (length>1) {
            cm_fn_00944c9f(destination,l);
            return 1;
        }
    }
    if (flags&0x80) format=0x009b744c;
    else if (flags&0x100) format=0x009b7428;
    else if (flags&0x200) format=0x009b7428;
    else if (flags&0x400) format=0x009b7428;
    else if (flags&0x800) format=0x009b746c;
    else {
        name=l+0x110; extension=l+0x10; line=0xd79;
        goto diagnostic;
    }
    cm_fn_0066f4e0(l+8,format,0x00ad9c64);
    cm_fn_00944c9f(destination,0x0099ec04,l,l+8);
    return 1;
diagnostic:
    cm_fn_00944cff(0x009cc3dc,0,0,name,extension);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,name,extension,line);
    cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
    D(0x00b67a34)=0;
    return 0;
}
