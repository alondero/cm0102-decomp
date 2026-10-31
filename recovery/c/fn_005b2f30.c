#include "cmre.h"

/* Preferred-image VA 005b2f30, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch20c: saved DI low WORD/inner counter L0, outer counter L4,
   saved first index L8, diagnostics Lc/L10c. Keep late BYTE argument
   rereads, per-row target reloads, and all original swap calls in order. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern void CM_THISCALL cm_fn_005b59a0(u32);
extern void CM_CDECL cm_fn_0090ba90(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_005b2f30(u32 self,volatile u8 first_arg,volatile u8 second_arg)
{
    u8 scratch[0x20c];
    u32 local=(u32)scratch,first,second,left,right,pointer,flag_pointer,i,value,name,extension,line,version;
    u16 saved,other;
    first=first_arg;
    if ((s8)first<0 || (s8)first>10) {
        name=local+0xc; extension=local+0x10c; line=0x1df;
        goto diagnostic;
    }
    second=second_arg;
    if ((s8)second<0 || (s8)second>10) {
        name=local+0x10c; extension=local+0xc; line=0x1e7;
        goto diagnostic;
    }
    saved=W(self+first*2+0x12d);
    other=W(self+second*2+0x12d);
    D(local+8)=first;
    W(local)=saved;
    if (saved!=other) {
        W(self+first*2+0x12d)=other;
        cm_fn_005b59a0(self);
        B(self+first+0x143)=1;
        pointer=self+0x12d;
        flag_pointer=self+0x143;
        for (i=0;i<11;++i) {
            if (i!=first_arg) {
                saved=W(pointer);
                if (saved==W(self+second*2+0x12d)) B(flag_pointer)=0;
                saved=W(local);
            }
            pointer+=2;
            ++flag_pointer;
        }
    }
    if (W(self+second*2+0x12d)!=saved) {
        W(self+second*2+0x12d)=saved;
        cm_fn_005b59a0(self);
        B(self+second+0x143)=1;
        pointer=self+0x12d;
        flag_pointer=self+0x143;
        for (i=0;i<11;++i) {
            if (i!=second_arg && W(pointer)==saved) B(flag_pointer)=0;
            pointer+=2;
            ++flag_pointer;
        }
    }
    cm_fn_0090ba90(self+first*2+0x14e,self+second*2+0x14e,2);
    D(local+4)=3;
    left=self+first*96+0x194;
    right=self+second*96+0x194;
    do {
        D(local)=4;
        do {
            cm_fn_0090ba90(left-0x30,right-0x30,4);
            cm_fn_0090ba90(left,right,4);
            value=D(local);
            right+=4;
            left+=4;
            --value;
            D(local)=value;
        } while (value!=0);
        value=D(local+4)-1;
        D(local+4)=value;
    } while (value!=0);
    first=D(local+8);
    cm_fn_0090ba90(self+first*8+0x188b,self+second*8+0x188b,8);
    cm_fn_0090ba90(self+first+0x5b4,self+second+0x5b4,1);
    cm_fn_0090ba90(self+first*4+0x5bf,self+second*4+0x5bf,4);
    D(self+0x187f)=0;
    return 1;
diagnostic:
    cm_fn_00944cff(0x009cc3dc,0,0,name,extension);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,name,extension,line);
    cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
    D(0x00b67a34)=0;
    return 0;
}
