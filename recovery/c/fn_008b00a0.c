#include "cmre.h"

extern u32 CM_THISCALL cm_fn_00414320(u32 self, u32 param_1, u32 param_2);
extern u32 CM_FASTCALL cm_fn_0068d020(u16 *param_1);
extern unsigned long long CM_THISCALL cm_fn_0068d0d0(volatile u8 *self, const u8 *src, s16 count);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);

typedef struct { void *prev; void *handler; s32 state; } cm_seh_record;

/* Thiscall (ECX = self), no stack arguments, `ret 0`, returns the u32 held in
   ESI (EAX at exit). Field offsets from self: 0x252 is a signed byte bound,
   0x253 a signed byte added to 0x0b, 0x254 and 0x258 are u32 values passed to
   0x414320 (thiscall on the global object at 0x00ad9c60; the result is the
   routine's ESI), 0x4e is a sub-object offset passed to 0x68d0d0 with count
   0x205. Allocation (0x0e) happens only when 0x414320 returned zero and the
   bound test passes. The SEH frame is one volatile record: state is -1 at
   entry, the value of ESI (zero on that path) before the allocation test, and
   -1 after the fastcall 0x68d020 returns. */
u32 CM_THISCALL cm_fn_008b00a0(void *self)
{
    volatile u8 *base = (volatile u8 *)self;
    volatile cm_seh_record seh;
    void *entry_prev;
    void *restore_prev;
    u32 arg_258;
    u32 arg_254;
    u32 obj;
    u32 esi_val;
    u32 alloc;
    s32 bound;
    s32 limit;

    seh.state = -1;
    seh.handler = (void *)0x009637eb;
    entry_prev = CM_FS(void *, 0);
    seh.prev = entry_prev;
    CM_FS(void *, 0) = (void *)&seh;

    arg_258 = *(volatile u32 *)(base + 0x258);
    arg_254 = *(volatile u32 *)(base + 0x254);
    obj = CM_GLOBAL(u32, 0x00ad9c60);
    esi_val = cm_fn_00414320(obj, arg_254, arg_258);

    limit = (s32)*(volatile s8 *)(base + 0x253) + 0x0b;
    bound = (s32)*(volatile s8 *)(base + 0x252);
    if (bound >= limit) {
        if (esi_val == 0) {
            alloc = (u32)cm_fn_00944cf1(0x0e);
            seh.state = (s32)esi_val;
            if (alloc != 0)
                esi_val = cm_fn_0068d020((u16 *)alloc);
            else
                esi_val = 0;
            seh.state = -1;
        }
        cm_fn_0068d0d0((volatile u8 *)esi_val, (const u8 *)(base + 0x4e), (s16)0x205);
    }

    restore_prev = seh.prev;
    CM_FS(void *, 0) = restore_prev;
    return esi_val;
}
