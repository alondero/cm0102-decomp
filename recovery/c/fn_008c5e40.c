/* Preferred-image VA008c5e40/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS2/RET8/full DWORD count. EBP caches the original record pointer;
   the actual first argument slot is later overwritten with the club id. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 record);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 list);
extern unsigned long long CM_FASTCALL cm_fn_0068d0d0(u32 self, u32 edx_in, u32 source, s16 count);

u32 CM_THISCALL cm_fn_008c5e40(u32 self, volatile u32 record_arg, volatile u32 output_arg)
{
    u32 target=record_arg, index=0, key, limit, map, receiver, found, club;
    u32 slots, record, list, head, source, expected;
    volatile u32 count=0;
    u8 kind;
    if (target!=0) {
        key=D(target);
        limit=D(0x00ae2364u);
        if ((s32)key>=(s32)limit) return 0;
    }
    key=D(target);
    map=D(self+0x86eu);
    if (B(map+key*8u+2u)==0) return 0;
    receiver=D(0x00ae1050u);
    found=cm_fn_004cf5a0(receiver,target);
    D(0x00ae1080u)=found;
    if (found==0) return 0;
    key=D(found+4u);
    receiver=D(0x00ae23b4u);
    club=receiver+key*581u;
    if (club==0) return 0;
    receiver=D(0x00ae1050u);
    found=cm_fn_004cf5a0(receiver,target);
    D(0x00ae1080u)=found;
    if (found!=0) {
        key=D(found+4u);
        receiver=D(0x00ae23b4u);
        club=receiver+key*581u;
    } else club=0;
    key=D(club);
    slots=D(self);
    record_arg=key;
    limit=D(self+4u);
    if ((s32)limit<=0) return count;
    for (;;) {
        record=D(slots);
        if (record!=0) {
            expected=record_arg;
            key=D(record+8u);
            if (key==expected) {
                kind=B(record+0x2cu);
                if (kind==2 || kind==3 || kind==0x14 || kind==0x10 || kind==4)
                    goto next;
                if (kind==0xf && B(record+0x2eu)>=3) goto next;
                list=D(record+4u);
                if (list!=0) {
                    head=D(list+2u);
                    D(list+0xau)=head;
                    record=D(slots);
                    list=D(record+4u);
                    head=cm_fn_0068d1b0(list);
                    while (head!=0) {
                        key=D(head);
                        expected=D(target);
                        if (key==expected) {
                            receiver=output_arg;
                            if (receiver!=0) {
                                source=D(slots)+0x20u;
                                (void)cm_fn_0068d0d0(receiver,source,source,4);
                            }
                            count=count+1u;
                            break;
                        }
                        record=D(slots);
                        list=D(record+4u);
                        head=cm_fn_0068d1b0(list);
                    }
                }
                key=D(target);
                map=D(self+0x86eu);
                expected=B(map+key*8u+2u);
                if (count==expected) break;
            }
        }
next:
        limit=D(self+4u);
        ++index;
        slots+=4u;
        if ((s32)index>=(s32)limit) break;
    }
    return count;
}
