#include "cmre.h"
/* Preferred-image VA00903cc0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/full EAX=third selected row's +d pointer. Scratch204:
   selected BYTEsL3,L2,L0/counterL1/diagnosticsL4,L104.
   Keep overwritten preliminary stores, the >=50 row+d fallthrough,
   signed BYTE counters and every original RNG call/retry. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_00903cc0(u32 self)
{
    u8 scratch[0x204];
    u32 l=(u32)scratch,pointer,id,constant,year,eax,first_pointer,second_pointer;
    u8 row=0,limit,first,second,third,counter,byte;
    B(l+3)=0xff;B(l+2)=0xff;limit=B(self+0x438);B(l)=0xff;B(l+1)=0;
    if ((s8)limit>0) {
        do {
            constant=D(0x009cf2fc);pointer=self+(u32)(s32)(s8)row*18u;
            id=D(pointer);
            if (id==constant) B(l+3)=row;
            else if (id==D(0x009cf470)) B(l+2)=row;
            else if (id==D(0x009cf240)) B(l)=row;
            if (id==D(0x009cf3a0)) B(pointer+0xc)=0;
            if ((s8)row<3) B(pointer+0xc)=4;
            else if ((s8)row<6) B(pointer+0xc)=3;
            else if ((s8)row<15) B(pointer+0xc)=2;
            else if ((s8)row<48) B(pointer+0xc)=1;
            else B(pointer+0xc)=0;
            id=D(pointer);constant=D(0x009cf3a0);
            if (id==constant || id==D(0x009cf450)) B(pointer+0xd)=0;
            if ((s8)row<6) B(pointer+0xd)=3;
            else if ((s8)row<8) B(pointer+0xd)=4;
            else if ((s8)row<15) B(pointer+0xd)=2;
            else if ((s8)row<21) B(pointer+0xd)=3;
            else if ((s8)row<48) B(pointer+0xd)=2;
            else if ((s8)row<50) B(pointer+0xd)=1;
            id=D(pointer);constant=D(0x009cf3d4);
            if (id==constant || id==D(0x009cf450) || id==D(0x009cf20c) ||
                id==D(0x009cf258) || id==D(0x009cf3a0)) goto zero;
            if (id==D(0x009cf45c)) {B(pointer+0xe)=1;goto next;}
            constant=D(0x009cf300);year=W(0x00ae2c92);
            if (id==constant && year==2001) {B(pointer+0xe)=4;goto next;}
            if (id==D(0x009cf314) && year==2001) {B(pointer+0xe)=3;goto next;}
            constant=D(0x009cf434);
            if ((id==constant || id==D(0x009cf408) || id==D(0x009cf488) ||
                 id==D(0x009cf448)) && year==2001) goto zero;
            if (id==D(0x009cf2e4) && year!=2001) {B(pointer+0xe)=1;goto next;}
            if ((id==D(0x009cf360) || id==constant) && year==2001) {
                B(pointer+0xe)=1;goto next;
            }
            counter=B(l+1);
            if ((s8)counter<16) goto two;
            if (id==D(0x009cf244) && year==2000) goto two;
            if ((s8)counter<43) {
                ++counter;B(pointer+0xe)=1;B(l+1)=counter;goto next;
            }
            ++counter;B(pointer+0xe)=0;B(l+1)=counter;goto next;
two:
            ++counter;B(pointer+0xe)=2;B(l+1)=counter;goto next;
zero:
            B(pointer+0xe)=0;
next:
            limit=B(self+0x438);row=(u8)(row+1);
        } while ((s8)row<(s8)limit);
    }
    if (W(0x00ae2c92)!=2001) goto randomize;
    if (B(l+3)==0xff || B(l+2)==0xff || B(l)==0xff) {
        cm_fn_00944cff(0x00aa1a1c,0,0,l+0x104,l+4);
        eax=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,eax,l+0x104,l+4,0x285);
        cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
    }
    first=B(l+3);second=B(l+2);third=B(l);
    goto validate;
randomize:
    limit=B(self+0x438);first=(u8)cm_fn_0090cfc0((u32)(s32)(s8)limit);
    limit=B(self+0x438);eax=cm_fn_0090cfc0((u32)(s32)(s8)limit);
    limit=B(self+0x438);B(l)=(u8)eax;
    eax=cm_fn_0090cfc0((u32)(s32)(s8)limit);second=B(l);third=(u8)eax;
validate:
    if (first==0xff || second==0xff || third==0xff ||
        first==second || first==third || second==third) goto randomize;
    constant=D(0x009cf360);
    if (D(self+(u32)(s32)(s8)first*18u)==constant) goto randomize;
    if (D(self+(u32)(s32)(s8)second*18u)==constant) goto randomize;
    if (D(self+(u32)(s32)(s8)third*18u)==constant) goto randomize;
    first_pointer=self+(u32)(s32)(s8)first*18u+0xd;
    eax=self+(u32)(s32)(s8)third*18u+0xd;
    byte=B(first_pointer);B(first_pointer)=(u8)(byte+1);
    second_pointer=self+(u32)(s32)(s8)second*18u+0xd;
    byte=B(second_pointer);B(second_pointer)=(u8)(byte+1);
    byte=B(eax);B(eax)=(u8)(byte+1);
    return eax;
}
