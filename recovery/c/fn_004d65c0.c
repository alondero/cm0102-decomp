/* Preferred-image VA 0x004d65c0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STDCALL3/RET12. The indexed caller overwrites EAX after this call.
   The original's 0x130-byte scratch has a saved selection DWORD at L0 and
   the uninitialized 300-byte text buffer at L4. Native callees remain boundaries;
   the getter's incidental EDX:EAX adapter result is narrowed to its loaded EAX. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define S8(a) CM_GLOBAL(volatile s8, (a))
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern u32 CM_CDECL cm_fn_0053e140(u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32 output, u32 format, ...);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self,u32 text);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32 self,u32 text,u32 code,u32 link);
extern u32 CM_CDECL cm_fn_0053e990(u32,u32,u32,u32,u32);

void CM_STDCALL cm_fn_004d65c0(u32 selection, u32 club_mode, u32 target)
{
    u8 scratch[0x130];
    u32 l = (u32)scratch;
    u32 person, first, second, third, club, offset, base, result;
    s32 marker;
    person = (u32)cm_fn_00785830((void *)selection,0u);
    first = (u32)cm_fn_00785830((void *)selection,1u);
    second = (u32)cm_fn_00785830((void *)selection,2u);
    third = (u32)cm_fn_00785830((void *)selection,3u);
    D(l) = third;
    club = (u32)cm_fn_00785830((void *)selection,4u);
    if (club_mode != 0u) {
        third = D(l);
        (void)cm_fn_0053e140(first,second,third,l+4u,300u,1u,person);
        base = D(0x00ae23b4u);
        offset = club * 0x245u;
        marker = S8(offset + base + 0x52u);
        (void)cm_fn_0066f4e0(0x00de1f64u,0x009aa484u,
                             (u32)marker,(u32)marker,offset+base+0x38u,l+4u);
        (void)cm_fn_004ae660((void *)target,0x00de1f64u);
        base = D(0x00ae23b4u);
        (void)cm_fn_004ae8a0(target,offset+base+0x38u,0x7d5u,offset+base);
        third = D(l);
        base = D(0x00ae23bcu);
        offset = person * 110u;
        result = cm_fn_0053e990(offset+base,first,second,third,0u);
        if (result != 0u) {
            base = D(0x00ae23bcu);
            (void)cm_fn_004ae8a0(target,l+4u,0x7d7u,offset+base);
        }
    } else {
        third = D(l);
        (void)cm_fn_0053e140(first,second,third,l+4u,300u,5u,person);
        (void)cm_fn_0066f4e0(0x00de1f64u,0x009aa448u,100u,l+4u);
        (void)cm_fn_004ae660((void *)target,0x00de1f64u);
        base = D(0x00ae23bcu);
        offset = person * 110u;
        result = cm_fn_0053e990(offset+base,first,second,third,0u);
        if (result != 0u) {
            base = D(0x00ae23bcu);
            (void)cm_fn_004ae8a0(target,l+4u,0x7d7u,offset+base);
        }
    }
}
