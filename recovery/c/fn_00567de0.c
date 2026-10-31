#include "cmre.h"

/* Offsets from the object passed in ECX. Every field is volatile so the loads
   happen in listing order: field_0x4, then field_0x8, then field_0x0. */
struct cm_net_buffer {
    volatile u32 field_0x0;
    volatile u32 field_0x4;
    volatile u32 field_0x8;
};

/* 0x0090d130 takes no arguments (body: mov eax, 0xad4394; ret). The three
   dwords pushed before its call are trailing variadic arguments of 0x00944c9f:
   cm_fn_00944c9f(0xae24d0, 0x9870e8, cm_fn_0090d130(), L+0x100, L+0, 0x93). */
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3,
                                   void *arg4, void *arg5);
/* EAX after this call is the return value of the error path (callers read it). */
extern u32 CM_CDECL cm_fn_005e8290(const char *title, const char *message,
                                   s32 arg);

u32 CM_THISCALL cm_fn_00567de0(struct cm_net_buffer *self, u32 *dest)
{
    u8 scratch[0x200];
    u32 index;
    u32 limit;

    index = self->field_0x4;
    limit = self->field_0x8;
    if (limit < index + 4u) {
        char *text;
        u32 status;

        cm_fn_00944cff(0x009c3734u, 0u, 0u,
                       (void *)&scratch[0x100], (void *)&scratch[0]);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text,
                       (void *)&scratch[0x100], (void *)&scratch[0], 0x93u);
        status = cm_fn_005e8290((const char *)0x009870e0u,
                                (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
        return status;
    }

    {
        volatile u32 *source = (volatile u32 *)(self->field_0x0 + index);
        u32 value = *source;
        *(volatile u32 *)dest = value;
    }
    {
        u32 next = self->field_0x4 + 4u;
        self->field_0x4 = next;
        return next;
    }
}
