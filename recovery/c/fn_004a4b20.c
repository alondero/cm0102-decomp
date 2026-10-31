#include "cmre.h"
/* Preferred-image VA004a4b20, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS3/fullEAX, scratch20c: counterL0/bufferL4/selfL8/diagLc,L10c.
   FCOMP C3 tests equality or unordered. Keep packed118-byte copy order. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
#define F(a) CM_GLOBAL(volatile float,(a))
extern u32 CM_CDECL cm_fn_00945333(u32,u32,u32,u32);
extern long double CM_STDCALL cm_fn_004a2ec0(u32,s8);
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_THISCALL cm_fn_00448170(u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_004a4b20(u32 self,volatile u32 selector_arg,volatile u32 count_arg,volatile u32 maximum_arg)
{
    u8 scratch[0x20c];
    u32 l=(u32)scratch,eax,maximum,selector,count_ptr,value,records,count,source,destination,offset,i,counter,version;
    u8 byte;
    volatile long double result,zero;
    records=D(self+0xc);
    D(l+8)=self;
    if (!records) {
        count_ptr=count_arg; D(count_ptr)=0;
        return 0;
    }
    maximum=maximum_arg;
    if ((s32)maximum<0) maximum=D(self+0x14);
    selector=selector_arg;
    byte=B(self+0x18);
    if (byte!=(u8)selector) {
        B(self+0x18)=(u8)selector;
        D(0x00adae10)=self;
        count=D(self+0x14); records=D(self+0xc);
        cm_fn_00945333(records,count,0x76,0x004ad1a0);
    }
    count=D(self+0x14);
    if ((s32)count>=(s32)maximum) count=maximum;
    count_ptr=count_arg;
    D(count_ptr)=count;
    if ((u8)selector!=4 && (u8)selector!=5 && (u8)selector!=3 && (s32)count>0) {
        do {
            count=D(count_ptr);
            records=D(self+0xc);
            result=cm_fn_004a2ec0(records+count*0x76-0x76,(s8)selector);
            zero=(long double)F(0x00968970);
            if (!(result==zero || __builtin_isunordered(result,zero))) break;
            count=D(count_ptr)-1;
            D(count_ptr)=count;
        } while ((s32)count>0);
    }
    count=D(count_ptr);
    if (!count) return 0;
    eax=cm_fn_00944e46(count*0x76);
    D(l+4)=eax;
    if (!eax) {
        cm_fn_00944cff(0x0099ca94,0,0,l+0x10c,l+0xc);
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,l+0x10c,l+0xc,0xd6c);
        cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        D(count_ptr)=0;
        return 0;
    }
    count=D(count_ptr);
    D(l)=0;
    if ((s32)count<=0) return eax;
    offset=0; destination=eax;
    for (;;) {
        records=D(self+0xc); source=records+offset;
        value=D(source); D(destination)=value;
        byte=B(source+4); B(destination+4)=byte;
        byte=B(source+5); B(destination+5)=byte;
        value=D(source+6); D(destination+6)=value;
        for (i=0;i<5;++i) {byte=B(source+0xa+i); B(destination+0xa+i)=byte;}
        for (i=0;i<5;++i) {byte=B(source+0xf+i); B(destination+0xf+i)=byte;}
        for (i=0;i<5;++i) {byte=B(source+0x14+i); B(destination+0x14+i)=byte;}
        cm_fn_00448170(destination+0x19,source+0x19);
        cm_fn_00448170(destination+0x2d,source+0x2d);
        cm_fn_00448170(destination+0x41,source+0x41);
        cm_fn_00448170(destination+0x55,source+0x55);
        byte=B(source+0x69); offset+=0x76; B(destination+0x69)=byte;
        for (i=0x6a;i<=0x73;++i) {byte=B(source+i); B(destination+i)=byte;}
        byte=B(source+0x74);
        count_ptr=count_arg;
        B(destination+0x74)=byte;
        byte=B(source+0x75);
        counter=D(l);
        B(destination+0x75)=byte;
        count=D(count_ptr);
        ++counter; destination+=0x76;
        D(l)=counter;
        if ((s32)counter>=(s32)count) break;
        self=D(l+8);
    }
    return D(l+4);
}
