/* Preferred-image VA008c47d0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/RET4. Preserve the physical EAX returned by original list cleanup.
   The 10h scratch list starts at L0; its iterator DWORD is at LA. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_0068d020(u32 list);
extern u32 CM_THISCALL cm_fn_0068d040(u32 list);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 list);
extern u32 CM_THISCALL cm_fn_008c5cf0(u32 self, u32 record, u32 output, u32 mode);
extern u32 CM_THISCALL cm_fn_008cbf70(u32 self, u32 record);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 record);
extern u32 CM_THISCALL cm_fn_0084aeb0(u32 self, u32 club, u32 record);
extern u32 CM_THISCALL cm_fn_008c6220(u32 self, u32 record, s8 days, u8 tick);

u32 CM_THISCALL cm_fn_008c47d0(u32 self, u32 argument)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x10];
    u32 local=(u32)scratch, index, map, head, record, league, club, base, value;
    frame.state=0xffffffffu;
    frame.handler=0x00963c88u;
    frame.prev=CM_FS(u32,0);
    CM_FS(u32,0)=(u32)&frame;
    (void)cm_fn_0068d020(local);
    map=D(self+0x86eu);
    frame.state=0;
    index=D(argument);
    if (B(map+index*8u+1u)!=0) {
        (void)cm_fn_008c5cf0(self,argument,local,0);
        value=D(local+2u);
        D(local+0xau)=value;
        head=cm_fn_0068d1b0(local);
        while (head!=0) {
            index=D(head);
            base=D(self);
            record=D(base+index*4u);
            if (B(record+0x2cu)==0x13)
                (void)cm_fn_008cbf70(self,record);
            index=D(record+0xcu);
            B(record+0x2cu)=0xf;
            B(record+0x2eu)=3;
            base=D(0x00ae23bcu);
            league=base+index*110u;
            base=D(0x00ae1050u);
            value=cm_fn_004cf5a0(base,league);
            base=D(0x00ae23b4u);
            D(0x00ae1080u)=value;
            club=value!=0 ? base+D(value+4u)*581u : 0;
            index=D(record+8u);
            value=base+index*581u;
            base=D(self+0x856u);
            (void)cm_fn_0084aeb0(base,value,league);
            head=D(record+4u);
            if (head!=0) {
                value=D(head+2u);
                D(head+0xau)=value;
                head=D(record+4u);
                head=cm_fn_0068d1b0(head);
                while (head!=0) {
                    index=D(head);
                    base=D(0x00ae23bcu);
                    league=base+index*110u;
                    base=D(self+0x856u);
                    (void)cm_fn_0084aeb0(base,club,league);
                    head=D(record+4u);
                    head=cm_fn_0068d1b0(head);
                }
            }
            (void)cm_fn_008c6220(self,record,10,0);
            head=cm_fn_0068d1b0(local);
        }
    }
    frame.state=0xffffffffu;
    value=cm_fn_0068d040(local);
    CM_FS(u32,0)=frame.prev;
    return value;
}
