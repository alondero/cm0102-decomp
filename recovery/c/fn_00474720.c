/* Preferred-image VA 00474720, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 kind, u32 option);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern u32 CM_THISCALL cm_fn_00786b70(u32 self, u32 person, u32 type, void *destination);
extern u32 CM_THISCALL cm_fn_005c89a0(u32 self, u32 kind, u32 option);
extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 first, u32 second, u32 third, u32 fourth, u32 fifth);
extern u32 CM_THISCALL cm_fn_007ff270(u32 self, u32 index, u32 value, u32 flags);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_CDECL cm_fn_00474720(volatile u32 first_arg, volatile u32 second_arg)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[0x2e4];
    u32 previous, first, second, receiver, result, selected, kind, record, club, version;
    previous = CM_FS(volatile u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x00955dbbu;
    frame.prev = previous;
    CM_FS(volatile u32, 0) = (u32)&frame;
    cm_fn_0077bbe0(work + 4, 0, 0);
    second = second_arg;
    first = first_arg;
    receiver = D(0x00b7431cu);
    frame.state = 0;
    result = cm_fn_00786b70(receiver, first, second, work + 4);
    if (result == 0) {
        cm_fn_00944cff(0x0098e598u, 0, 0, work + 0x1e4, work + 0xe4);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x1e4, work + 0xe4, 0x37cfu);
        result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        goto done;
    }
    selected = (u32)cm_fn_00785830(work + 4, 1);
    kind = (u32)cm_fn_00785830(work + 4, 2);
    receiver = D(0x00ae2c74u);
    B((u32)work) = (u8)kind;
    kind = D((u32)work);
    record = cm_fn_005c89a0(receiver, kind, 0);
    if ((s32)selected < 0) club = 0;
    else club = D(0x00ae23b4u) + selected * 581u;
    result = cm_fn_007fe600(0x00b74450u, 0x004748a0u, 0x00474ac0u, 1, 0, 0);
    if (result == 0) goto done;
    cm_fn_007ff270(0x00b74450u, 0, first, 0);
    cm_fn_007ff270(0x00b74450u, 1, second, 0);
    cm_fn_007ff270(0x00b74450u, 2, record, 0);
    result = cm_fn_007ff270(0x00b74450u, 3, club, 0);
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return result;
}
