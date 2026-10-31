#include "cmre.h"

/* Preferred-image VA 004a2090, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Preserve each alias-sensitive field reload and the last two distinct load
   orders. Qsort's count<2 branch leaves EAX untouched: regparm(1) carries
   the caller's EAX explicitly while retaining its four original stack args. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
extern u32 CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 __attribute__((regparm(1))) cm_fn_00945333(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

#define FIELD(source_offset,target_offset) do { \
    count=D(self+8); \
    source=D(self+0xc); \
    value=W(source+offset+(source_offset)); \
    destination=D(self); \
    W(destination+count*28+(target_offset))=(u16)value; \
} while (0)

u32 CM_THISCALL cm_fn_004a2090(u32 self)
{
    u8 scratch[0x200];
    u32 local=(u32)scratch,result,count,index,offset,source,destination,value,version;
    result=D(self);
    if (result!=0) {
        cm_fn_009452ca(result);
        D(self)=0;
        D(self+4)=0;
        D(self+8)=0;
    }
    result=D(self+0x14);
    if (result==0) return result;
    D(self+4)=result;
    D(self+8)=0;
    result=cm_fn_00944e46(result*28);
    D(self)=result;
    if (result==0) {
        cm_fn_00944cff(0x0099ca94,0,0,local+0x100,local);
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,local+0x100,local,0x5f7);
        result=cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        D(self+4)=0;
        return result;
    }
    value=D(self+0x14);
    index=0;
    if ((s32)value>0) {
        offset=0;
        do {
            count=D(self+8);
            source=D(self+0xc);
            value=D(source+offset);
            destination=D(self);
            D(destination+count*28)=value;
            FIELD(0x19,4);
            FIELD(0x1d,6);
            FIELD(0x1f,8);
            FIELD(0x2d,0xa);
            FIELD(0x31,0xc);
            FIELD(0x33,0xe);
            FIELD(0x41,0x10);
            FIELD(0x45,0x12);
            FIELD(0x47,0x14);
            FIELD(0x55,0x16);
            count=D(self+8);
            source=D(self+0xc);
            destination=D(self);
            value=W(source+offset+0x59);
            offset+=0x76;
            W(destination+count*28+0x18)=(u16)value;
            count=D(self+8);
            source=D(self+0xc);
            value=W(source+offset-0x1b);
            destination=D(self);
            W(destination+count*28+0x1a)=(u16)value;
            count=D(self+8)+1;
            ++index;
            D(self+8)=count;
            value=D(self+0x14);
        } while ((s32)index<(s32)value);
    }
    count=D(self+8);
    destination=D(self);
    D(self+4)=count;
    return cm_fn_00945333(count,destination,count,28,0x008b4e00);
}
