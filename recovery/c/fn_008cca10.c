/* CM0102 3.9.68, preferred-image VA 008cca10, base 00400000.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The 16-byte scratch retains the original overlapping list fields. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
typedef struct { void *prev; void *handler; s32 state; } seh_8cca10;
extern u32 CM_THISCALL cm_fn_0068d020(u32 self);
extern void CM_THISCALL cm_fn_0068d040(u32 self);
extern u32 CM_THISCALL cm_fn_0068d080(u32 self);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
extern u32 CM_THISCALL cm_fn_008c5cf0(u32 self,u32 record,u32 output,u32 flag);
extern u32 CM_THISCALL cm_fn_008c5e40(u32 self,u32 record,u32 output);

u32 CM_THISCALL cm_fn_008cca10(u32 self,u32 record,u32 output)
{
    volatile seh_8cca10 frame;
    u8 scratch[16];
    u32 list=(u32)scratch, eax, ecx, edx, selected, slots, index, limit;
    frame.state=-1;
    frame.handler=(void *)0x00963e98u;
    frame.prev=CM_FS(void *,0);
    CM_FS(void *,0)=(void *)&frame;
    cm_fn_0068d020(list);
    ecx=D(self+0x86eu);
    frame.state=0;
    eax=D(record);
    if (!(B(ecx+eax*8u)&0x10u)) goto absent;
    eax=cm_fn_008c5cf0(self,record,list,0);
    if (eax!=0) {
        eax=D(list+2u);
        D(list+10u)=eax;
        eax=cm_fn_0068d1b0(list);
        while (eax!=0) {
            ecx=D(eax);
            edx=D(self);
            ecx=D(edx+ecx*4u);
            if (B(ecx+0x2cu)==0x13u) {
                selected=D(eax);
                if (selected!=0xffffffffu) goto found;
                break;
            }
            eax=cm_fn_0068d1b0(list);
        }
    }
    cm_fn_0068d080(list);
    eax=cm_fn_008c5e40(self,record,list);
    if (eax!=0) {
        eax=D(list+2u);
        D(list+10u)=eax;
        eax=cm_fn_0068d1b0(list);
        while (eax!=0) {
            ecx=D(eax);
            edx=D(self);
            ecx=D(edx+ecx*4u);
            if (B(ecx+0x2cu)==0x13u) {
                selected=D(eax);
                if (selected!=0xffffffffu) goto found;
                break;
            }
            eax=cm_fn_0068d1b0(list);
        }
    }
    ecx=D(record);
    edx=D(self+0x86eu);
    eax=edx+ecx*8u;
    B(eax)=B(edx+ecx*8u)&0xefu;
absent:
    frame.state=-1;
    cm_fn_0068d040(list);
    ecx=(u32)frame.prev;
    CM_FS(void *,0)=(void *)ecx;
    return 0;
found:
    if (output!=0) {
        limit=D(self+0x928u);
        index=0;
        if ((s32)limit>0) {
            slots=D(self+0x920u);
            ecx=slots+8u;
            do {
                if (D(ecx)==selected) {
                    D(output)=slots+index*12u;
                    break;
                }
                index+=1u;
                ecx+=12u;
            } while ((s32)index<(s32)limit);
        }
    }
    frame.state=-1;
    cm_fn_0068d040(list);
    ecx=(u32)frame.prev;
    CM_FS(void *,0)=(void *)ecx;
    return 1;
}
