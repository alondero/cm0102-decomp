/* Preferred-image VA 008d0a70, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_THISCALL cm_fn_0054a110(void *date);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 person);
extern u32 CM_CDECL cm_fn_00544670(u32 nation);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_STDCALL cm_fn_008d0a70(u32 person)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[0x208];
    u32 receiver, pointer, index, base, club, nation, result, version;
    frame.state = 0xffffffffu;
    frame.handler = 0x0096405bu;
    frame.prev = CM_FS(volatile u32, 0);
    CM_FS(volatile u32, 0) = (u32)&frame;
    cm_fn_0054a110(work);
    frame.state = 0;
    if (person == 0) {
        cm_fn_00944cff(0x00a8d514u, 0, 0, work + 0x108, work + 8);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x108, work + 8, 0x3798u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        result = 0;
        goto done;
    }
    receiver = D(0x00ae1050u);
    pointer = cm_fn_004cf5a0(receiver, person);
    D(0x00ae1080u) = pointer;
    if (pointer != 0 && (B(pointer + 0x35u) & 0x3fu) != 5u) {
        index = D(pointer + 4u);
        base = D(0x00ae23b4u);
        club = base + index * 0x245u;
        if (club != 0) {
            nation = D(club + 0x53u);
            if (nation != 0) {
                result = cm_fn_00544670(nation);
                goto done;
            }
        }
    }
    result = 1;
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return result;
}
