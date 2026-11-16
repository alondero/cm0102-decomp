/* fn_00665f50 - derived-class destructor (thiscall, this in ECX).
   Listing: preferred-image VA 0x00665f50 (image base 0x00400000), 201 bytes.
   Instruction addresses in the comments refer to that listing. */
#include "cmre.h"

extern void CM_FASTCALL cm_fn_0049f450(u8 *param_1);
extern void CM_THISCALL cm_fn_00518690(void *self_arg);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

void CM_THISCALL cm_fn_00665f50(u32 self)
{
    /* 00665f50 push -1 / 00665f52 push 0x95c248 / 00665f57-5d push fs:[0],
       mov fs:[0], esp: one SEH record { prev, handler, state }. */
    volatile struct {
        u32 this_slot;
        struct { u32 prev, handler, state; } frame;
    } registration;
    registration.frame.state = 0xffffffffu;
    registration.frame.handler = 0x0095c248u;
    registration.frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&registration.frame;
    /* 00665f65-6a: slot immediately below the registration holds ECX. */
    registration.this_slot = self;

    /* 00665f6e mov dword ptr [esi], 0x96cc48 */
    *(volatile u32 *)self = 0x0096cc48u;

    /* 00665f74 mov eax, dword ptr [esi + 0xa7] ; 00665f7a state = 0 ;
       00665f86-8c push eax / call 0x9452ca */
    u32 pointer = *(volatile u32 *)(self + 0xa7u);
    registration.frame.state = 0u;
    if (pointer != 0)
        cm_fn_009452ca(pointer);

    /* 00665f8f mov eax, dword ptr [esi + 0xa3] ; 00665f99-9f call 0x9452ca */
    pointer = *(volatile u32 *)(self + 0xa3u);
    if (pointer != 0)
        cm_fn_009452ca(pointer);

    /* 00665fa2 mov eax, dword ptr [esi + 0xad] ; 00665fa8 test eax, eax */
    if (*(volatile u32 *)(self + 0xadu) != 0) {
        volatile u32 *const arrayp = (volatile u32 *)(self + 0xadu);
        volatile s16 *const countp = (volatile s16 *)(self + 0x3cu);
        u32 index = 0;

        /* 00665fae cmp word ptr [esi + 0x3c], di / 00665fb2 jle */
        if (*countp > 0) {
            for (;;) {
                /* 00665fb4 mov eax, dword ptr [esi + 0xad] ;
                   00665fba mov eax, dword ptr [eax + edi*4] ;
                   00665fc1-7 push eax / call 0x9452ca */
                u32 element = *(volatile u32 *)(*arrayp + index * 4u);
                if (element != 0)
                    cm_fn_009452ca(element);
                /* 00665fca movsx ecx, word ptr [esi + 0x3c] ;
                   00665fce inc edi ; 00665fcf cmp edi, ecx */
                ++index;
                if (!((s32)index < (s32)*countp))
                    break;
            }
        }
        /* 00665fd3 mov edx, dword ptr [esi + 0xad] ; 00665fd9-df call 0x9452ca */
        cm_fn_009452ca(*arrayp);
    }

    /* 00665fe2 mov edi, dword ptr [esi + 8] ; 00665fe9-feb mov ecx, edi /
       call 0x49f450 ; 00665ff0-f6 push edi / call 0x944c94 */
    u32 child = *(volatile u32 *)(self + 8u);
    if (child != 0) {
        cm_fn_0049f450((u8 *)child);
        cm_fn_00944c94((void *)child);
    }

    /* 00665ffb mov dword ptr [esp + 0x14], 0xffffffff ; 00666003 call 0x518690 */
    registration.frame.state = 0xffffffffu;
    cm_fn_00518690((void *)self);

    /* 00666008 mov ecx, dword ptr [esp + 0xc] ; 0066600e mov dword ptr fs:[0], ecx */
    CM_FS(u32, 0) = registration.frame.prev;
}
