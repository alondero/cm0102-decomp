/* Preferred-image VA 0x00544F30, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: push esi; mov esi, ecx; then for each of eight dword fields at
   offsets +0x20, +0x24, +0x28, +0x2c, +0x30, +0x34, +0x38, +0x3c, in that order,
   if the field is nonzero, push it and call 009452CA, then add esp, 4. Pop ESI;
   ret with no stack bytes popped. The single register input is ECX, used as the
   object pointer. Field meanings and the callee's behaviour remain unknown;
   009452CA is an explicit boundary.
   The routine has no semantic return; direct callers do not observe EAX or EDX. */
#include "cmre.h"
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);

void CM_THISCALL cm_fn_00544f30(void *self)
{
    u8 *base = (u8 *)self;
    u32 field;

    field = *(u32 *)(base + 0x20);
    if (field != 0) {
        cm_fn_009452ca(field);
    }
    field = *(u32 *)(base + 0x24);
    if (field != 0) {
        cm_fn_009452ca(field);
    }
    field = *(u32 *)(base + 0x28);
    if (field != 0) {
        cm_fn_009452ca(field);
    }
    field = *(u32 *)(base + 0x2c);
    if (field != 0) {
        cm_fn_009452ca(field);
    }
    field = *(u32 *)(base + 0x30);
    if (field != 0) {
        cm_fn_009452ca(field);
    }
    field = *(u32 *)(base + 0x34);
    if (field != 0) {
        cm_fn_009452ca(field);
    }

    field = *(u32 *)(base + 0x38);
    if (field != 0) {
        cm_fn_009452ca(field);
    }

    field = *(u32 *)(base + 0x3c);
    if (field != 0) {
        cm_fn_009452ca(field);
    }
}
