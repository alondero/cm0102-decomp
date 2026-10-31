#include "cmre.h"

/* Preferred-image VA 004a4dc0, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   FCOMP/C3 treats unordered as equal. Preserve forward 11-DWORD+BYTE
   copies and the final allocation-address read after the epilogue POP. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_CDECL cm_fn_00945333(u32,u32,u32,u32);
extern long double CM_THISCALL cm_fn_004a3250(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_004ae4f0(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_004a4dc0(u32 self,volatile u32 sort_arg,volatile u32 out_arg,
                              volatile u32 limit,volatile u32 mask_arg)
{
    u8 scratch[0x210];
    u32 local=(u32)scratch,sort,out,value,base,count,allocation,offset=0;
    u32 destination,source,mask,index,selected,version;
    long double statistic,bound;
    if (D(self+0x19)==0) {
        out=out_arg;
        D(out)=0;
        return 0;
    }
    if ((s32)limit<0) limit=D(self+0x21);
    sort=sort_arg;
    value=B(self+0x25);
    if ((u8)value!=(u8)sort) {
        if ((u8)sort==0x11 && B(self+0x18)!=0xff) {
            B(self+0x18)=0xff;
            D(0x00adae10)=self;
            count=D(self+0x14);
            base=D(self+0xc);
            cm_fn_00945333(base,count,0x76,0x004ad1a0);
        }
        B(self+0x25)=(u8)sort;
        D(0x00adae10)=self;
        count=D(self+0x21);
        base=D(self+0x19);
        cm_fn_00945333(base,count,0x2d,0x004ad250);
    }
    value=D(self+0x21);
    out=out_arg;
    D(out)=value;
    if ((s32)value>0) {
        do {
            count=D(out);
            base=D(self+0x19);
            statistic=cm_fn_004a3250(self,base+count*45-45,sort);
            bound=CM_GLOBAL(volatile float,0x00968970);
            if (!(__builtin_isunordered(statistic,bound) || statistic==bound)) break;
            value=D(out)-1;
            D(out)=value;
        } while ((s32)value>0);
    }
    value=D(out);
    if (value==0) return 0;
    allocation=cm_fn_00944e46(value*45);
    D(local+0xc)=allocation;
    if (allocation==0) {
        cm_fn_00944cff(0x0099ca94,0,0,local+0x110,local+0x10);
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,local+0x110,local+0x10,0xdc6);
        cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        D(out)=0;
        return 0;
    }
    value=D(out);
    D(local)=0;
    D(local+8)=0;
    if ((s32)value>0) {
        D(local+4)=allocation;
        do {
            selected=D(local);
            value=limit;
            if ((s32)selected>=(s32)value) break;
            mask=mask_arg;
            if ((u16)mask!=0xffff) {
                base=D(self+0x19);
                count=D(0x00ae23c4);
                value=D(base+offset);
                value=cm_fn_004ae4f0(count+value*70);
                if ((u16)(value&mask)==0) goto next;
            }
            value=sort_arg;
            base=D(self+0x19);
            statistic=cm_fn_004a3250(self,base+offset,value);
            bound=CM_GLOBAL(volatile float,0x00968970);
            if (__builtin_isunordered(statistic,bound) || statistic==bound) goto next;
            base=D(self+0x19);
            destination=D(local+4);
            source=base+offset;
            value=D(local+4);
            for (index=0;index<11;++index) {
                count=D(source);
                D(destination)=count;
                source+=4;
                destination+=4;
            }
            selected=D(local)+1;
            value+=45;
            B(destination)=B(source);
            out=out_arg;
            D(local)=selected;
            D(local+4)=value;
next:
            value=D(local+8);
            count=D(out);
            ++value;
            offset+=45;
            D(local+8)=value;
        } while ((s32)value<(s32)count);
    }
    value=D(local);
    D(out)=value;
    return D(local+0xc);
}
