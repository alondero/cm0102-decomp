/* fn_00592440 -- preferred-image VA 0x00592440, 201 bytes, CM0102 3.9.68.
   Destructor for the object whose vtable is installed at [this] (0x0096a360).

   Translation notes (listing order preserved):
   - Entry: ECX is read before written, so the receiver arrives in ECX
     (thiscall, no stack arguments: the epilogue is "add esp, 0x10; ret").
   - 0x00592440..0x0059244e install an SEH frame {prev, handler, state}:
     state = -1, handler = 0x00958ed8, prev = fs:[0], then fs:[0] = frame.
   - 0x00592455 reserves the receiver slot immediately below the frame;
     0x0059245a stores the receiver there.
   - 0x0059245e stores the vtable pointer 0x0096a360 into [this].
   - 0x00592464..0x005924cf release this->0xa7, this->0xa3 and the array at
     this->0xad (its elements, count taken from the word at this+0x3c, then
     the array itself). Every load of this->0xad / this+0x3c is repeated
     after a release call, exactly as in the listing.
   - 0x005924d2..0x005924e6 destroy and release this->8 when non-zero.
   - 0x005924e9..0x00592508 call the base destructor with the receiver and
     restore the previous SEH link.

   Field addresses are loop invariant (ESI is not modified inside the loop),
   so only the address arithmetic is hoisted; every load keeps the original
   width, order and reload points.

   Callee boundaries stay explicit (original code). */

#include "cmre.h"

extern void CM_FASTCALL cm_fn_0049f450(u8 *param_1);
extern void CM_THISCALL cm_fn_00518690(void *self_arg);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

void CM_THISCALL cm_fn_00592440(u32 self)
{
    /* One allocation preserves the receiver at frame-4 and the linked
       SEH record { prev, handler, state } at frame+0, +4, +8. */
    volatile struct {
        u32 saved_self;
        struct { u32 prev, handler, state; } frame;
    } locals;

    locals.frame.state = 0xffffffffu;       /* push -1            */
    locals.frame.handler = 0x00958ed8u;      /* push 0x958ed8      */
    locals.frame.prev = CM_FS(u32, 0);       /* push dword fs:[0]  */
    CM_FS(u32, 0) = (u32)&locals.frame;      /* mov fs:[0], esp    */
    locals.saved_self = self;               /* 0x0059245a: [esp+8] */

    CM_GLOBAL(volatile u32, self) = 0x0096a360u; /* mov [esi], 0x96a360 */

    /* 0x00592464: load this->0xa7, then clear the SEH state word. */
    u32 value = CM_GLOBAL(volatile u32, self + 0xa7u);
    locals.frame.state = 0;
    if (value != 0) {
        cm_fn_009452ca(value);        /* push eax; call 0x9452ca */
    }

    /* 0x0059247f: this->0xa3 */
    value = CM_GLOBAL(volatile u32, self + 0xa3u);
    if (value != 0) {
        cm_fn_009452ca(value);
    }

    /* 0x00592492: this->0xad array and the word count at this+0x3c */
    u32 *array_slot = (u32 *)(self + 0xadu);
    s16 *count_slot = (s16 *)(self + 0x3cu);
    if (*(volatile u32 *)array_slot != 0) {
        if (*(volatile s16 *)count_slot > 0) {   /* cmp word [esi+0x3c], di */
            u32 index = 0;
            do {
                /* 0x005924a4: reload the array, then read element[index]. */
                u32 element = *(volatile u32 *)(*(volatile u32 *)array_slot + index * 4u);
                if (element != 0) {
                    cm_fn_009452ca(element);
                }
                /* 0x005924ba: sign extended word count, index++ */
                index++;
                if ((s32)index >= (s32)*(volatile s16 *)count_slot) {
                    break;
                }
            } while (1);
        }
        /* 0x005924c3: release the array itself. */
        cm_fn_009452ca(*(volatile u32 *)array_slot);
    }

    /* 0x005924d2: child object at this+8 */
    u32 child = CM_GLOBAL(volatile u32, self + 8u);
    if (child != 0) {
        cm_fn_0049f450((u8 *)child);        /* mov ecx, edi; call 0x49f450 */
        cm_fn_00944c94((void *)child);      /* push edi; call 0x944c94    */
    }

    /* 0x005924e9: base destructor; state word set to -1 before the call. */
    locals.frame.state = 0xffffffffu;
    cm_fn_00518690((void *)self);

    u32 restore_prev = locals.frame.prev;
    CM_FS(u32, 0) = restore_prev;
}
