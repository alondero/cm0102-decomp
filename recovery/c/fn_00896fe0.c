#include "cmre.h"

/* Preferred-image VA 00896fe0, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Keep late mode loads, per-DWORD table-pointer reloads, forward copies and
   the unconditional selected backup-slot clear on the restore path. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_THISCALL cm_fn_008982b0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_00896fe0(u32 self,u32 key,volatile u32 second_arg,
                              volatile u32 mode_arg,volatile u32 restore_arg)
{
    u8 scratch[0x200];
    u32 local=(u32)scratch,second,index,mode,owner,day,previous,slot,offset,base,value,pointer,i;
    if (key==0) {
        cm_fn_00944cff(0x00a8b908,0,0,local+0x100,local);
        value=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,value,local+0x100,local,0x3d5);
        cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        return 0;
    }
    second=second_arg;
    index=cm_fn_008982b0(self,key,second);
    if (index==0xffffffffu) return 0;
    if (restore_arg==0) {
        owner=(u32)(s32)(s16)W(0x00b77c7a);
        owner=D(0x00b74c26+owner*768);
        if (owner!=0) {
            mode=mode_arg;
            if (mode!=0) {
                day=D(0x00ae2364);
                previous=D(owner);
                day=day-previous-1;
                if ((s32)day>=0 && (s32)day<16) {
                    slot=self+day*92;
                    if (D(slot+0x38)==0) B(self+0x5f0)=(u8)(B(self+0x5f0)+1);
                    offset=index*145;
                    D(slot+0x30)=key;
                    D(slot+0x34)=second;
                    D(slot+0x38)=1;
                    pointer=slot+0x3c;
                    for (i=0;i<0x50;i+=4) {
                        base=D(self+0x601);
                        value=D(base+i+offset+2);
                        D(pointer)=value;
                        pointer+=4;
                    }
                }
            }
        }
        offset=index*145;
        for (i=0;i<0x50;i+=4) {
            base=D(self+0x601);
            D(base+i+offset+2)=0xffffffffu;
        }
        return 1;
    }
    mode=mode_arg;
    if (mode!=0) {
        owner=(u32)(s32)(s16)W(0x00b77c7a);
        owner=D(0x00b74c26+owner*768);
        if (owner==0) return 1;
        day=D(0x00ae2364);
        previous=D(owner);
        day=day-previous-1;
        if ((s32)day<0 || (s32)day>=16) return 1;
        slot=self+day*92;
        if (D(slot+0x38)!=0 && D(slot+0x30)==key && D(slot+0x34)==second) {
            offset=index*145;
            pointer=slot+0x3c;
            for (i=0;i<0x50;i+=4) {
                base=D(self+0x601);
                value=D(pointer);
                D(base+i+offset+2)=value;
                pointer+=4;
            }
        }
        if (D(slot+0x38)!=0) B(self+0x5f0)=(u8)(B(self+0x5f0)-1);
        D(slot+0x38)=0;
        return 1;
    }
    if (B(self+0x5f0)!=0) {
        pointer=self+0x38;
        for (i=0;i<16;++i) {
            if (D(pointer)!=0 && D(pointer-8)==key && D(pointer-4)==second) {
                D(pointer)=0;
                B(self+0x5f0)=(u8)(B(self+0x5f0)-1);
            }
            pointer+=0x5c;
        }
    }
    return 1;
}
