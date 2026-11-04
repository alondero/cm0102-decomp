#include "cmre.h"
/* Preferred-image VA 007aa4c0, base00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ret12: three stack arguments, no incoming receiver. Preserve full native EAX.
   All seven callees remain original boundaries. No FTOL sites in this body. */
extern unsigned long long CM_THISCALL cm_fn_00785830(void *,u32);
extern u32 CM_CDECL cm_fn_0053e140(u32,u32,u32,u32,u32,u32,u32);
extern char *CM_CDECL cm_fn_0066f4e0(u32,u32,...);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *,u32);
extern u32 CM_CDECL cm_fn_0053e990(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32,u8 *,u32,u32);
extern char *CM_CDECL cm_fn_0053ad00(u8,s32,s32,u8 *);
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))
#define SB(a) (*(volatile s8 *)(u32)(a))
#define SW(a) (*(volatile s16 *)(u32)(a))
#define TEXT 0x00de1f64u

u32 CM_STDCALL cm_fn_007aa4c0(void *record,u32 mode,void *destination)
{
    /* L is the original esp after sub1a4. Character L+3 and name L+14(hex)
       remain adjacent within the same uninitialized scratch allocation. */
    u8 scratch[0x1a4];
    u8 *name=scratch+0x14;
    u32 person,first,second,third,related,related_index,comparison;
    u32 p,offset,base,value,format,result,nation;
    u8 selector,role;
    s32 gender,club_gender;
    char *job;
    if (mode==0) {
        person=(u32)cm_fn_00785830(record,0);
        first=(u32)cm_fn_00785830(record,1);
        second=(u32)cm_fn_00785830(record,2);
        third=(u32)cm_fn_00785830(record,3);
        cm_fn_0053e140(first,second,third,(u32)name,0x190,5,person);
        cm_fn_0066f4e0(TEXT,0x00a70110,(u32)name);
        cm_fn_004ae660(destination,TEXT);
        base=D(0x00ae23bc);offset=person*110u;
        result=cm_fn_0053e990(base+offset,first,second,third,0);
        if (!result) return result;
        base=D(0x00ae23bc);
        return cm_fn_004ae8a0((u32)destination,name,0x7d7,offset+base);
    }
    person=(u32)cm_fn_00785830(record,0);
    first=(u32)cm_fn_00785830(record,1);
    second=(u32)cm_fn_00785830(record,2);
    third=(u32)cm_fn_00785830(record,3);
    selector=(u8)cm_fn_00785830(record,4);
    related_index=(u32)cm_fn_00785830(record,5);
    comparison=(u32)cm_fn_00785830(record,6);
    related=0;
    if (related_index!=0xffffffffu) {
        base=D(0x00ae23b4);
        related=base+related_index*581u;
    }
    cm_fn_0053e140(first,second,third,(u32)name,0x190,1,person);
    if (related==0) goto outside;
    value=D(related+0xcf);
    if (value==0) goto outside;
    offset=(u32)(s32)SW(0x00b77c7a)*768u;
    if (value!=D(0x00b74c26u+offset)) goto outside;
    /* 007aa666..007aa8ae: related association matches the selected table. */
    if (selector==16) {
        base=D(0x00ae23bc);p=base+person*110u;
        value=D(p+0x69);
        if (value) {
            role=B(p+0x3d);
            if (!role || role==selector) format=0x00a6fc1c;
            else if ((s8)role==12) format=0x00a6fd20;
            else if ((s8)role==13) format=0x00a6fe1c;
            else if ((s8)role==15) format=0x00a6ff24;
            else format=0x00a70020;
        } else {
            format=SB(p+0x18)<27 ? 0x00a6fb74 : 0x00a6fac8;
        }
        goto simple;
    }
    if (selector==1) {
        base=D(0x00ae23bc);p=base+person*110u;
        value=D(p+0x69);
        if (!value) {format=0x00a6f57c;goto simple;}
        role=B(p+0x3d);
        if (!role || role==16 || role==11) goto generic_retire;
        if ((s8)role==12) format=0x00a6f628;
        else if ((s8)role==13) format=0x00a6f730;
        else if ((s8)role==15) format=0x00a6f840;
        else format=(s32)comparison>27 ? 0x00a6fa04 : 0x00a6f944;
        goto simple;
    }
    if (selector==32) {
        offset=person*110u;base=D(0x00ae23bc);
        role=B(offset+base+0x3d);p=offset+base+0x3d;
        if (role && role!=16) {
            value=D(offset+base+0x39);
            if (value) {
                role=B(p);
                job=cm_fn_0053ad00(role,0,0,scratch+3);
                gender=(s8)scratch[3];
                cm_fn_0066f4e0(TEXT,0x00a6f4b0,gender,gender,(u32)name,(u32)job);
                goto assign;
            }
        }
        goto nation_branch;
    }
    if (selector==2) {format=0x00a6e24c;goto simple;}
    goto assign;
outside:
    /* 007aa8b3..007aac60: related association absent or different. */
    if (selector==16) {
        base=D(0x00ae23bc);p=base+person*110u;
        value=D(p+0x69);
        if (!value) {
            role=B(p+0x18);
            club_gender=SB(related+0x52);
            format=(s8)role<27 ? 0x00a6ec2c : 0x00a6eb44;
            goto club_format;
        }
        role=B(p+0x3d);
        if (!role || role==selector) {format=0x00a6ed10;goto club_load;}
        if ((s8)role==12) {format=0x00a6ee4c;goto club_load;}
        if ((s8)role==13) {format=0x00a6ef84;goto club_load;}
        club_gender=SB(related+0x52);
        format=(s8)role==15 ? 0x00a6f0c8 : 0x00a6f200;
        goto club_format;
    }
    if (selector==1) {
        base=D(0x00ae23bc);p=base+person*110u;
        value=D(p+0x69);
        if (!value) {
            if (!related) {format=0x00a6f57c;goto simple;}
            format=0x00a6e4fc;goto club_load;
        }
        role=B(p+0x3d);
        if (!role || role==16 || role==11 || !related) goto generic_retire;
        if ((s8)role==12) {format=0x00a6e6cc;goto club_load;}
        if ((s8)role==13) {format=0x00a6e7f8;goto club_load;}
        club_gender=SB(related+0x52);
        format=(s8)role==15 ? 0x00a6e930 : 0x00a6ea5c;
        goto club_format;
    }
    if (selector==32) {
        base=D(0x00ae23bc);
        if (related) {
            p=base+person*110u;
            role=B(p+0x3d);
            if (role && role!=16) {
                value=D(p+0x39);
                if (value) {
                    role=B(p+0x3d);
                    job=cm_fn_0053ad00(role,0,0,scratch+3);
                    gender=(s8)scratch[3];
                    club_gender=SB(related+0x52);related+=0x38;
                    cm_fn_0066f4e0(TEXT,0x00a6e408,club_gender,club_gender,gender,gender,related,(u32)name,(u32)job);
                    goto assign;
                }
            }
        }
        offset=person*110u;
        role=B(offset+base+0x28);p=offset+base+0x28;
        if (role && role!=16) {
            value=D(offset+base+0x24);
            if (value) goto nation_format;
        }
        format=0x00a6e35c;goto simple;
    }
    if (selector!=2) goto assign;
    if (!related) {format=0x00a6e24c;goto simple;}
    format=0x00a6e2c0;
club_load:
    club_gender=SB(related+0x52);
club_format:
    related+=0x38;
    cm_fn_0066f4e0(TEXT,format,club_gender,club_gender,related,(u32)name);
    goto assign;
generic_retire:
    format=0x00a6e5cc;goto simple;
nation_branch:
    role=B(offset+base+0x28);p=offset+base+0x28;
    if (role && role!=16) {
        value=D(offset+base+0x24);
        if (value) goto nation_format;
    }
    format=0x00a6f338;goto simple;
nation_format:
    role=B(p);
    job=cm_fn_0053ad00(role,0,0,scratch+3);
    base=D(0x00ae23bc);
    nation=D(offset+base+0x24);
    gender=(s8)scratch[3];club_gender=SB(nation+0x37);nation+=4;
    cm_fn_0066f4e0(TEXT,0x00a6f3cc,club_gender,club_gender,gender,gender,(u32)name,nation,(u32)job);
    goto assign;
simple:
    cm_fn_0066f4e0(TEXT,format,(u32)name);
assign:
    cm_fn_004ae660(destination,TEXT);
    offset=person*110u;base=D(0x00ae23bc);
    result=cm_fn_0053e990(offset+base,first,second,third,0);
    if (!result) return result;
    base=D(0x00ae23bc);
    return cm_fn_004ae8a0((u32)destination,name,0x7d7,offset+base);
}
