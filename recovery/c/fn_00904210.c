#include "cmre.h"
/* Preferred-image VA00904210/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS2/RET8/full incidental EAX, scratch264. Keep byte table writes,
   late arguments, signed byte counters, wrapping addresses and self-1 write
   when a nation is not found. 9050d0 is actual STD6/RET24; its fourth
   argument is read only as BYTE at90516a. Unwritten upper bytes of L50
   are unobserved; no initialization of them is inferred. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_CDECL cm_fn_004afd80(u32,u32);
extern u32 CM_STDCALL cm_fn_009050d0(u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_00904210(u32 self,volatile u32 first_arg,volatile u32 second_arg)
{
    u8 scratch[0x264];
    u32 l=(u32)scratch,eax,nation,base,head,number,index,cursor,remaining;
    u32 value,pointer,first,second,key;
    u8 row,selector,byte,limit,attempt;
    B(l+0x11)=2;B(l+0x12)=2;B(l+0x29)=2;
    nation=D(0x009cf6e8);base=D(0x00ae23d0);
    D(l+0x34)=self;D(l+0x30)=0;
    B(l+8)=0;B(l+9)=0x10;B(l+0xa)=0x12;B(l+0xb)=0x13;B(l+0xc)=0x12;
    B(l+0x10)=0;B(l+0x13)=0;B(l+0x28)=0;B(l+0x2a)=1;B(l+0x2b)=0;
    B(l+0x18)=0;B(l+0x19)=1;B(l+0x1a)=1;B(l+0x1b)=0;
    B(l+0x20)=0;B(l+0x21)=1;B(l+0x22)=0;B(l+0x23)=1;
    B(l+0x38)=0;B(l+0x39)=0;B(l+0x3a)=1;B(l+0x3b)=1;
    B(l+0x48)=0;B(l+0x49)=0;B(l+0x4a)=1;B(l+0x4b)=0;
    B(l+0x54)=0;B(l+0x55)=0;B(l+0x56)=0;B(l+0x57)=1;
    B(l+0x40)=0;B(l+0x41)=0;B(l+0x42)=0;B(l+0x43)=0;
    eax=cm_fn_004afd80(base+nation*107u,0);index=0xffffffffu;
    if (!eax) goto diagnostic;
    head=D(eax+0x53);if(!head) goto diagnostic;
    if (D(eax+0x1db)!=0xffffffffu) goto diagnostic;
    nation=D(0x009cf6e8);B(eax+0x1df)=1;D(eax+0x1db)=nation;
    eax=0;limit=B(self+0x438);D(l+0x30)=1;
    if ((s8)limit>0) {
        number=D(head);cursor=self;
        do {
            if (D(cursor)==number) {index=eax;break;}
            ++eax;cursor+=18;
        } while ((s32)eax<(s32)(s8)limit);
    }
    B(l+9)=15;pointer=self+index*18u+0x11;
    byte=B(pointer);B(pointer)=(u8)(byte+1);eax=pointer;
    goto start_rows;
diagnostic:
    cm_fn_00944cff(0x00aa1a1c,0,0,l+0x164,l+0x64);
    eax=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,eax,l+0x164,l+0x64,0x35d);
    eax=cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
    D(0x00b67a34)=0;
start_rows:
    eax=(eax&0xffffff00u)|B(self+0x438);row=0;B(l+7)=0;
    if ((s8)(u8)eax<=0) return eax;
outer:
    if ((s8)row<3) {value=D(l+0x10);eax&=0xffffff00u;D(l)=value;}
    else if ((s8)row<6) {eax=D(l+0x28);D(l)=eax;eax&=0xffffff00u;}
    else if ((s8)row<9) {value=D(l+0x18);eax&=0xffffff00u;D(l)=value;}
    else if ((s8)row<10) {eax=D(l+0x20);D(l)=eax;eax&=0xffffff00u;}
    else if ((s8)row<15) {value=D(l+0x38);eax&=0xffffff00u;D(l)=value;}
    else if ((s8)row<16) {eax=D(l+0x48);D(l)=eax;eax&=0xffffff00u;}
    else if ((s8)row<29) {value=D(l+0x54);eax&=0xffffff00u;D(l)=value;}
    else {eax=D(l+0x40);D(l)=eax;eax=(eax&0xffffff00u)|1;}
    B(l+4)=(u8)eax;B(l+0x50)=1;
    eax=(u32)(s32)(s8)row;D(l+0x5c)=eax;
    D(l+0x60)=self+eax*18u;
inner:
    pointer=D(l+0x60);attempt=B(l+0x50);key=D(pointer);
    first=first_arg;second=second_arg;
    eax=cm_fn_009050d0(first,second,key,attempt,0,0);
    selector=1;
    do {
        index=(u32)(s32)(s8)selector;
        if (B(l+index)) {
            if (B(l+8+index)) goto selected;
            if ((s32)index+1<5) {byte=B(l+index+1);B(l+index+1)=(u8)(byte+1);}
            byte=B(l+index);B(l+index)=(u8)(byte-1);
        }
        selector=(u8)(selector+1);
    } while ((s8)selector<5);
    goto after_selection;
selected:
    index=D(l+0x5c);
    if (!eax) goto rejected;
    head=D(eax+0x53);if(!head) goto rejected;
    if (D(eax+0x1db)!=0xffffffffu) goto rejected;
    nation=D(0x009cf6e8);B(eax+0x1df)=selector;D(eax+0x1db)=nation;
    remaining=D(l+0x30);++remaining;D(l+0x30)=remaining;
    if (index==0xffffffffu) {
        limit=B(self+0x438);eax=0;
        if ((s8)limit>0) {
            number=D(head);cursor=D(l+0x34);
            do {
                if (D(cursor)==number) {index=eax;break;}
                ++eax;cursor+=18;
            } while ((s32)eax<(s32)(s8)limit);
            self=D(l+0x34);
        }
    }
    pointer=self+index*18u+0x11;
    byte=B(pointer);B(pointer)=(u8)(byte+1);
    eax=(u32)(s32)(s8)selector;
    byte=B(l+8+eax);B(l+8+eax)=(u8)(byte-1);
    eax=l+eax;byte=B(eax);B(eax)=(u8)(byte-1);
    goto after_selection;
rejected:
    eax=l+(u32)(s32)(s8)selector;byte=B(eax);B(eax)=(u8)(byte-1);
after_selection:
    eax=(eax&0xffffff00u)|B(l+0x50);
    eax=(eax&0xffffff00u)|(u8)((u8)eax+1);B(l+0x50)=(u8)eax;
    if ((s8)(u8)eax<8) goto inner;
    eax=(eax&0xffffff00u)|B(l+7);limit=B(self+0x438);
    eax=(eax&0xffffff00u)|(u8)((u8)eax+1);B(l+7)=(u8)eax;
    if ((s8)(u8)eax<(s8)limit) {row=B(l+7);goto outer;}
    return eax;
}
