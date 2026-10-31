/* Preferred-image VA00519fc0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ECX receiver, RET0. Preserve full incidental EAX, including entry EAX
   on the empty path and the high word retained by partial AX loads. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_THISCALL cm_fn_00905790(u32 self, u32 id, u32 second, u32 third);

u32 CM_CDECL __attribute__((regparm(3))) cm_fn_00519fc0(
    u32 eax_in, u32 edx_in __attribute__((unused)), u32 self)
{
    u32 eax=eax_in, edx=0, rows, primary=0xffffffffu, secondary=0xffffffffu;
    u32 vector, data, first=0xffffffffu, other=0xffffffffu, i=0, table, pointer, id, receiver;
    u16 ignored_first, ignored_second, count;
    if ((s16)W(self+0x3cu)<=0) return eax;
    rows=D(self+0xa3u);
    for (;;) {
        eax=(u32)(s32)(s16)(u16)edx;
        eax=eax*13u;
        eax=(eax&0xffff0000u)|W(rows+eax*8u+7u);
        if ((u16)eax==0x82) primary=edx;
        else if ((u16)eax==0x96) secondary=edx;
        if ((u16)primary!=0xffff && (u16)secondary!=0xffff) break;
        ++edx;
        if ((s16)(u16)edx>=(s16)W(self+0x3cu)) break;
    }
    if ((u16)primary==0xffff || (u16)secondary==0xffff) return eax;
    vector=D(self+0xadu);
    eax=(u32)(s32)(s16)(u16)secondary;
    eax=D(vector+eax*4u);
    ignored_first=W(eax);
    eax=(eax&0xffff0000u)|W(eax+2u);
    ignored_second=(u16)eax;
    eax=(u32)(s32)(s16)(u16)primary;
    count=W(rows+eax*104u+0x1au);
    if ((s16)count<=0) { eax=other; goto emit; }
    data=D(vector+eax*4u);
    for (;;) {
        eax=(u32)(s32)(s16)(u16)i;
        edx=eax*53u;
        eax=(eax&0xffff0000u)|W(edx+data);
        edx+=data;
        if ((u16)eax!=ignored_first && (u16)eax!=ignored_second) {
            if ((u16)eax==0xffff) return eax;
            if (first!=0xffffffffu) {
                eax=(u32)(s32)(s16)(u16)i;
                edx=eax*53u;
                eax=(u32)(s32)(s16)W(edx+data);
                table=D(self+0xa7u);
                pointer=D(table+eax*6u);
                eax=D(pointer);
                goto emit;
            }
            table=D(self+0xa7u);
            eax=(u32)(s32)(s16)(u16)eax;
            eax=D(table+eax*6u);
            first=D(eax);
        }
        eax=(eax&0xffff0000u)|W(edx+2u);
        if ((u16)eax!=ignored_first && (u16)eax!=ignored_second) {
            if ((u16)eax==0xffff) return eax;
            if (first!=0xffffffffu) {
                eax=(u32)(s32)(s16)(u16)i;
                edx=eax*53u;
                eax=(u32)(s32)(s16)W(edx+data+2u);
                table=D(self+0xa7u);
                pointer=D(table+eax*6u);
                eax=D(pointer);
                goto emit;
            }
            eax=(u32)(s32)(s16)(u16)eax;
            edx=eax*3u;
            eax=D(self+0xa7u);
            pointer=D(eax+edx*2u);
            first=D(pointer);
        }
        ++i;
        if ((s16)(u16)i>=(s16)count) break;
    }
    eax=other;
emit:
    pointer=D(self+4u);
    id=D(pointer);
    receiver=D(0x00de1f58u);
    return cm_fn_00905790(receiver,id,first,eax);
}
