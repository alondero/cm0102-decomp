#include "cmre.h"
/* Preferred-image VA 005d5f20, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch208: integer conversion L4, diagnostic extension L8/name L108.
   Keep each x87 arithmetic step in extended precision until its FSTP. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
#define Q(a) CM_GLOBAL(volatile double,(a))
extern u32 CM_THISCALL cm_fn_0054cfb0(u32);
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_CDECL cm_fn_00945333(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_CDECL cm_fn_005d5f20(void)
{
    u8 scratch[0x208];
    u32 l=(u32)scratch,eax,pointer,count,index,buffer,row,lo,hi,next;
    u16 day;
    u8 annual=0,flag;
    volatile long double first,second;
    flag=B(0x009cd218);
    if (flag) {
        count=D(0x00ae2350);
        pointer=D(0x00ae23a8);
        index=0;
        if ((s32)count>0) {
            pointer+=0xd0;
            do {
                first=(long double)Q(pointer-0x28);
                ++index;
                Q(pointer)=(double)first;
                Q(pointer-8)=(double)first;
                Q(pointer-0x10)=(double)first;
                Q(pointer-0x18)=(double)first;
                Q(pointer-0x20)=(double)first;
                count=D(0x00ae2350);
                pointer+=0x122;
            } while ((s32)(s16)index<(s32)count);
        }
        B(0x009cd218)=0;
    }
    day=(u16)D(0x00ae2c90);
    eax=cm_fn_0054cfb0(0x00ae2c90);
    eax=(u32)(s32)(s8)eax;
    if (W(0x009cd200+eax*2)!=day) return eax;
    eax=D(0x00ae2350);
    buffer=cm_fn_00944e46(eax*12);
    if (!buffer) {
        cm_fn_00944cff(0x009cd2a4,0,0,l+0x108,l+8);
        eax=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,eax,l+0x108,l+8,0x156d);
        eax=cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        return eax;
    }
    if (W(0x009cd216)==W(0x00ae2c90)) annual=1;
    count=D(0x00ae2350);
    eax=D(0x00ae23a8);
    index=0;
    if ((s32)count>0) {
        do {
            row=buffer+(u32)(s32)(s16)index*12;
            D(row)=eax;
            first=(long double)Q(eax+0xc0);
            first=first*(long double)Q(0x00968e48);
            second=(long double)Q(eax+0xb8);
            second=second*(long double)Q(0x00968e00);
            first=first+second;
            second=(long double)Q(eax+0xb0);
            second=second*(long double)Q(0x00968918);
            first=first+second;
            second=(long double)Q(eax+0xc8);
            second=second*(long double)Q(0x009689b0);
            first=first+second;
            first=first+(long double)Q(eax+0xd8);
            first=first+(long double)Q(eax+0xd0);
            Q(row+4)=(double)first;
            if (annual) {
                flag=B(eax+0x121);
                if ((s8)flag<12) {
                    D(l+4)=(u32)(s32)(s16)W(eax+0x8e);
                    first=(long double)(s32)D(l+4);
                    D(l+4)=12-(u32)(s32)(s8)flag;
                    first=first*(long double)Q(0x00967888);
                    second=(long double)(s32)D(l+4);
                    first=first*second;
                    first=first*(long double)Q(0x00968930);
                    first=first+(long double)Q(eax+0xd8);
                    Q(eax+0xd8)=(double)first;
                }
                lo=D(eax+0xb8); hi=D(eax+0xbc);
                D(eax+0xb0)=lo; lo=D(eax+0xc0);
                D(eax+0xb4)=hi; hi=D(eax+0xc4);
                D(eax+0xb8)=lo; lo=D(eax+0xc8);
                D(eax+0xbc)=hi; hi=D(eax+0xcc);
                D(eax+0xc0)=lo; lo=D(eax+0xd0);
                D(eax+0xc4)=hi; hi=D(eax+0xd4);
                D(eax+0xc8)=lo; lo=D(eax+0xd8);
                D(eax+0xcc)=hi; hi=D(eax+0xdc);
                D(eax+0xd0)=lo; D(eax+0xd8)=0;
                D(eax+0xd4)=hi; D(eax+0xdc)=0;
                B(eax+0x121)=0;
            }
            count=D(0x00ae2350);
            ++index;
            next=(u32)(s32)(s16)index;
            eax+=0x122;
        } while ((s32)next<(s32)count);
    }
    cm_fn_00945333(buffer,count,12,0x004afba0);
    return cm_fn_009452ca(buffer);
}
