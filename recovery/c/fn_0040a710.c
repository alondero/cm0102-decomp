#include "cmre.h"

extern u32 CM_THISCALL cm_fn_0040a7d0(u32 self, u32 param_2, u32 param_3);
extern u32 CM_THISCALL cm_fn_0054c8e0(u32 self, u32 param_2, u32 param_3);

typedef struct { void *prev; void *handler; s32 state; } cm_seh_record;

/* Thiscall (ECX = self) with one stack argument (arg_04 = [entry+4]), `ret 4`,
   no return value. The SEH frame is one volatile record linked at entry and
   unlinked from its saved prev at exit; state is -1 until the 0x54c8e0 call and
   0 from then on. `frame` is the six-dword scratch block below the frame
   (sub esp, 0x18): frame[0..1] receive the two dwords read through the pointer
   returned by 0x54c8e0, frame[2] is the output buffer passed to 0x40a7d0, and
   frame[4] is the output buffer passed to 0x54c8e0.
   The packed argument to 0x40a7d0 is the saved fs:[0] value's high word with
   the argument's word at +2 in the low word (the listing writes only AX).
   Field offsets from self: 0x04 is a pointer whose +0x0c is the 0x40a7d0
   receiver; 0x0a is compared with a table field. Globals: count at 0x00ae235c,
   table base at 0x00ae23b4 (0x245-byte entries), 0x00de1f54 holds a pointer
   whose +0x91c is indexed by entry[0] << 4 when entry +0x53 matches self+0x0a. */
void CM_THISCALL cm_fn_0040a710(void *self, u32 arg_04)
{
    volatile u8 *base = (volatile u8 *)self;
    volatile cm_seh_record seh;
    void *entry_prev;
    void *restore_prev;
    u32 frame[6];
    u32 packed;
    u32 result;
    u32 ret2;
    u32 ecx_v;
    u32 edx_v;
    u32 esi_v;
    s32 count;
    s32 i;
    u32 table;
    u32 g;
    u32 t2;

    seh.state = -1;
    seh.handler = (void *)0x00953728;
    entry_prev = CM_FS(void *, 0);
    seh.prev = entry_prev;
    CM_FS(void *, 0) = (void *)&seh;

    packed = ((u32)entry_prev & 0xffff0000u) | (u32)*(volatile u16 *)(arg_04 + 2);
    ecx_v = *(volatile u32 *)(base + 0x04) + 0x0cu;
    result = cm_fn_0040a7d0(ecx_v, (u32)&frame[2], packed);

    ecx_v = (u32)&frame[4];
    seh.state = 0;
    ret2 = cm_fn_0054c8e0(result, ecx_v, 1u);

    ecx_v = *(volatile u32 *)ret2;
    edx_v = *(volatile u32 *)(ret2 + 4);
    frame[0] = ecx_v;
    frame[1] = edx_v;
    if ((u16)(frame[0] >> 16) == *(volatile u16 *)(arg_04 + 2) &&
        (u16)frame[0] == *(volatile u16 *)arg_04) {
        count = *(volatile s32 *)0x00ae235c;
        table = *(volatile u32 *)0x00ae23b4;
        if (count > 0) {
            i = 0;
            do {
                edx_v = *(volatile u32 *)(table + 0x53);
                esi_v = *(volatile u32 *)(base + 0x0a);
                if (edx_v == esi_v) {
                    g = *(volatile u32 *)0x00de1f54;
                    edx_v = *(volatile u32 *)table;
                    t2 = *(volatile u32 *)(g + 0x91c);
                    *(volatile u8 *)((edx_v << 4) + t2) = 0;
                }
                count = *(volatile s32 *)0x00ae235c;
                i++;
                table += 0x245u;
            } while (i < count);
        }
    }

    restore_prev = seh.prev;
    CM_FS(void *, 0) = restore_prev;
}
