/* CM0102 3.9.68, preferred-image VA 008c7330, base 00400000.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Preserve the incidental full EAX result and the original volatile reloads. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL __attribute__((regparm(2)))
cm_fn_00945333(u32 eax_in, u32 edx_in, u32 base, u32 count, u32 width, u32 compare);

u32 CM_THISCALL cm_fn_008c7330(u32 self)
{
    u32 eax=D(self+0x93eu), edx=0, edi=0, iteration=0;
    u32 table, map, row, record, ecx, age, difference;
    if ((s32)eax<=0) return eax;
    do {
        table=D(self+0x93au);
        map=D(self+0x86eu);
        eax=D(table+edx);
        row=D(0x00ae23bcu)+eax*110u;
        eax=D(row);
        record=map+eax*8u;
        if (!(B(record)&0x20u)) B(record)=0;
        table=D(self+0x93au);
        record=table+edx;
        age=W(0x00ae2c92u);
        difference=age-W(record+8u);
        if ((s32)difference>4) {
            eax=D(record+10u);
            edi=eax*145u;
            eax=D(0x00ae23a8u)+edi*2u;
            D(row+0x1eu)=eax;
            ecx=D(row);
            eax=D(self+0x86eu)+ecx*8u;
            B(eax)=B(eax)&0xdfu;
            ecx=D(self+0x93au);
            D(edx+ecx)=0xffffffffu;
            edi=1;
        } else {
            eax=D(row+0x61u);
            if (eax==0) {
                eax=D(row);
                ecx=D(self+0x86eu);
                eax=ecx+eax*8u;
                B(eax)=B(eax)&0xdfu;
                eax=D(self+0x93au);
                D(edx+eax)=0xffffffffu;
                edi=1;
            }
        }
        eax=iteration;
        ecx=D(self+0x93eu);
        eax+=1u;
        edx+=14u;
        iteration=eax;
    } while ((s32)eax<(s32)ecx);
    if (edi!=0) {
        edx=D(self+0x93au);
        (void)cm_fn_00945333(eax,edx,edx,ecx,14u,0x008e55c0u);
        eax=D(self+0x93eu)-1u;
        if ((s32)eax>=0) {
            edx=D(self+0x93au);
            ecx=edx+eax*14u;
            do {
                u32 count;
                if (D(ecx)!=0xffffffffu) break;
                count=D(self+0x93eu);
                ecx-=14u;
                count-=1u;
                eax-=1u;
                D(self+0x93eu)=count;
            } while ((s32)eax>=0);
        }
    }
    return eax;
}
