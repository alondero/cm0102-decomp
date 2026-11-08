/* Preferred-image VA 0x006adb70, image base 0x00400000.
   CM 01/02 3.9.68 SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Three stack arguments, RET 12; indexed caller 00781ae7 overwrites EAX.
   Numeric globals and original callees remain explicit boundaries. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define SB(a) CM_GLOBAL(volatile s8, (a))
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern u32 CM_CDECL cm_fn_0053dba0(u32,u32,u32,u32);
/* 0066f4e0 consumes the variadic argument cursor; callers clean 8..48 bytes. */
extern u32 CM_CDECL cm_fn_0066f4e0(u32 destination,u32 format,...);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self,u32 text);
extern u32 CM_CDECL cm_fn_0053e990(u32,u32,u32,u32,u32);
/* ECX receiver, three stack arguments; 004aeac5 RET 12. */
extern u32 CM_THISCALL cm_fn_004ae8a0(void *self,u32 text,u32 tag,u32 value);

void CM_STDCALL cm_fn_006adb70(void *message,u32 detailed,void *output)
{
    u8 name[500];
    u32 person_index,a1,a2,a3,club_index,kind,mode,level,a9,a10;
    u32 person,club,format,n,related,rn;
    person_index=(u32)cm_fn_00785830(message,0);
    a1=(u32)cm_fn_00785830(message,1);
    a2=(u32)cm_fn_00785830(message,2);
    a3=(u32)cm_fn_00785830(message,3);
    club_index=(u32)cm_fn_00785830(message,4);
    kind=(u32)cm_fn_00785830(message,5);
    mode=(u32)cm_fn_00785830(message,6);
    level=(u32)cm_fn_00785830(message,7);
    cm_fn_00785830(message,8);
    a9=(u32)cm_fn_00785830(message,9);
    a10=(u32)cm_fn_00785830(message,10);
    person=D(0x00ae23bcu)+person_index*110u;
    club=D(0x00ae23b4u)+club_index*581u;
    cm_fn_0053dba0(person,(u32)name,500,1);
    if (!detailed) {
        if (mode==1) {
            n=(u32)(s32)SB(club+0x52u);
            cm_fn_0066f4e0(0x00de1f64u,0x00a12ef8u,n,n,club+0x38u);
        } else {
            if (level==2 || level==3 || level==4) {
                format=kind==1?0x00a12f38u:kind==2?0x00a12f5cu:0x00a12f8cu;
            } else {
                format=kind==1?0x00a12fc4u:kind==2?0x00a12fe8u:0x00a13018u;
            }
            cm_fn_0066f4e0(0x00de1f64u,format);
        }
        cm_fn_004ae660(output,0x00de1f64u);
        return;
    }
    if (mode==1) {
        if (level==2) {
            format=kind==1?0x00a13ad0u:kind==2?0x00a13c54u:kind==3?0x00a13e44u:0x00a13ffcu;
            n=(u32)(s32)SB(club+0x37u);
        } else if (kind==1) {
            if (!a9) {
                related=D(club+0x69u);
                n=(u32)(s32)SB(club+0x37u);
                rn=(u32)(s32)SB(related+0x37u);
                cm_fn_0066f4e0(0x00de1f64u,0x00a13050u,n,n,100u,100u,rn,rn,club+4u,(u32)name,(u32)name,related+4u);
                goto assign;
            }
            n=(u32)(s32)SB(club+0x37u);
            format=0x00a131f8u;
        } else if (kind==2) {
            n=(u32)(s32)SB(club+0x37u);
            format=a10?0x00a13398u:0x00a13520u;
        } else {
            n=(u32)(s32)SB(club+0x37u);
            format=kind==3?0x00a13738u:0x00a13924u;
        }
        cm_fn_0066f4e0(0x00de1f64u,format,n,n,100u,100u,club+4u,(u32)name,(u32)name);
    } else {
        if (level==4) format=0x00a14e5cu;
        else if (level==3) format=kind==1?0x00a1499cu:kind==2?0x00a14af0u:kind==3?0x00a14c18u:0x00a14d30u;
        else if (level==2) format=kind==1?0x00a1450cu:kind==2?0x00a14664u:kind==3?0x00a14764u:0x00a14848u;
        else if (level==1) format=kind==1?0x00a141c4u:kind==2?0x00a14280u:kind==3?0x00a14354u:0x00a14438u;
        else goto assign;
        n=(u32)(s32)SB(club+0x37u);
        cm_fn_0066f4e0(0x00de1f64u,format,n,n,club+4u);
    }
assign:
    cm_fn_004ae660(output,0x00de1f64u);
    if (cm_fn_0053e990(person,a1,a2,a3,0))
        cm_fn_004ae8a0(output,(u32)name,0x7dau,person);
    cm_fn_004ae8a0(output,club+4u,0x7d5u,club);
}
