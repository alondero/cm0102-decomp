#include "cmre.h"
/* Preferred-image VA 00928860, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch208: signed cutoff BYTE L3, original candidate L4,
   diagnostic extension L8/name L108. Final shuffle always starts at slot8. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_CDECL cm_fn_00945501(u32,u32);
extern u32 CM_CDECL cm_fn_004afd80(u32,u8);
extern u32 CM_CDECL cm_fn_004afb40(u32,s16);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_00928860(u32 self)
{
    u8 scratch[0x208];
    u32 l=(u32)scratch,array,candidate,base,pointer,total,index,selected=0,value,other,version;
    s8 rank,cutoff;
    D(self+0xf2)=12;
    array=cm_fn_00945501(4,12);
    D(self+0xee)=array;
    if (!array) {
        cm_fn_00944cff(0x00ad572c,0,0,l+0x108,l+8);
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,l+0x108,l+8,0x2b3);
        cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        return 0;
    }
    value=D(0x009cf954);
    base=D(0x00ae23d0);
    candidate=cm_fn_004afd80(base+value*107,0);
    D(l+4)=candidate;
    if (candidate) {
        rank=(s8)B(candidate+0x5f);
        B(l+3)=8;
        if (rank>=9) B(l+3)=7;
    } else B(l+3)=7;
    if (candidate) {
        array=D(self+0xee);
        selected=1;
        D(array)=candidate;
    }
    pointer=D(0x00ae23b4);
    total=D(0x00ae235c);
    index=0;
    if ((s32)total>0) {
        do {
            if ((s32)selected>=8) break;
            other=D(pointer+0x5b);
            if (other) {
                value=D(0x009cf94c);
                if (D(other)==value) {
                    rank=(s8)B(pointer+0x5f);
                    cutoff=(s8)B(l+3);
                    if (rank<=cutoff && pointer!=D(l+4)) {
                        array=D(self+0xee);
                        ++selected;
                        D(array+selected*4-4)=pointer;
                        total=D(0x00ae235c);
                    }
                }
            }
            ++index; pointer+=0x245;
        } while ((s32)(s16)index<(s32)total);
    }
    pointer=D(0x00ae23b4);
    index=0;
    if ((s32)total>0) {
        do {
            if ((s32)selected>=8) break;
            other=D(pointer+0x5b);
            if (other) {
                value=D(0x009cf94c);
                if (D(other)==value) {
                    cutoff=(s8)B(l+3);
                    rank=(s8)B(pointer+0x5f);
                    if (rank>cutoff && pointer!=D(l+4)) {
                        array=D(self+0xee);
                        ++selected;
                        D(array+selected*4-4)=pointer;
                        total=D(0x00ae235c);
                    }
                }
            }
            ++index; pointer+=0x245;
        } while ((s32)(s16)index<(s32)total);
    }
    pointer=D(0x00ae23b4);
    index=0;
    if ((s32)total>0) {
        do {
            if ((s32)selected>=8) break;
            other=D(pointer+0x57);
            if (other) {
                value=D(0x009cf94c);
                if (D(other)==value && pointer!=D(l+4)) {
                    array=D(self+0xee);
                    ++selected;
                    D(array+selected*4-4)=pointer;
                    total=D(0x00ae235c);
                }
            }
            ++index; pointer+=0x245;
        } while ((s32)(s16)index<(s32)total);
    }
    array=D(self+0xee);
    cm_fn_004afb40(array,(s16)selected);
    value=D(0x009d0434); base=D(0x00ae23b4);
    array=D(self+0xee); ++selected;
    D(array+selected*4-4)=base+value*0x245;
    value=D(0x009d0430); base=D(0x00ae23b4);
    ++selected; array=D(self+0xee);
    D(array+selected*4-4)=base+value*0x245;
    value=D(0x009d043c); base=D(0x00ae23b4);
    array=D(self+0xee);
    D(array+selected*4)=base+value*0x245;
    value=D(0x009d0438); base=D(0x00ae23b4);
    array=D(self+0xee);
    D(array+selected*4+4)=base+value*0x245;
    array=D(self+0xee);
    cm_fn_004afb40(array+0x20,4);
    return 1;
}
