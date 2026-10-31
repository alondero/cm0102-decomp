#include "cmre.h"

/* Preferred-image VA 005b7ab0, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL3/fullEAX; read WORD selector only after destination/capacity guards,
   then clear destination before dispatch. Default copies the scanned global
   string forward in DWORDs followed by BYTEs, including its terminator. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_CDECL cm_fn_0066f4e0(u32,u32,...);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_CDECL cm_fn_005b7ab0(volatile u16 type_arg,u32 destination,volatile u32 capacity)
{
    u8 scratch[0x200];
    u32 local=(u32)scratch,type,format,version,source,remaining,length,value,words,tail;
    if (destination==0 || (s32)capacity<100) {
        cm_fn_00944cff(0x009cc3dc,0,0,local+0x100,local);
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,local+0x100,local,0xd96);
        cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        return 0;
    }
    type=type_arg;
    B(destination)=0;
    switch (type) {
    case 1: format=0x009b7600; break;
    case 2: format=0x009b75dc; break;
    case 4: format=0x009b75b0; break;
    case 8: format=0x009b756c; break;
    case 0x10: format=0x009b74f8; break;
    case 0x20: format=0x009b7528; break;
    case 0x40:
        cm_fn_0066f4e0(destination,0x009b7484,0x00ad9c64);
        return 1;
    case 0x80: format=0x009b7444; break;
    case 0x100: format=0x009ccacc; break;
    case 0x200: format=0x009b7420; break;
    case 0x400: format=0x009ccaa0; break;
    case 0x800: format=0x009b7464; break;
    case 0x1000: format=0x009ccad8; break;
    default:
        source=0x00ad9c64;
        remaining=0xffffffffu;
        do {
            if (remaining==0) break;
            value=B(source);
            ++source;
            --remaining;
        } while (value!=0);
        length=~remaining;
        source-=length;
        words=length>>2;
        while (words!=0) {
            value=D(source);
            D(destination)=value;
            source+=4;
            destination+=4;
            --words;
        }
        tail=length&3;
        while (tail!=0) {
            value=B(source);
            B(destination)=(u8)value;
            ++source;
            ++destination;
            --tail;
        }
        return 1;
    }
    cm_fn_0066f4e0(destination,format);
    return 1;
}
