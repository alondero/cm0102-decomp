#include "cmre.h"

/* Preferred-image VA 00690890, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Preserve both physical dispatch-table reads and their distinct role paths.
   The primary default tests the low BYTE of the original first argument
   slot, not a role flag. The old-ID exit returns the full person ID. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_THISCALL cm_fn_004d0b50(u32,u32);
extern u32 CM_THISCALL cm_fn_00843e10(u32,u32,u32,u8,u8);
extern u32 CM_THISCALL cm_fn_007f7270(u32,u32);
extern u32 CM_THISCALL cm_fn_008b2470(u32,u32,u32,u32);

#define CLEAR_SLOTS(start,total) do { \
    pointer=club+(start); \
    for (i=0;i<(total);++i) { \
        if (D(pointer)==person) D(pointer)=0; \
        pointer+=4; \
    } \
} while (0)

u32 CM_STDCALL cm_fn_00690890(volatile u32 person_arg,volatile u32 club_arg)
{
    u32 owner,person,club,role,target,pointer,i,value,current;
    owner=D(0x00ae1050);
    person=person_arg;
    cm_fn_004d0b50(owner,person);
    club=club_arg;
    value=D(person+0x39);
    if (value==club) {
        role=(u32)(s32)(s8)B(person+0x3d)-1;
        if (role<=14) {
            target=D(0x00690af8+role*4);
            (void)target;
            switch (role+1) {
            case 1:
                D(club+0xbf)=0;
                D(person+0x39)=0;
                break;
            case 2:
                CLEAR_SLOTS(0xc3,3);
                D(person+0x39)=0;
                break;
            case 5:
                if (D(club+0xcf)==person) D(club+0xcf)=0;
                D(person+0x39)=0;
                break;
            case 6:
                D(club+0xd3)=0;
                D(person+0x39)=0;
                break;
            case 8:
                CLEAR_SLOTS(0x19f,5);
                D(person+0x39)=0;
                break;
            case 9:
                CLEAR_SLOTS(0x1b3,7);
                D(person+0x39)=0;
                break;
            case 10:
                CLEAR_SLOTS(0x1cf,3);
                D(person+0x39)=0;
                break;
            case 12:
                B(person+0x3d)=11;
                if (D(club+0xcf)==person) D(club+0xcf)=0;
                break;
            case 13:
                B(person+0x3d)=11;
                D(club+0xd3)=0;
                break;
            case 15:
                B(person+0x3d)=11;
                CLEAR_SLOTS(0x19f,5);
                break;
            default:
                goto primary_default;
            }
        } else {
primary_default:
            if (*(volatile u8 *)&person_arg==0) D(person+0x39)=0;
        }
    } else if (D(person+0x24)==club) {
        role=(u32)(s32)(s8)B(person+0x28)-1;
        if (role<=14) {
            target=D(0x00690b34+role*4);
            (void)target;
            switch (role+1) {
            case 1: D(club+0xbf)=0; break;
            case 2: CLEAR_SLOTS(0xc3,3); break;
            case 5: D(club+0xcf)=0; break;
            case 6: D(club+0xd3)=0; break;
            case 8: CLEAR_SLOTS(0x19f,5); break;
            case 9: CLEAR_SLOTS(0x1b3,7); break;
            case 10: CLEAR_SLOTS(0x1cf,3); break;
            case 12:
                D(club+0xcf)=0;
                B(person+0x28)=11;
                goto notify;
            case 13:
                B(person+0x28)=11;
                D(club+0xd3)=0;
                goto notify;
            case 15:
                B(person+0x28)=11;
                CLEAR_SLOTS(0x19f,5);
                goto notify;
            }
        }
        D(person+0x24)=0;
    }
notify:
    owner=D(0x00de1f54);
    owner=D(owner+0x856);
    cm_fn_00843e10(owner,person,club,4,2);
    current=D(0x00ae2364);
    value=D(person);
    current-=16;
    if ((s32)value<(s32)current) return value;
    owner=D(0x00b74448);
    cm_fn_007f7270(owner,person);
    owner=D(0x00de1f44);
    return cm_fn_008b2470(owner,person,club,0);
}
