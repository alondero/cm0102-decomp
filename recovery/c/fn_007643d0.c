/* Preferred-image VA007643D0, base00400000; target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL1/RET0, full EAX results. Cached WORD selectors and every original
   table reload are distinct. Scratch L+8 is the original 4F-byte output.
   Original engine/UI calls remain boundaries, including held callees. */
#include "cmre.h"
#define B(p) CM_GLOBAL(volatile u8,(u32)(p))
#define W(p) CM_GLOBAL(volatile u16,(u32)(p))
#define D(p) CM_GLOBAL(volatile u32,(u32)(p))
#define SW(v) ((u32)(s32)(s16)(v))
extern void CM_CDECL cm_fn_00414c20(u32);
extern void CM_CDECL cm_fn_00453d60(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00468b10(u32,u32);
extern u32 CM_THISCALL cm_fn_00468b40(u32,u32);
extern u32 CM_THISCALL cm_fn_00468b90(u32,u32);
extern void CM_CDECL cm_fn_00469e70(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004739a0(u32,u32);
extern void CM_CDECL cm_fn_004739d0(u32,u32);
extern u32 CM_THISCALL cm_fn_0048d200(u32,u32);
extern void CM_CDECL cm_fn_0048f4a0(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_004b0870(u32);
extern u32 CM_CDECL cm_fn_0053b900(u32);
extern u32 CM_THISCALL cm_fn_005c2990(u32,u32);
extern u32 CM_THISCALL cm_fn_005fe290(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_005fe500(u32,u32,u32);
extern void CM_CDECL cm_fn_0071b4a0(u32);
extern u32 CM_THISCALL cm_fn_00729e30(u32,u32,u32);
/* Native full73B consumes one stack argument and RET4; incoming ECX dead. */
extern u32 CM_STDCALL cm_fn_007e6f20(u32);
extern void CM_THISCALL cm_fn_00803bf0(u32);
extern void CM_CDECL cm_fn_00808540(u32);
/* Physical EAX is the same value pushed as the first original stack arg. */
extern u32 CM_CDECL __attribute__((regparm(1)))
cm_fn_008702a0(u32,u32,u32,u32,u32,u32);

static u32 field_007643d0(u16 page,u32 selection,u32 offset)
{
    u32 receiver,index;
    if ((u16)selection==0xffff) return 0;
    receiver=D(0xb74c4c+SW(page)*768u);
    index=SW(selection)*396u;
    return D(receiver+index+offset);
}

static u16 route_007643d0(u16 page,u32 selection,u16 fallback)
{
    u32 value=field_007643d0(page,selection,0xba966);
    if ((u16)value==0) value=(value&0xffff0000u)|fallback;
    return (u16)value;
}

u32 CM_CDECL cm_fn_007643d0(u32 selection)
{
    volatile u8 scratch[0x58];
    u16 page,fallback;
    u32 a,b,receiver,club,slot,subslot,offset,record,entry,i,limit,packet;
    u32 other,mode;
    u8 count;
    page=W(0xb77c7a);
    a=field_007643d0(page,selection,0xba966);
    fallback=W(0xdd6bde);
    if ((u16)a==0) a=(a&0xffff0000u)|fallback;
    if ((u16)a==0x426) {
        cm_fn_00803bf0(0xb74450);
        fallback=W(0xdd6bde); page=W(0xb77c7a);
    }
    if (route_007643d0(page,selection,fallback)==0x7d0) {
        record=field_007643d0(page,selection,0xba9a6);
        a=D(record); b=D(0xadadfc); a=D(b+a*4);
        if (a!=0) {
            a=cm_fn_007e6f20(record);
            if (a==0) {
                receiver=D(0xae2c74);
                a=cm_fn_005c2990(receiver,record);
                if (a==0) {
                    cm_fn_0048f4a0(record,0xfffffffeu,0xfffffffeu,0);
                    return 0xfffffffcu;
                }
            }
            fallback=W(0xdd6bde); page=W(0xb77c7a);
        }
    }
    if (route_007643d0(page,selection,fallback)==0x7e5) {
        club=field_007643d0(page,selection,0xba9a6)>>22;
        slot=(field_007643d0(page,selection,0xba9a6)>>11)&0x7ff;
        subslot=field_007643d0(page,selection,0xba9a6)&0x7ff;
        if ((s32)club>=0) {
            limit=D(0xae2378);
            if ((s32)club<(s32)limit) {
                a=D(0xadadfc); record=D(a+club*4);
                if (record!=0) {
                    a=D(record+4); a=cm_fn_007e6f20(a);
                    if (a==0) {
                        offset=club*107u;
                        a=D(0xae23d0); a+=offset;
                        receiver=D(0xae2c74);
                        a=cm_fn_005c2990(receiver,a);
                        if (a==0) {
                            a=D(record+0x30); i=0xffffffffu;
                            if ((s32)a>=-1) {
                                do {
                                    entry=cm_fn_0048d200(record,i);
                                    if (W(entry+0x3a)==(u16)slot) goto found_record;
                                    a=D(record+0x30); i++;
                                } while ((s32)i<=(s32)a);
                            }
                            a=D(0xae23d0);
                            cm_fn_0048f4a0(offset+a,0xfffffffeu,0xfffffffeu,0);
                            return 0xfffffffcu;
found_record:
                            if (B(entry+0x43)==1) {
                                count=B(entry+0xab);
                                if ((s8)count>0) {
                                    b=0; packet=0;
                                    do {
                                        a=cm_fn_004b0870(subslot);
                                        receiver=D(entry+0xa3);
                                        if (W(receiver+packet+7)==(u16)a) {
                                            a=D(0xae23d0);
                                            cm_fn_0048f4a0(a+offset,i,b,0);
                                            return 0xfffffffcu;
                                        }
                                        count=B(entry+0xab); b++; packet+=0x68;
                                    } while ((s32)b<(s32)(s8)count);
                                }
                            }
                            a=D(0xae23d0);
                            cm_fn_0048f4a0(a+offset,i,0xfffffffeu,0);
                            return 0xfffffffcu;
                        }
                    }
                    fallback=W(0xdd6bde); page=W(0xb77c7a);
                }
            }
        }
    }
    if (route_007643d0(page,selection,fallback)==0x7e6) {
        a=field_007643d0(page,selection,0xba9a6);
        cm_fn_00469e70(a,0,1);
        return 0xfffffffcu;
    }
    if (route_007643d0(page,selection,fallback)==0x7d1) {
        a=field_007643d0(page,selection,0xba9a6);
        cm_fn_004739d0(a,0);
        return 0xfffffffcu;
    }
    if (route_007643d0(page,selection,fallback)==0x7d3) {
        if ((u16)selection!=0xffff) {
            a=field_007643d0(page,selection,0xba9a6);
            if (a==0xffffffffu) return 0;
        } else a=0;
        receiver=D(0xb6b7c0);
        b=cm_fn_00729e30(receiver,a,(u32)&scratch[8]);
        if (b==0) return 0;
        cm_fn_0071b4a0(a);
        return 0xfffffffcu;
    }
    if (route_007643d0(page,selection,fallback)==0x7d5) {
        record=field_007643d0(page,selection,0xba9a6);
        a=cm_fn_0053b900(record);
        if (a!=0) {
            a=D(record+0x53);
            if (a!=0) {
                receiver=D(a+0x71);
                if (receiver==0) goto reload;
            }
        }
        a=cm_fn_0053b900(record);
        if (a==0) {
            a=D(record+0x53);
            if (a==0) goto reload;
        }
        cm_fn_00453d60(record,0,0,0,0);
        return 0xfffffffcu;
reload:
        fallback=W(0xdd6bde); page=W(0xb77c7a);
    }
    if (route_007643d0(page,selection,fallback)==0x7d6) {
        a=field_007643d0(page,selection,0xba9a6);
        cm_fn_00453d60(a,1,0,0,0);
        return 0xfffffffcu;
    }
    if (route_007643d0(page,selection,fallback)==0x7d4) {
        a=field_007643d0(page,selection,0xba9a6);
        cm_fn_00414c20(a);
        return 0xfffffffcu;
    }
    if (route_007643d0(page,selection,fallback)==0x7d7) {
        a=field_007643d0(page,selection,0xba9a6);
        cm_fn_008702a0(a,a,1,1,0,0);
        return 0xfffffffcu;
    }
    if (route_007643d0(page,selection,fallback)==0x7e7) {
        a=field_007643d0(page,selection,0xba9a6);
        cm_fn_008702a0(a,a,1,1,0,2);
        return 0xfffffffcu;
    }
    if (route_007643d0(page,selection,fallback)==0x7d8) {
        a=field_007643d0(page,selection,0xba9a6);
        cm_fn_008702a0(a,a,2,1,0,0);
        return 0xfffffffcu;
    }
    /* Each original probe reloads the page WORD after its call, except the
       final unmatched 7E0 probe. Original 468B10/4739A0 consume WORD index. */
    for (b=0;b<8;b++) {
        receiver=D(0xb74c4c+SW(page)*768u);
        a=cm_fn_00468b10(receiver,selection);
        if ((u16)a==0) a=(a&0xffff0000u)|W(0xdd6bde);
        switch(b) {
        case 0: slot=0x7da; mode=3; other=1; break;
        case 1: slot=0x7d9; mode=2; other=1; break;
        case 2: slot=0x7db; mode=4; other=1; break;
        case 3: slot=0x7dc; mode=1; other=2; break;
        case 4: slot=0x7dd; mode=2; other=2; break;
        case 5: slot=0x7df; mode=3; other=2; break;
        case 6: slot=0x7de; mode=2; other=2; break;
        default: slot=0x7e0; mode=4; other=2; break;
        }
        if (b!=7 || (u16)a==slot) page=W(0xb77c7a);
        if ((u16)a==slot) {
            receiver=D(0xb74c4c+SW(page)*768u);
            a=cm_fn_004739a0(receiver,selection);
            cm_fn_008702a0(a,a,mode,other,0,0);
            return 0xfffffffcu;
        }
    }
    a=cm_fn_00468b40(0xb74450,selection);
    if (a==0x7d2) {
        packet=cm_fn_00468b90(0xb74450,selection);
        receiver=D(0xb63c98);
        a=cm_fn_005fe290(receiver,packet,0);
        receiver=D(0xb63c98);
        a=cm_fn_005fe500(receiver,a!=0?0:packet,0);
        if (a!=0) return 0xfffffff5u;
    }
    a=cm_fn_00468b40(0xb74450,selection);
    if (a==0x7e4) {
        a=cm_fn_00468b90(0xb74450,selection);
        cm_fn_00808540(a);
        return 0xfffffffcu;
    }
    return 0;
}
