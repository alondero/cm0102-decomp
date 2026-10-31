#include "cmre.h"
/* Preferred-image VA008e4f20, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STD1/full EAX. Scratchf0: sumL0,offsetL4,remainingL8,quotientLc,
   messageL10. The aggregate is observed only as DI/a signed WORD atL0;
   upper EBX residue in MOVSX BX additions does not enter the calculation.
   Keep both lookups, original null read, signed division and 64-bit payment.
   No message destruction or final SEH state store occurs in the original. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_THISCALL cm_fn_0077bbe0(u32,u32,u8);
extern u32 CM_THISCALL cm_fn_00785790(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007861e0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);
extern u32 CM_THISCALL cm_fn_005a0590(u32,u32);
extern u32 CM_THISCALL cm_fn_00599800(u32,u32,u32);

u32 CM_STDCALL cm_fn_008e4f20(volatile u32 record_arg)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0xf0];
    u32 l=(u32)scratch,record,id,base,competition,value,entry,sum,i,person;
    u32 owner,lookup,club,weight,amount,high,threshold,offset,remaining;
    u8 age;
    frame.state=0xffffffff; frame.handler=0x009648ab;
    frame.prev=CM_FS(volatile u32,0);
    CM_FS(volatile u32,0)=(u32)&frame;
    record=record_arg;
    id=D(record+0xc);
    D(l)=0;
    base=D(0x00ae23bc);
    competition=base+id*110;
    age=B(competition+0x18);
    value=(base&0xffffff00u)|age;
    if ((s8)age<23) goto done;
    cm_fn_0077bbe0(l+0x10,0xfc3,0);
    id=D(competition);
    base=D(0x00ae2d0c);
    frame.state=0;
    entry=base+id*79+0x12;
    sum=0;
    for (i=0;i<5;++i) {
        if (D(entry-4)!=0xffffffff) sum+=(u32)(s32)(s8)B(entry);
        entry+=5;
    }
    value=D(record+0x1b);
    D(l)=sum;
    D(l+0xc)=(u32)((s32)value/20);
    if ((u16)sum==0) D(l)=1;
    cm_fn_00785790(l+0x10,0,id);
    person=D(competition+4); value=D(person+0x33);
    cm_fn_00785790(l+0x10,1,value);
    person=D(competition+8); value=D(person+0x33);
    cm_fn_00785790(l+0x10,2,value);
    person=D(competition+0xc); value=D(person+0x33);
    cm_fn_00785790(l+0x10,3,value);
    value=D(record+8); cm_fn_00785790(l+0x10,4,value);
    owner=D(0x00ae1050);
    lookup=cm_fn_004cf5a0(owner,competition);
    D(0x00ae1080)=lookup;
    value=0xffffffff;
    if (lookup!=0 && (B(lookup+0x35)&0x3f)!=5) {
        id=D(lookup+4);
        base=D(0x00ae23b4);
        if (base+id*581!=0) {
            owner=D(0x00ae1050);
            lookup=cm_fn_004cf5a0(owner,competition);
            D(0x00ae1080)=lookup;
            if (lookup!=0 && (B(lookup+0x35)&0x3f)!=5) {
                id=D(lookup+4);
                base=D(0x00ae23b4);
                value=D(base+id*581);
            } else {
                value=D(0);
            }
        }
    }
    cm_fn_00785790(l+0x10,7,value);
    D(l+4)=0;
    D(l+8)=5;
    do {
        id=D(competition);
        offset=D(l+4);
        base=D(0x00ae2d0c);
        entry=base+id*79+offset;
        id=D(entry+0xe);
        if (id!=0xffffffff) {
            base=D(0x00ae23b4);
            age=B(entry+0x12);
            club=base+id*581;
            weight=((s8)age<1)?1:(u32)(s32)(s8)age;
            value=D(l+0xc);
            sum=(u32)(s32)(s16)W(l);
            amount=(u32)((s32)value/(s32)sum);
            amount*=weight;
            record=record_arg;
            high=((s32)amount<0)?0xffffffff:0;
            id=D(record+8);
            owner=D(0x00ae2a38);
            lookup=cm_fn_005a0590(owner,base+id*581);
            cm_fn_00599800(lookup,amount,high);
            threshold=(u32)(s32)(s16)W(club+0x80)*11-10000;
            if ((s32)amount>(s32)threshold) {
                cm_fn_00785790(l+0x10,5,amount);
                value=D(club);
                cm_fn_00785790(l+0x10,6,value);
                owner=D(0x00b7431c);
                cm_fn_007861e0(owner,l+0x10,club);
            }
        }
        offset=D(l+4);
        remaining=D(l+8);
        offset+=5;
        --remaining;
        D(l+4)=offset;
        D(l+8)=remaining;
    } while (remaining!=0);
    value=remaining;
done:
    CM_FS(volatile u32,0)=frame.prev;
    return value;
}
