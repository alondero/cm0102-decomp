#include "cmre.h"
/* Preferred-image VA006ac560/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/RET4; both indexed callers consume AL. Original L denotes ESP
   after SUB ESP,24: copied key L0, dates L4/Lc, list L14..21.
   Original006ac700 remains a physical EAX/EDX/ECX input and EDX:EAX
   output boundary, distinct from its accepted void C definition. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_THISCALL cm_fn_0068d020(u32);
extern void CM_THISCALL cm_fn_0068d040(u32);
extern u8 CM_CDECL cm_fn_00669360(u32);
extern u32 CM_THISCALL cm_fn_0054c770(u32,u32,u8);
extern u32 CM_THISCALL cm_fn_005acbe0(u32,u32,u32,u16,u16,u32,u32,u32,u8,u8);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern unsigned long long CM_STDCALL __attribute__((regparm(3)))
cm_fn_006ac700(u32,u32,u32,u32,u32,u32);
extern unsigned long long CM_FASTCALL cm_fn_0068d0d0(u32,u32,u32,s16);

u8 CM_THISCALL cm_fn_006ac560(u32 self, volatile u32 club_arg)
{
    volatile struct { u32 prev,handler,state; } frame;
    u8 scratch[0x24];
    u32 l=(u32)scratch, club, p, item, first_day, first_year, owner, other, value;
    u16 last_year,last_day;
    unsigned long long pair;
    frame.state=0xffffffffu;
    frame.handler=0x0095d088u;
    frame.prev=CM_FS(u32,0);
    CM_FS(u32,0)=(u32)&frame;
    cm_fn_0068d020(l+0x14u);
    club=club_arg;
    frame.state=0;
    if (!club || !D(club+0xcfu) || !D(club+0x57u)) goto no_match;
    cm_fn_00669360(club);
    p=cm_fn_0054c770(0x00ae2c90u,l+4u,8);
    last_year=W(p+2u);
    B((u32)&frame.state)=1;
    p=cm_fn_0054c770(0x00ae2c90u,l+0xcu,8);
    value=D(club);
    last_day=W(p);
    first_year=((l+0x14u)&0xffff0000u)|W(0x00ae2c92u);
    owner=D(0x00ae2a58u);
    first_day=(p&0xffff0000u)|W(0x00ae2c90u);
    B((u32)&frame.state)=2;
    cm_fn_005acbe0(owner,first_day,first_year,last_day,last_year,value,
                  l+0x14u,0xffffffffu,0xfe,1);
    value=W(l+0x14u);
    B((u32)&frame.state)=0;
    if (!value) goto no_match;
    D(l+0x1eu)=D(l+0x16u);
    item=cm_fn_0068d1b0(l+0x14u);
    while (item) {
        first_day=(u32)(s32)(s16)W(0x00ae2c90u);
        first_year=W(item+0x2au);
        if (first_year!=first_day || W(item+0x28u)!=W(0x00ae2c92u)) {
            if (B(item+0x40u) && B(item+0x43u)==0xff) break;
        }
        item=cm_fn_0068d1b0(l+0x14u);
    }
    if (!item) goto no_match;
    owner=D(item+0x14u);
    if (!owner) goto no_match;
    other=D(item+0x1cu);
    value=other==club ? D(item+0x20u) : other;
    pair=cm_fn_006ac700(value,other,self,club,value,owner);
    value=D(club);
    owner=D(self+0x10u);
    D(l)=value;
    cm_fn_0068d0d0(owner,(u32)(pair>>32),l,4);
    frame.state=0xffffffffu;
    cm_fn_0068d040(l+0x14u);
    CM_FS(u32,0)=frame.prev;
    return 1;
no_match:
    frame.state=0xffffffffu;
    cm_fn_0068d040(l+0x14u);
    CM_FS(u32,0)=frame.prev;
    return 0;
}
