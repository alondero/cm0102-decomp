#include "cmre.h"

/* 0x944c3a is an import thunk (jmp [0x967220]) to closesocket, __stdcall with
   one argument; the listing has no `add esp` after its calls, so the callee
   pops it. 0x944c34 is WSACleanup, __stdcall with no arguments. */
extern s32 CM_STDCALL cm_fn_00944c3a(u32 s);
extern s32 CM_STDCALL cm_fn_00944c34(void);

/* Thiscall (ECX = self), no stack arguments, `ret 0`, no return value.
   Field offsets from self: 0x00 is a table pointer, 0x04 is a signed 16-bit
   count, 0x288 and 0x2d8 are 20-slot u32 arrays, 0x4b6 is a u32 handle.
   Each live slot (not 0xffffffff, and table non-null for the first array) is
   passed to 0x944c3a and then overwritten with 0xffffffff. The 0x4b6 handle
   is released through 0x944c34 and zeroed when non-zero. The index is a
   16-bit counter (DI) read as signed for the table offset. */
void CM_THISCALL cm_fn_008ae2a0(void *self)
{
    volatile u8 *base = (volatile u8 *)self;
    u16 index;
    u32 off;
    u32 table;
    u32 elem;
    u32 slot;
    u32 remaining;

    index = 0;
    if (*(volatile s16 *)(base + 0x04) > (s16)index) {
        do {
            table = *(volatile u32 *)base;
            if (table != 0) {
                off = (u32)((s32)(s16)index * 4);
                elem = *(volatile u32 *)(table + off);
                if (elem != 0xffffffffu) {
                    cm_fn_00944c3a(elem);
                    table = *(volatile u32 *)base;
                    *(volatile u32 *)(table + off) = 0xffffffffu;
                }
            }
            index++;
        } while ((s16)index < *(volatile s16 *)(base + 0x04));
    }

    slot = (u32)base + 0x2d8;
    remaining = 0x14;
    do {
        elem = *(volatile u32 *)slot;
        if (elem != 0xffffffffu) {
            cm_fn_00944c3a(elem);
            *(volatile u32 *)slot = 0xffffffffu;
        }
        slot += 4;
        remaining--;
    } while (remaining != 0);

    slot = (u32)base + 0x288;
    remaining = 0x14;
    do {
        elem = *(volatile u32 *)slot;
        if (elem != 0xffffffffu) {
            cm_fn_00944c3a(elem);
            *(volatile u32 *)slot = 0xffffffffu;
        }
        slot += 4;
        remaining--;
    } while (remaining != 0);

    elem = *(volatile u32 *)(base + 0x4b6);
    if (elem != 0) {
        cm_fn_00944c34();
        *(volatile u32 *)(base + 0x4b6) = 0;
    }
}
