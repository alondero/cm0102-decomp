#include "cmre.h"

/* Preferred-image VA 006d47e0, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Full EAX preserves the upper bits on AL-only exits. When mode is nonzero,
   the original cached comparison pointer is an unwritten scratch DWORD.
   The 48ce70/48ce90 boundaries ignore the two physical pushed arguments;
   keep those pushes and the original conditional calls explicit. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_CDECL cm_fn_0048ce70(u32,u32);
extern u32 CM_CDECL cm_fn_0048ce90(u32,u32);
extern u8 CM_CDECL cm_fn_00540490(u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_CDECL cm_fn_006d47e0(u32 first,volatile u32 second_arg,
                           volatile u32 mode_arg)
{
    u8 scratch[0x20c];
    u32 local=(u32)scratch,second,compare,key,base,result,a,b,name,ext,line;
    u32 version,mode;
    u8 step,match;
    if (first==0) {
        name=local+0x10c; ext=local+0xc; line=0x4a2c;
        goto error;
    }
    second=second_arg;
    if (second==0) {
        name=local+0xc; ext=local+0x10c; line=0x4a32;
        goto error;
    }
    mode=(u8)mode_arg;
    if (mode==0) {
        compare=D(second+0x87);
        if (compare==0) {
            key=D(0x009cf17c);
            base=D(0x00ae23ec);
            compare=base+key*58;
        }
    } else compare=D(local+4);
    B(local+3)=0;
    for (;;) {
        result=(u32)(s32)(s8)B(local+3);
        if (result==0) {
            b=D(first+0x87);
            a=D(first+0x83);
            D(local+8)=a;
            D(local+4)=b;
        } else if (result==1 || result==2) {
            u32 offset=result==1?0x8b:0x93;
            a=D(first+offset);
            b=D(first+offset+4);
            D(local+8)=a;
            D(local+4)=b;
            if (a==0) {result=2; goto done;}
            if (b==0) {result=(a&0xffffff00u)|2; goto done;}
        } else {
            result=((result-2)&0xffffff00u)|1;
            goto done;
        }
        result=cm_fn_0048ce70(a,b);
        if (result!=0)
            result=cm_fn_0048ce90(local+8,local+4);
        step=B(local+3);
        mode=(u8)mode_arg;
        step=(u8)(step+1);
        B(local+3)=step;
        if (mode==0) {
            b=D(local+4);
            if (b==0) continue;
            a=D(local+8);
            if (a==0) continue;
            match=cm_fn_00540490(b,compare);
            if (match!=0) continue;
        }
        a=D(local+8);
        if (a==0) {result=1; goto done;}
        b=D(local+4);
        if (b==0) result=1;
        else result=(b&0xffffff00u)|B(local+3);
        goto done;
    }
error:
    cm_fn_00944cff(0x00a15884,0,0,name,ext);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,name,ext,line);
    result=cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
    D(0x00b67a34)=0;
    result&=0xffffff00u;
done:
    return result;
}
