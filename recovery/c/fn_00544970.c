/* CM0102 3.9.68, preferred-image VA00544970, base00400000.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL0; indexed caller8283d9 kills EAX before observation. Numeric original
   helpers and comparator remain boundaries; no shared-source composition.
   53bd10 reads its third argument as a signed BYTE: the unknown EDX high bits
   forwarded after544bfa are dead inside that original callee. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32,(u32)(a))
#define B(a) CM_GLOBAL(volatile u8,(u32)(a))
extern u32 CM_CDECL cm_fn_00540a50(u32,u32,u32);
extern u32 CM_CDECL cm_fn_0053c190(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0053bd10(u32,u32,s8,u32,u32,u32);
/* Original qsort has four stack arguments. Incidental return EAX/EDX are dead
   here; its accepted register-prefix reconstruction is not composed. */
extern void CM_CDECL cm_fn_00945333(u32,u32,u32,u32);

static u32 count_staff(u32 address,u32 slots)
{
    u32 count=0,person,limit,id;
    do {
        person=D(address);
        if(person) {
            limit=D(0x00ae2364u);
            id=D(person);
            if((s32)id<(s32)limit)++count;
        }
        address+=4u;
    } while(--slots);
    return count;
}

static void transfer_staff(u32 original,u32 partner,u32 offset,u32 slots,s8 role)
{
    u32 count=count_staff(original+offset,slots);
    u32 address=partner+offset,remaining=slots,person,limit,id;
    do {
        person=D(address);
        if(person) {
            limit=D(0x00ae2364u);
            id=D(person);
            if((s32)id<(s32)limit) {
                cm_fn_0053c190(partner,person,0,1,0);
                if((s32)count<(s32)slots) {
                    ++count;
                    cm_fn_0053bd10(original,person,role,0x00ae2c90u,1,0);
                }
            }
        }
        address+=4u;
    } while(--remaining);
}

void CM_CDECL cm_fn_00544970(void)
{
    /* Original L0..Lf: partner, current club, ordinal, helper direction.
       The last cell is initialized only by the original helper. */
    u32 scratch[4];
    u32 total=D(0x00ae235cu),club=D(0x00ae23b4u);
    u32 partner,address,remaining,person,id,limit,players,ordinal=0;
    scratch[2]=0;
    scratch[1]=club;
    if((s32)total<=0)return;
    do {
        partner=cm_fn_00540a50(club,(u32)&scratch[3],1);
        scratch[0]=partner;
        if(partner && scratch[3]) {
            limit=D(0x00ae2364u);
            players=0;
            address=club+0xd7u;
            remaining=50;
            do {
                person=D(address);
                if(person && (s32)D(person)<(s32)limit)++players;
                address+=4u;
            } while(--remaining);
            address=partner+0xd7u;
            cm_fn_00945333(address,50,4,0x00540130u);
            remaining=50;
            do {
                person=D(address);
                if(person) {
                    limit=D(0x00ae2364u);
                    id=D(person);
                    if((s32)id<(s32)limit) {
                        ++players;
                        if((s32)players>50)
                            cm_fn_0053c190(partner,person,0,1,0);
                    }
                }
                address+=4u;
            } while(--remaining);
            transfer_staff(club,partner,0x19fu,5,8);
            transfer_staff(club,partner,0x1b3u,7,9);
            transfer_staff(club,partner,0x1cfu,3,10);
            person=D(club+0xcfu);
            if(person) {
                person=D(partner+0xcfu);
                if(person) {
                    cm_fn_0053c190(partner,person,0,1,0);
                    goto secondary_single;
                }
            }
            person=D(partner+0xcfu);
            if(person) {
                cm_fn_0053c190(partner,person,0,1,0);
                id=B(person+0x3du);
                cm_fn_0053bd10(club,person,(s8)id,0,1,0);
            }
secondary_single:
            person=D(club+0xd3u);
            if(person) {
                person=D(partner+0xd3u);
                if(person)cm_fn_0053c190(partner,person,0,1,0);
            }
        }
        total=D(0x00ae235cu);
        ++ordinal;
        club+=0x245u;
        scratch[2]=ordinal;
        scratch[1]=club;
    } while((s32)ordinal<(s32)total);
}
