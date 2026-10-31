#include "cmre.h"
/* Preferred-image VA007cad30, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/full EAX; scratch204 scan counterL0/diagnosticsL4,L104.
   Four original passes, six-byte rows, signed WORD scan comparisons and
   signed BYTE final-pass tests. Preserve field reloads between row stores,
   the pass-specific global-read order and allocation retention on failure. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_007cad30(u32 self)
{
    u8 scratch[0x204];
    u32 l=(u32)scratch,selected=0,value,pass,total,entry,offset,pointer,target,id;
    u32 index,base,take;
    if (W(self+0x40)==2001) W(self+0x3e)=36;
    else W(self+0x3e)=34;
    value=(u32)(s32)(s16)W(self+0x3e);
    value=cm_fn_00944e46(value*6);
    D(self+0xa7)=value;
    if (value==0) {
        cm_fn_00944cff(0x00a785d4,0,0,l+0x104,l+4);
        value=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,value,l+0x104,l+4,0x168);
        cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        return 0xffffffff;
    }
    for (pass=0;pass<4;++pass) {
        total=D(0x00ae235c);
        entry=D(0x00ae23b4);
        D(l)=0;
        if ((s32)total<=0) continue;
        offset=selected*6;
        do {
            pointer=D(entry+0x57);
            take=0;
            if (pass==0) {
                target=D(0x009cf85c);
                if (pointer!=0 && D(pointer)==target && (s32)selected<(s32)(s16)W(self+0x3e)) {
                    pointer=D(entry+0x5b);
                    if (pointer==0) take=1;
                    else {
                        id=D(pointer);
                        if (id!=D(0x009cf858) && id!=target) take=1;
                    }
                }
            } else if (pointer!=0) {
                if (pass==1) target=D(0x009cf85c);
                else target=D(0x009cf858);
                if (D(pointer)==target && (s32)selected<(s32)(s16)W(self+0x3e)) {
                    if (pass==1) {
                        pointer=D(entry+0x5b);
                        if (pointer!=0) {
                            id=D(pointer);
                            if (id==D(0x009cf858) || id==target) take=1;
                        }
                    } else if (pass==2) {
                        if ((s8)B(entry+0x5f)>7) take=1;
                    } else {
                        if ((s8)B(entry+0x5f)<8) take=1;
                    }
                }
            }
            if (take) {
                base=D(self+0xa7);
                ++selected;
                D(base+offset)=entry;
                base=D(self+0xa7); B(base+offset+4)=0;
                base=D(self+0xa7); B(base+offset+5)=0;
                offset+=6;
            }
            index=D(l);
            total=D(0x00ae235c);
            ++index; entry+=0x245;
            D(l)=index;
        } while ((s32)(s16)index<(s32)total);
    }
    value=(u32)(s32)(s16)W(self+0x3e);
    return selected==value;
}
