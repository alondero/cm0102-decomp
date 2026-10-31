#include "cmre.h"
/* Preferred-image VA00403350, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Physical entry ECX=self, incidental EAX retained by early AX/AL writes,
   RET0. regparm3 explicitly carries EAX/unusedEDX/ECX; no stack arguments.
   Scratch20c topL0/heightL4/cumulativeL8/diagnosticsLc,L10c.
   Keep wrapping32-bit products, signed IDIV, zero-divisor faults, cached
   sum bounds and original count reloads during the endpoint passes. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern void CM_THISCALL cm_fn_00403600(u32,s32,s32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);
#define DIAG(name,extension,line) do { \
    cm_fn_00944cff(0x009873c0,0,0,(name),(extension)); \
    value=cm_fn_0090d130(); \
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,value,(name),(extension),(line)); \
    value=cm_fn_005e8290(0x009870e0,0x00ae24d0,0); \
    D(0x00b67a34)=0; \
} while (0)

u32 CM_CDECL __attribute__((regparm(3))) cm_fn_00403350(u32 eax_in,u32 edx_in,u32 self)
{
    u8 scratch[0x20c];
    u32 l=(u32)scratch,value,index,base,flags,left,top,width,height,first,second;
    u32 columns,rows,sum,bound,cumulative,product,endpoint;
    u16 word;
    (void)edx_in;
    value=(eax_in&0xffff0000u)|W(self+0x20e);
    if ((u16)value!=0xffff) {
        index=(u32)(s32)(s16)value;
        word=W(self+0xb72);
        base=D(self+0x208);
        D(self+0xbe9)=0;
        value=index*99;
        W(base+value*4+0x7c)=word;
    }
    flags=D(self+0x18);
    W(self+0xb76)=0;
    if (flags&0x80000) return value;
    value=(value&0xffffff00u)|B(self+0xbab);
    if ((u8)value==0) return value;
    if ((u8)value>30) {
        DIAG(l+0x10c,l+0xc,0x1f1);
        return value;
    }
    value=(value&0xffffff00u)|B(self+0xbac);
    if ((u8)value==0) return value;
    if ((u8)value>30) {
        DIAG(l+0xc,l+0x10c,0x1fb);
        return value;
    }
    rows=(u8)value;
    if (flags&1) {
        left=0; D(l)=0;
    } else {
        left=2;
        if ((flags&0x10) || rows==1) D(l)=2;
        else D(l)=8;
    }
    width=D(self+8);
    top=D(l);
    width-=left*2;
    first=D(self);
    width-=first;
    second=D(self+4);
    height=0-top*2-second;
    second=D(self+0xc);
    ++width;
    height+=second+1;
    first=D(self+0x14)+1;
    D(l+4)=height;
    if ((s32)first>(s32)rows) {
        top=D(l);
        cm_fn_00403600(self,(s32)left,(s32)top);
        width-=22;
    }
    sum=0; index=0;
    columns=B(self+0xbab);
    bound=columns-1;
    if ((s32)bound>=0) {
        do {
            sum+=B(self+index+0xbad);
            ++index;
        } while ((s32)index<=(s32)bound);
    }
    cumulative=0; index=0;
    if ((s32)bound>=0) {
        do {
            cumulative+=B(self+index+0xbad);
            D(l+8)=cumulative;
            if (index==0) D(self+0x1c)=left;
            else {
                value=D(self+index*4+0x108)+2;
                D(self+index*4+0x1c)=value;
            }
            bound=(u32)B(self+0xbab)-1;
            if ((s32)index<(s32)bound) {
                product=cumulative*width;
                endpoint=(u32)((s32)product/(s32)sum)+left-1;
                D(self+index*4+0x10c)=endpoint;
                cumulative=D(l+8);
            } else D(self+index*4+0x10c)=width+left-1;
            ++index;
            bound=(u32)B(self+0xbab)-1;
        } while ((s32)index<=(s32)bound);
    }
    sum=0;
    bound=(u32)B(self+0xbac)-1;
    value=0;
    if ((s32)bound>=0) {
        do {
            sum+=B(self+value+0xbcb);
            ++value;
        } while ((s32)value<=(s32)bound);
    }
    cumulative=0; index=0;
    if ((s32)bound>=0) {
        do {
            cumulative+=B(self+index+0xbcb);
            if (index==0) {
                top=D(l);
                D(self+0x94)=top;
            } else {
                value=D(self+index*4+0x180);
                top=D(l);
                value+=2;
                D(self+index*4+0x94)=value;
            }
            bound=(u32)B(self+0xbac)-1;
            if ((s32)index<(s32)bound) {
                product=cumulative*D(l+4);
                value=(u32)((s32)product/(s32)sum)+top-1;
            } else value=D(l+4)+top-1;
            D(self+index*4+0x184)=value;
            bound=(u32)B(self+0xbac)-1;
            ++index;
        } while ((s32)index<=(s32)bound);
    }
    return value;
}
