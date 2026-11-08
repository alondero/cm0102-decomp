/* CM0102 3.9.68, preferred-image VA004cbd20, image base00400000.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Native RET12, ECX self. Original callees remain explicit boundaries. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern __attribute__((regparm(3))) u32 CM_STDCALL cm_fn_004cbb60(u32 incoming_eax,u32 unused_edx,u32 self,volatile u32 record,u32 club);
extern u32 CM_CDECL cm_fn_0053b9a0(u32 club,s8 role,u32 person);
extern u32 CM_CDECL cm_fn_0053bd10(u32 group,u32 person,s8 role,u32 date,u32 update_person,u32 force_role);
extern u32 CM_CDECL cm_fn_0053c190(u32 club,u32 person,u32 date,u32 detach,u32 players_only);
extern u32 CM_CDECL cm_fn_00540980(u32 address,u32 flags);
extern u32 CM_CDECL cm_fn_00540a50(u32 club,u32 direction,u32 require_flag);
extern u32 CM_THISCALL cm_fn_007c5fe0(u32 self,u32 person,u32 field,s8 flag3,s8 flag4,s32 filter,u32 flag6);
extern u32 CM_THISCALL cm_fn_00896c60(u32 self,u32 club,volatile u32 person_arg,volatile u32 last_arg,volatile u32 option);

u32 CM_THISCALL cm_fn_004cbd20(u32 self,u32 person,u32 club,u32 output)
{
    u32 value,index,record,base,limit,source,destination,id;
    u8 mode,flag,role;
    if (club == 0u) return 1u;
    if (output != 0u) D(output)=0u;
    /* 004cbd37 loads the output pointer into EAX. Native 004cbb60
       reads EAX/ECX and its two native stack arguments; EDX is unused. */
    mode=(u8)cm_fn_004cbb60(output,0u,self,person,club);
    if (mode == 0u) return 1u;
    if (mode == 1u || mode == 2u) {
        if (person != 0u) {
            value=D(person);
            limit=D(0x00ae2364u);
            if ((s32)value < (s32)limit) {
                base=D(0x00ae2d0cu);
                index=value*79u;
                value=D(index+base+4u);
                record=index+base;
                if ((s32)value >= 0) {
                    base=D(0x00ae1050u);
                    value=value*80u;
                    base=D(base);
                    value+=base;
                    if (value != 0u) {
                        value=D(value+4u);
                        id=D(club);
                        if (value != id) {
                            value=D(record);
                            if ((s32)value < 0) value=0u;
                            else {base=D(self);value=value*80u+base;}
                            flag=B(value+0x35u);
                            if (mode == 1u) flag &= 0xbfu;
                            else flag |= 0x40u;
                            B(value+0x35u)=flag;
                            return 1u;
                        }
                    }
                }
            }
        }
        cm_fn_00540980(person,mode);
        base=D(0x00de1d08u);
        cm_fn_00896c60(base,club,person,mode == 1u,1u);
        return 1u;
    }
    if (mode == 3u) {
        source=cm_fn_00540a50(club,0u,1u);
        destination=club;
    } else {
        source=club;
        destination=cm_fn_00540a50(club,0u,1u);
    }
    if (output != 0u) D(output)=destination;
    role=B(person+0x3du);
    value=cm_fn_0053b9a0(destination,(s8)role,person);
    if (value == 0u) return value;
    if (person != 0u) {
        id=D(person);
        limit=D(0x00ae2364u);
        if ((s32)id < (s32)limit) {
            base=D(0x00ae2d0cu);
            index=id*79u;
            value=D(index+base+4u);
            if ((s32)value >= 0) {
                index=value*80u;
                base=D(0x00ae1050u);
                value=D(base);
                value+=index;
                if (value != 0u) {
                    value=D(value+4u);
                    base=D(club);
                    if (value == base) {
                        base=D(0x00b74350u);
                        cm_fn_007c5fe0(base,person,0xffffffffu,0,0,-1,0u);
                        cm_fn_0053c190(source,person,0u,1u,1u);
                        base=D(0x00de1d08u);
                        cm_fn_00896c60(base,source,person,1u,1u);
                        base=D(0x00de1d08u);
                        cm_fn_00896c60(base,source,person,0u,1u);
                        role=B(person+0x3du);
                        cm_fn_0053bd10(destination,person,(s8)role,0u,1u,1u);
                        return 1u;
                    }
                }
            }
            /* The second bound/table lookup is a native reload. */
            limit=D(0x00ae2364u);
            if ((s32)id < (s32)limit) {
                base=D(0x00ae2d0cu);
                index=id*79u;
                value=D(index+base+4u);
                record=index+base;
                if ((s32)value >= 0) {
                    base=D(0x00ae1050u);
                    value=value*80u;
                    value+=D(base);
                    /* Native 004cbfe2 assigns its argument slot, not *output. */
                    if (value != 0u) {
                        value=D(record);
                        if ((s32)value < 0) value=0u;
                        else value=value*80u+D(self);
                        flag=B(value+0x35u);
                        if ((flag&0x40u) == 0u) flag |= 0x40u;
                        else flag &= 0xbfu;
                        B(value+0x35u)=flag;
                        cm_fn_0053c190(source,person,0u,0u,1u);
                        cm_fn_0053bd10(destination,person,11,0u,0u,1u);
                        return 1u;
                    }
                }
            }
        }
    }
    base=D(0x00b74350u);
    cm_fn_007c5fe0(base,person,0xffffffffu,0,0,-1,0u);
    cm_fn_0053c190(source,person,0u,1u,0u);
    base=D(0x00de1d08u);
    cm_fn_00896c60(base,source,person,1u,1u);
    base=D(0x00de1d08u);
    cm_fn_00896c60(base,source,person,0u,1u);
    role=B(person+0x3du);
    cm_fn_0053bd10(destination,person,(s8)role,0u,1u,0u);
    return 1u;
}
