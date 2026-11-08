/* CM0102 3.9.68, preferred-image VA008cd450, base00400000.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS4/RET16/full EAX. Numeric callees remain original-code boundaries.
   The original counter bytes and both report buffers share one0x220 scratch
   allocation. The score is stored as double before comparing its extended
   value, and a selected score is cached at the stored double precision. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32,(u32)(a))
#define W(a) CM_GLOBAL(volatile u16,(u32)(a))
#define B(a) CM_GLOBAL(volatile u8,(u32)(a))
extern u32 CM_CDECL cm_fn_00540a50(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);
extern u32 CM_THISCALL cm_fn_004cf6d0(u32,u32);
extern u32 CM_CDECL cm_fn_0053b610(u32);
extern u32 CM_CDECL cm_fn_0053b620(u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern u8 CM_THISCALL cm_fn_008c1780(u32,u32);
extern u32 CM_CDECL cm_fn_00600a50(u32,u32,u8,u32,u32);
extern u32 CM_THISCALL cm_fn_008c5cf0(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_008c5e40(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_008cca10(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0086d800(u32,u32);
extern u32 CM_THISCALL cm_fn_008c1af0(u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern void CM_CDECL cm_fn_005e8290(const char *,const char *,u32);

static void set_category(u32 work,u8 role,u8 value)
{
    if(role==8 || role==15)B(work+2u)=value;
    else if(role==9)B(work+1u)=value;
    else if(role==10)B(work)=value;
    else if(role==6 || role==13)B(work+3u)=value;
}
static void add_category(u32 work,u8 role,u8 delta)
{
    if(role==8 || role==15)B(work+2u)=(u8)(B(work+2u)+delta);
    else if(role==9)B(work+1u)=(u8)(B(work+1u)+delta);
    else if(role==10)B(work)=(u8)(B(work)+delta);
    else if(role==6 || role==13)B(work+3u)=(u8)(B(work+3u)+delta);
}

u32 CM_THISCALL cm_fn_008cd450(u32 self,u32 club,u32 offer,u32 allow,u32 output)
{
    u8 scratch[0x220];
    u32 work=(u32)scratch, mapped,person,node,found,index,base,limit;
    u32 pointer,slots,remaining,selected=0,manager,header;
    u32 version;
    u8 players=0,category,status,role;
    volatile double best=1000000.0,stored;
    long double extended;
    B(work+2u)=0;B(work)=0;B(work+1u)=0;B(work+3u)=0;
    if(club==0) {
        cm_fn_00944cff(0x00a8d514u,0,0,(void *)(work+0x120u),(void *)(work+0x20u));
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u,0x009870e8u,version,work+0x120u,work+0x20u,0x2c3bu);
        cm_fn_005e8290((const char *)0x009870e0u,(const char *)0x00ae24d0u,0);
        D(0x00b67a34u)=0;return 0;
    }
    mapped=cm_fn_00540a50(club,0,1);
    index=D(club);base=D(offer+8u);
    if(index==base) {
        index=D(offer+0xcu);base=D(0x00ae23bcu);person=base+index*110u;
        found=cm_fn_004cf5a0(D(0x00ae1050u),person);
        D(0x00ae1080u)=found;
        if(found && (B(found+0x35u)&0x3fu)==5) {
            found=cm_fn_004cf5a0(D(0x00ae1050u),person);
            D(0x00ae1080u)=found;
            if(found) {
                index=D(found+4u);base=D(0x00ae23b4u);
                found=base+index*581u;
            }
            if(found==club)goto already_present;
        }
        found=cm_fn_004cf6d0(D(0x00ae1050u),person);
        if(found) {
            found=cm_fn_004cf6d0(D(0x00ae1050u),person);
            index=D(found+4u);
            if(index==D(club)) {
already_present:
                players=0xffu;
            }
        }
        if(cm_fn_0053b620(B(offer+0x11u)))players=(u8)(players+1u);
        if(cm_fn_0053b610(B(offer+0x11u)))set_category(work,B(offer+0x11u),1);
        header=D(offer+4u);
        if(header) {
            index=D(header+2u);D(header+0xau)=index;
            node=cm_fn_0068d1b0(D(offer+4u));
            while(node) {
                index=D(node);base=D(0x00ae23bcu);
                role=B(base+index*110u+0x3du);person=base+index*110u;
                if(cm_fn_0053b620(role))players=(u8)(players-1u);
                if(cm_fn_0053b610(B(person+0x3du)))add_category(work,B(person+0x3du),0xffu);
                node=cm_fn_0068d1b0(D(offer+4u));
            }
        }
    } else {
        if(D(offer+4u)==0)return 1;
        index=D(offer+0xcu);base=D(0x00ae23bcu);
        role=B(base+index*110u+0x3du);person=base+index*110u;
        if(cm_fn_0053b620(role))players=0xffu;
        if(cm_fn_0053b610(B(person+0x3du)))set_category(work,B(person+0x3du),0xffu);
        header=D(offer+4u);
        if(header) {
            index=D(header+2u);D(header+0xau)=index;
            node=cm_fn_0068d1b0(D(offer+4u));
            while(node) {
                if(cm_fn_0053b620(B(node+6u)))players=(u8)(players+1u);
                if(cm_fn_0053b610(B(node+6u)))add_category(work,B(node+6u),1);
                node=cm_fn_0068d1b0(D(offer+4u));
            }
        }
    }
    if((s8)players>0) {
        slots=club+0xd7u;limit=D(0x00ae2364u);pointer=slots;remaining=50;
        do {found=D(pointer);if(found && (s32)D(found)<(s32)limit)players=(u8)(players+1u);pointer+=4u;}while(--remaining);
        if(mapped) {
            pointer=mapped+0xd7u;remaining=50;
            do {found=D(pointer);if(found && (s32)D(found)<(s32)limit)players=(u8)(players+1u);pointer+=4u;}while(--remaining);
        }
        if((s8)players>50) {
            if(allow) {
                players=(u8)(players-cm_fn_008c1780(self,club));
                limit=D(0x00ae2364u);
            }
            if((s8)players>50) {
                index=D(club);base=D(offer+8u);
                if(index!=base)goto player_full;
                if(cm_fn_00600a50(club,1,1,0,0)!=0)goto player_full;
                if(D(offer+4u)!=0)goto player_full;
                pointer=slots;remaining=50;
                do {
                    person=D(pointer);
                    if(!person)goto next_player;
                    index=D(person);limit=D(0x00ae2364u);
                    if((s32)index>=(s32)limit)goto next_player;
                    if(cm_fn_004cf6d0(D(0x00ae1050u),person))goto next_player;
                    found=cm_fn_004cf5a0(D(0x00ae1050u),person);
                    if(!found)goto next_player;
                    status=B(found+0x3du);
                    extended=((long double)(s8)status+CM_GLOBAL(volatile double,0x009688a8u))*(long double)(s32)D(person+0x52u);
                    stored=(double)extended;
                    if(!(extended<(long double)best))goto next_player;
                    if((s8)status>0)goto next_player;
                    found=cm_fn_004cf5a0(D(0x00ae1050u),person);D(0x00ae1080u)=found;
                    if(found && (B(found+0x35u)&0x3fu)==5)goto next_player;
                    if(cm_fn_004cf6d0(D(0x00ae1050u),person))goto next_player;
                    if(cm_fn_008c5cf0(D(0x00de1f54u),person,0,0))goto next_player;
                    if(cm_fn_008c5e40(D(0x00de1f54u),person,0))goto next_player;
                    if(cm_fn_008cca10(D(0x00de1f54u),person,0))goto next_player;
                    manager=D(0x00de1f54u);index=D(manager+0x92eu);header=manager+0x92cu;D(header+0xau)=index;
                    node=cm_fn_0068d1b0(header);
                    while(node) {
                        index=D(person);base=D(node);if(index==base)goto next_player;
                        node=cm_fn_0068d1b0(header);
                    }
                    role=B(person+0x3du);
                    if(role==11 || role==15 || role==13){selected=person;best=stored;}
next_player:
                    pointer+=4u;
                }while(--remaining);
                if(selected) {
                    cm_fn_0086d800(D(0x00dd7f00u),selected);
                    cm_fn_008c1af0(self,selected,club,0,0xffffffffu,0);
                    limit=D(0x00ae2364u);
                } else goto player_full;
            }
        }
    } else limit=D(0x00ae2364u);
    category=B(work+2u);
    if((s8)category>0) {
        pointer=club+0x19fu;remaining=5;
        do {if(D(pointer))category=(u8)(category+1u);pointer+=4u;}while(--remaining);
        B(work+2u)=category;
        if(mapped) {
            pointer=mapped+0x19fu;remaining=5;
            do {found=D(pointer);if(found && (s32)D(found)<(s32)limit)B(work+2u)=(u8)(B(work+2u)+1u);pointer+=4u;}while(--remaining);
        }
        if((s8)B(work+2u)>5)goto staff_full;
    }
    category=B(work);
    if((s8)category>0) {
        pointer=club+0x1cfu;remaining=3;
        do {if(D(pointer))category=(u8)(category+1u);pointer+=4u;}while(--remaining);
        B(work)=category;
        if(mapped) {
            pointer=mapped+0x1cfu;remaining=3;
            do {found=D(pointer);if(found && (s32)D(found)<(s32)limit)B(work)=(u8)(B(work)+1u);pointer+=4u;}while(--remaining);
        }
        if((s8)B(work)>3)goto staff_full;
    }
    category=B(work+1u);
    if((s8)category>0) {
        pointer=club+0x1b3u;remaining=7;
        do {if(D(pointer))category=(u8)(category+1u);pointer+=4u;}while(--remaining);
        B(work+1u)=category;
        if(mapped) {
            pointer=mapped+0x1b3u;remaining=7;
            do {found=D(pointer);if(found && (s32)D(found)<(s32)limit)B(work+1u)=(u8)(B(work+1u)+1u);pointer+=4u;}while(--remaining);
        }
        if((s8)B(work+1u)>7)goto staff_full;
    }
    if((s8)B(work+3u)>0) {
        if(D(club+0xd3u))goto staff_full;
        if(mapped && D(mapped+0xd3u))goto staff_full;
    }
    return 1;
player_full:
    if(output){index=D(club);base=D(offer+8u);W(output)=(u16)((index!=base)+4u);}
    return 0;
staff_full:
    if(output){index=D(club);base=D(offer+8u);W(output)=(u16)((index!=base)+0x22u);}
    return 0;
}
