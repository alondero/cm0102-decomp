#include "cmre.h"
/* Preferred-image VA 005b7580, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch2c8: text L0/L64, diagnostic buffers Lc8/L1c8.
   Flags retain priority and formatter arity, including bit40's extra string. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_CDECL cm_fn_0066f4e0(u32,u32,...);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_CDECL cm_fn_005b7580(volatile u32 flags_arg,volatile u32 destination_arg,volatile u32 capacity_arg,volatile u32 diagnose_arg)
{
    u8 scratch[0x2c8];
    u32 l=(u32)scratch,i,destination,flags,diagnose,format,name,extension,line,version;
    u8 initial=B(0x00ad9c64);
    B(l)=initial;
    for (i=0;i<24;++i) D(l+1+i*4)=0;
    W(l+0x61)=0; B(l+0x63)=0;
    B(l+0x64)=initial;
    for (i=0;i<24;++i) D(l+0x65+i*4)=0;
    destination=destination_arg;
    W(l+0xc5)=0; B(l+0xc7)=0;
    if (!destination || (s32)capacity_arg<100) {
        name=l+0xc8; extension=l+0x1c8; line=0xc80;
        goto diagnostic;
    }
    flags=flags_arg;
    B(destination)=0;
    if (flags&1) {
        cm_fn_0066f4e0(destination,0x009b7600);
        return 1;
    }
    diagnose=diagnose_arg;
    if (flags&2) format=0x009b75dc;
    else if (flags&4) format=0x009b75b0;
    else if (flags&8) format=0x009b756c;
    else if (flags&0x10) format=0x009b74f8;
    else if (flags&0x20) format=0x009b7528;
    else if (flags&0x40) {
        cm_fn_0066f4e0(l,0x009b7484,0x00ad9c64);
        goto second_text;
    } else {
        if (diagnose) {
            name=l+0x1c8; extension=l+0xc8; line=0xcbd;
            goto diagnostic;
        }
        goto second_text;
    }
    cm_fn_0066f4e0(l,format);
second_text:
    if (flags&0x80) format=0x009b7444;
    else if (flags&0x100) format=0x009ccacc;
    else if (flags&0x200) format=0x009b7420;
    else if (flags&0x400) format=0x009ccaa0;
    else if (flags&0x800) format=0x009b7464;
    else {
        if (diagnose) {
            name=l+0xc8; extension=l+0x1c8; line=0xcea;
            goto diagnostic;
        }
        goto join;
    }
    cm_fn_0066f4e0(l+0x64,format);
join:
    cm_fn_0066f4e0(destination,0x009ccab0,l,l+0x64);
    return 1;
diagnostic:
    cm_fn_00944cff(0x009cc3dc,0,0,name,extension);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,name,extension,line);
    cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
    D(0x00b67a34)=0;
    return 0;
}
