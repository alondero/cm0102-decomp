#include "cmre.h"

/* Preferred-image VA 0x005fe700, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   L is ESP after SUB ESP,20Ch. Only the counter's low byte is initialized.
   VA 005ae6ad replaces CX before forwarding ECX: preserve its incoming high
   half explicitly. VA 005abfc7 consumes only the counter's low byte. */
extern u32 CM_CDECL cm_fn_00600a50(u32, u32, u8, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00549ef0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_005ae6a0(u32, u32, u8, u32);
extern u32 CM_FASTCALL cm_fn_0068d1b0(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_STDCALL cm_fn_005fe700(u32 packet, u32 person)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x20c];
    u32 work = (u32)scratch, eax, index, club, flag, day, year, receiver, count;
    u32 list, item;
    u8 next, limit;
    char *version;

    eax = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095b02b;
    frame.prev = eax;
    CM_FS(u32, 0) = (u32)&frame;
    if (!packet) {
        cm_fn_00944cff(0x009cf11c, 0, 0, scratch + 0x10c, scratch + 0xc);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version,
                      scratch + 0x10c, scratch + 0xc, 0x707);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        goto no;
    }
    if (!person) {
        index = (u32)(s32)(s16)W(0x00b77c7a);
        person = D(0x00b74c26 + index * 768u);
        if (!person) goto no;
    }
    if ((s8)B(packet + 0x43) > -1) goto no;
    flag = B(packet + 0x40);
    club = D(packet + 0x1c);
    if (cm_fn_00600a50(club, 0, (u8)flag, person, 0)) goto no;
    flag = B(packet + 0x41);
    club = D(packet + 0x20);
    if (cm_fn_00600a50(club, 0, (u8)flag, person, 0)) goto no;
    B(work) = 0;
    if ((s8)B(0x00b64738) <= 0) goto yes;
    do {
        day = W(packet + 0x28);
        year = W(packet + 0x2a);
        eax = (u32)cm_fn_00549ef0(work + 4, year, day);
        count = D(work);
        flag = B(packet + 0x3f);
        receiver = D(0x00ae2a58);
        frame.state = 0;
        list = cm_fn_005ae6a0(receiver, eax, (u8)flag, count);
        frame.state = 0xffffffffu;
        if (list) {
            eax = D(list + 2);
            D(list + 10) = eax;
            item = cm_fn_0068d1b0(list);
            while (item) {
                flag = B(item + 0x40);
                club = D(item + 0x1c);
                if (cm_fn_00600a50(club, 0, (u8)flag, person, 0)) goto no;
                flag = B(item + 0x41);
                club = D(item + 0x20);
                if (cm_fn_00600a50(club, 0, (u8)flag, person, 0)) goto no;
                item = cm_fn_0068d1b0(list);
            }
        }
        next = B(work);
        limit = B(0x00b64738);
        ++next;
        B(work) = next;
    } while ((s8)next < (s8)limit);
yes:
    eax = 1;
    goto done;
no:
    eax = 0;
done:
    CM_FS(u32, 0) = frame.prev;
    return eax;
}
